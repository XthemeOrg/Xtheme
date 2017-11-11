/*
 * Copyright (c) 2017 Austin Ellis <siniStar@IRC4Fun.net>
 * Rights to this code are as documented in doc/LICENSE.
 *
 * ChanServ/REGPROC is similar to ChanServ/MODERATE in the sense that
 * Services Operators review channels requesting registration, however,
 * it also requires the SUPPORT of TWO (registered) users.
 *
 * Users who have NOSUPPORT enabled will not be able to be used as
 * channel SUPPORTers (as they have indicated by use of the setting,
 * that they do not wish to SUPPORT channels in the Registration
 * process.)
 *
 * Each Supporter will be notified of being used as a Supporter for a
 * given channel's registration and will need to indicate whether they
 * SUPPORT the channel or not.  Services Operators can review channels
 * at any time, even if the SUPPORT verification process has not completed.
 *
 * Once both Supporters have verified their SUPPORT for a Channel's
 * Registration, the channel application is moved in the REGPROC to
 * "PENDING REVIEW" and Services Operators are notified (if set) via a
 * Group memo that the channel is now ready for review.
 *
 * -Supporters are notified when listed on a Channel Application
 * via Memo.
 * -Staff is notified of "Pending Review" channels via Group Memo.
 * -Founders are notified when the Channel is reviewed via Memo.
 *
 * -General Registration Process (REGPROC) Flow:
 * Submitted>Supporter Notification>Supporter Verification>Pending Review
 *
 */

#include "atheme.h"
#include "chanserv.h"

DECLARE_MODULE_V1
(
	"chanserv/regproc", true, _modinit, _moddeinit,
	PACKAGE_STRING,
	VENDOR_STRING
);

unsigned int ratelimit_count = 0;
time_t ratelimit_firsttime = 0;

static void cs_cmd_register(sourceinfo_t *si, int parc, char *parv[]);
static void cs_cmd_waiting(sourceinfo_t *si, int parc, char *parv[]);
static void cs_cmd_reject(sourceinfo_t *si, int parc, char *parv[]);
static void cs_cmd_activate(sourceinfo_t *si, int parc, char *parv[]);
static void cs_cmd_support(sourceinfo_t *si, int parc, char *parv[]);
static void cs_cmd_nosupport(sourceinfo_t *si, int parc, char *parv[]);

static void write_csrq(database_handle_t *db);
static void write_csrs1(database_handle_t *db);
static void write_csrs2(database_handle_t *db);
static void db_h_csrq(database_handle_t *db, const char *type);
static void db_h_csrs1(database_handle_t *db, const char *type);
static void db_h_csrs2(database_handle_t *db, const char *type);

command_t cs_register = { "REGISTER", N_("Starts an Application to Register a channel."),
                           AC_AUTHENTICATED, 5, cs_cmd_register, { .path = "cservice/register" } };
command_t cs_waiting  = { "WAITING", N_("View pending registrations"), PRIV_CHAN_ADMIN,
				 1, cs_cmd_waiting, { .path = "cservice/waiting" } };
command_t cs_reject   = { "REJECT", N_("Rejects a pending registration"), PRIV_CHAN_ADMIN,
				 2, cs_cmd_reject, { .path = "cservice/reject" } };
command_t cs_activate = { "ACTIVATE", N_("Activates a pending registration"), PRIV_CHAN_ADMIN,
				 2, cs_cmd_activate, { .path = "cservice/activate" } };
command_t cs_support = { "SUPPORT", N_("Support a Channel Registration."),
                           AC_AUTHENTICATED, 2, cs_cmd_support, { .path = "cservice/support" } };
command_t cs_nosupport = { "NOSUPPORT", N_("Toggles the ability to be used as a Supporter for Channel Registrations."),
                           AC_AUTHENTICATED, 2, cs_cmd_nosupport, { .path = "cservice/nosupport" } };

static char *groupmemo;

struct csrq_ {
	char *chan;
	time_t chan_ts;
	char *founder;
	char *status;
	char *supp1;
	char *supp2;
};

typedef struct csrq_ csrq_t;

mowgli_list_t cs_csrqlist;

struct csrs1_ {
	char *chan;
	char *supp1;
	char *supp1rply;
};

typedef struct csrs1_ csrs1_t;

mowgli_list_t cs_csrs1list;

struct csrs2_ {
	char *chan;
	char *supp2;
	char *supp2rply;
};

typedef struct csrs2_ csrs2_t;

mowgli_list_t cs_csrs2list;

static void send_memo(sourceinfo_t *si, myuser_t *mu, const char *memo, ...)
{
	service_t *msvs;
	va_list va;
	char buf[BUFSIZE];

	return_if_fail(si != NULL);
	return_if_fail(mu != NULL);
	return_if_fail(memo != NULL);

	va_start(va, memo);
	vsnprintf(buf, BUFSIZE, memo, va);
	va_end(va);

	if ((msvs = service_find("memoserv")) == NULL)
		myuser_notice(chansvs.nick, mu, "%s", buf);
	else
	{
		char cmdbuf[BUFSIZE];

		mowgli_strlcpy(cmdbuf, entity(mu)->name, BUFSIZE);
		mowgli_strlcat(cmdbuf, " ", BUFSIZE);
		mowgli_strlcat(cmdbuf, buf, BUFSIZE);

		command_exec_split(msvs, si, "SEND", cmdbuf, msvs->commands);
	}
}

static void send_group_memo(sourceinfo_t *si, const char *memo, ...)
{
	service_t *msvs;
	va_list va;
	char buf[BUFSIZE];

	return_if_fail(si != NULL);
	return_if_fail(memo != NULL);

	va_start(va, memo);
	vsnprintf(buf, BUFSIZE, memo, va);
	va_end(va);

	if ((msvs = service_find("memoserv")) == NULL)
		return;
	else
	{
		char cmdbuf[BUFSIZE];

		mowgli_strlcpy(cmdbuf, groupmemo, BUFSIZE);
		mowgli_strlcat(cmdbuf, " ", BUFSIZE);
		mowgli_strlcat(cmdbuf, buf, BUFSIZE);

		command_exec_split(msvs, si, "SEND", cmdbuf, msvs->commands);
	}
}


void _modinit(module_t *m)
{
	if (!module_find_published("backend/opensex"))
	{
		slog(LG_INFO, "Module %s requires use of the OpenSEX database backend, refusing to load.", m->name);
		m->mflags = MODTYPE_FAIL;
		return;
	}
	MODULE_CONFLICT(m, "chanserv/register");
	MODULE_CONFLICT(m, "chanserv/moderate");

	hook_add_db_write(write_csrq);
	hook_add_db_write(write_csrs1);
	hook_add_db_write(write_csrs2);

	db_register_type_handler("CSRQ", db_h_csrq);
	db_register_type_handler("CSRS1", db_h_csrs1);
	db_register_type_handler("CSRS2", db_h_csrs2);

	service_named_bind_command("chanserv", &cs_register);
	service_named_bind_command("chanserv", &cs_waiting);
	service_named_bind_command("chanserv", &cs_reject);
	service_named_bind_command("chanserv", &cs_activate);
	service_named_bind_command("chanserv", &cs_support);
	service_named_bind_command("chanserv", &cs_nosupport);
}

void _moddeinit(module_unload_intent_t intent)
{
	hook_del_db_write(write_csrq);
	hook_del_db_write(write_csrs1);
	hook_del_db_write(write_csrs2);

	db_unregister_type_handler("CSRQ");
	db_unregister_type_handler("CSRS1");
	db_unregister_type_handler("CSRS2");

	service_named_unbind_command("chanserv", &cs_register);
	service_named_unbind_command("chanserv", &cs_waiting);
	service_named_unbind_command("chanserv", &cs_reject);
	service_named_unbind_command("chanserv", &cs_activate);
	service_named_unbind_command("chanserv", &cs_support);
	service_named_unbind_command("chanserv", &cs_nosupport);
}

static void write_csrq(database_handle_t *db)
{
	mowgli_node_t *n;

	MOWGLI_ITER_FOREACH(n, cs_csrqlist.head)
	{
		csrq_t *l = n->data;

		db_start_row(db, "CSRQ");
		db_write_word(db, l->chan);
		db_write_time(db, l->chan_ts);
		db_write_word(db, l->founder);
		db_write_word(db, l->status);
		db_write_word(db, l->supp1);
		db_write_word(db, l->supp2);
		db_commit_row(db);
	}
}

static void write_csrs1(database_handle_t *db)
{
	mowgli_node_t *n;

	MOWGLI_ITER_FOREACH(n, cs_csrs1list.head)
	{
		csrs1_t *l = n->data;

		db_start_row(db, "CSRS1");
		db_write_word(db, l->chan);
		db_write_word(db, l->supp1);
		db_write_word(db, l->supp1rply);
		db_commit_row(db);
	}
}

static void write_csrs2(database_handle_t *db)
{
	mowgli_node_t *n;

	MOWGLI_ITER_FOREACH(n, cs_csrs2list.head)
	{
		csrs2_t *l = n->data;

		db_start_row(db, "CSRS2");
		db_write_word(db, l->chan);
		db_write_word(db, l->supp2);
		db_write_word(db, l->supp2rply);
		db_commit_row(db);
	}
}

static void db_h_csrq(database_handle_t *db, const char *type)
{
	const char *chan = db_sread_word(db);
	time_t chan_ts = db_sread_time(db);
	const char *founder = db_sread_word(db);
	const char *status = db_sread_word(db);
	const char *supp1 = db_sread_word(db);
	const char *supp2 = db_sread_word(db);

	csrq_t *l = smalloc(sizeof(csrq_t));
	l->chan = sstrdup(chan);
	l->chan_ts = chan_ts;
	l->founder = sstrdup(founder);
	l->status = sstrdup(status);
	l->supp1 = sstrdup(supp1);
	l->supp2 = sstrdup(supp2);
	mowgli_node_add(l, mowgli_node_create(), &cs_csrqlist);
}

static void db_h_csrs1(database_handle_t *db, const char *type)
{
	const char *chan = db_sread_word(db);
	const char *supp1 = db_sread_word(db);
	const char *supp1rply = db_sread_word(db);

	csrs1_t *l = smalloc(sizeof(csrs1_t));
	l->chan = sstrdup(chan);
	l->supp1 = sstrdup(supp1);
	l->supp1rply = sstrdup(supp1rply);
	mowgli_node_add(l, mowgli_node_create(), &cs_csrs1list);
}

static void db_h_csrs2(database_handle_t *db, const char *type)
{
	const char *chan = db_sread_word(db);
	const char *supp2 = db_sread_word(db);
	const char *supp2rply = db_sread_word(db);

	csrs2_t *l = smalloc(sizeof(csrs2_t));
	l->chan = sstrdup(chan);
	l->supp2 = sstrdup(supp2);
	l->supp2rply = sstrdup(supp2rply);
	mowgli_node_add(l, mowgli_node_create(), &cs_csrs2list);
}

static void cs_cmd_register(sourceinfo_t *si, int parc, char *parv[])
{
	channel_t *c;
	chanuser_t *cu, *cu2;
	mychan_t *mc;
	myuser_t *mu;
	user_t *tu1, *tu2;
	char *name = parv[0];
	char str[21];
	unsigned int fl;
	char *status = NULL;
	char *supp1 = parv[1];
	char *supp1rply = NULL;
	char *supp2 = parv[2];
	char *supp2rply = NULL;
	mowgli_node_t *n, *tn;
	csrq_t *l;

	/* This command is not useful on registered channels, ignore it if
	 * it is a fantasy command so users can program bots to react on
	 * it without interference from ChanServ.
	 */
	if (si->c != NULL)
		return;

	if (!name)
	{
		command_fail(si, fault_needmoreparams, STR_INSUFFICIENT_PARAMS, "REGISTER");
		command_fail(si, fault_needmoreparams, _("To register a channel: REGISTER <#channel> <supporter1> <supporter2>"));
		return;
	}

	if (!supp1)
	{
		command_fail(si, fault_needmoreparams, STR_INSUFFICIENT_PARAMS, "REGISTER");
		command_fail(si, fault_needmoreparams, _("To register a channel: REGISTER <#channel> <supporter1> <supporter2>"));
		return;
	}

	if (!supp2)
	{
		command_fail(si, fault_needmoreparams, STR_INSUFFICIENT_PARAMS, "REGISTER");
		command_fail(si, fault_needmoreparams, _("To register a channel: REGISTER <#channel> <supporter1> <supporter2>"));
		return;
	}

	if (*supp1 == *supp2)
	{
		command_fail(si, fault_noprivs, _("You cannot use the same Supporter twice!"));
		return;
	}

	if (*name != '#')
	{
		command_fail(si, fault_badparams, STR_INVALID_PARAMS, "REGISTER");
		command_fail(si, fault_badparams, _("Syntax: REGISTER <#channel> <supporter1> <supporter2>"));
		return;
	}

	if (si->smu->flags & MU_WAITAUTH)
	{
		command_fail(si, fault_notverified, _("You need to \2verify your email address\2 before you may register channels."));
		return;
	}

	/* make sure it isn't already registered */
	if ((mc = mychan_find(name)))
	{
		if (!use_channel_private || !(mc->flags & MC_PRIVATE))
			command_fail(si, fault_alreadyexists, _("\2%s\2 is already registered to \2%s\2."), mc->name, mychan_founder_names(mc));
		else
			command_fail(si, fault_alreadyexists, _("\2%s\2 is already registered."), mc->name);
		return;
	}

	/* make sure it isn't already in request process */
	MOWGLI_ITER_FOREACH(n, cs_csrqlist.head)
	{
		l = n->data;

		if (!irccasecmp(l->chan, name))
		{
			command_fail(si, fault_alreadyexists, _("Sorry, \2%s\2 has already been requested and is in the Registration process."), name);
			return;
		}
	}

	/* make sure the channel exists */
	if (!(c = channel_find(name)))
	{
		command_fail(si, fault_nosuch_target, _("The channel \2%s\2 must exist in order to register it."), name);
		return;
	}

	/* make sure they're in it */
	if (!(cu = chanuser_find(c, si->su)))
	{
		command_fail(si, fault_noprivs, _("You must be in \2%s\2 in order to register it."), name);
		return;
	}

	/* make sure they're opped (or protected/owner on unreal/inspircd) */
	if (!((CSTATUS_OP | CSTATUS_PROTECT | CSTATUS_OWNER) & cu->modes))
	{
		command_fail(si, fault_noprivs, _("You must be a channel operator in \2%s\2 in order to register it."), name);
		return;
	}

	if (metadata_find(si->smu, "private:restrict:setter"))
	{
		command_fail(si, fault_noprivs, _("You have been restricted from registering channels by network staff."));
		return;
	}

	mu = myuser_find(supp1);
	if (!mu)
	{
		command_fail(si, fault_nosuch_target, _("Account \2%s\2 is not registered, and cannot be used as a Supporter."), supp1);
		return;
	}

	if (MU_WAITAUTH & user(mu)->flags)
	{
		command_fail(si, fault_notverified, _("\2%s\2 needs to \2complete account verification\2 before they can SUPPORT channels."), supp1);
		return;
	}

	if (mu == si->smu)
	{
		command_fail(si, fault_noprivs, _("You cannot use yourself as a \2SUPPORTER\2 on your own application!"));
		return;
	}

	if (metadata_find(mu, "private:restrict:setter"))
	{
		command_fail(si, fault_noprivs, _("\2%s\2 is not allowed to \2SUPPORT\2 Channel Registrations."), supp1);
		return;
	}

	if (metadata_find(mu, "private:nosupport"))
	{
		command_fail(si, fault_noprivs, _("\2%s\2 has indicated that they do not wish to \2SUPPORT\2 Channel Registrations."), supp1);
		return;
	}

	mu = myuser_find(supp2);
	if (!mu)
	{
		command_fail(si, fault_nosuch_target, _("Account \2%s\2 is not registered, and cannot be used as a Supporter."), supp2);
		return;
	}

	if (MU_WAITAUTH & user(mu)->flags)
	{
		command_fail(si, fault_notverified, _("\2%s\2 needs to \2complete account verification\2 before they can SUPPORT channels."), supp2);
		return;
	}

	if (mu == si->smu)
	{
		command_fail(si, fault_noprivs, _("You cannot use yourself as a \2SUPPORTER\2 on your own application!"));
		return;
	}

	if (metadata_find(mu, "private:restrict:setter"))
	{
		command_fail(si, fault_noprivs, _("\2%s\2 is not allowed to \2SUPPORT\2 Channel Registrations."), supp2);
		return;
	}

	if (metadata_find(mu, "private:nosupport"))
	{
		command_fail(si, fault_noprivs, _("\2%s\2 has indicated that they do not wish to \2SUPPORT\2 Channel Registrations."), supp2);
		return;
	}

	if ((unsigned int)(CURRTIME - ratelimit_firsttime) > config_options.ratelimit_period)
		ratelimit_count = 0, ratelimit_firsttime = CURRTIME;

	if (ratelimit_count > config_options.ratelimit_uses && !has_priv(si, PRIV_FLOOD))
	{
		command_fail(si, fault_toomany, _("The system is currently too busy to process your registration, please try again later."));
		slog(LG_INFO, "REGISTER:THROTTLED: \2%s\2 by \2%s\2", name, entity(si->smu)->name);
		return;
	}

	else
	{

		l = smalloc(sizeof(csrq_t));
		l->chan = sstrdup(name);
		l->chan_ts = CURRTIME;;
		l->founder = sstrdup(entity(si->smu)->name);
		l->status = "NOTIFICATION";
		l->supp1 = sstrdup(supp1);
		l->supp2 = sstrdup(supp2);

		logcommand(si, CMDLOG_REGISTER, "REGISTER:REGPROC:STAGE1 \2%s\2 (Status: \2SUPPORTER NOTIFICATION\2) -- \2SUPPORTER1:\2 %s - \2SUPPORTER2:\2 %s", name, supp1, supp2);

		n = mowgli_node_create();
		mowgli_node_add(l, n, &cs_csrqlist);

		command_success_nodata(si, _("Your application to \2REGISTER %s\2 has been posted to the \2Registration Process\2."), name);
		command_success_nodata(si, _("\2Both\2 of your Supporters will need to use the \2SUPPORT\2 command to indicate that"));
		command_success_nodata(si, _("they support you in registering \2%s\2.  \2After\2 your Supporters indicate their"), name);
		command_success_nodata(si, _("support (or lack of support), \2%s\2 will be reviewed by \2%s Staff\2."), name, me.netname);
		command_success_nodata(si, _("\2NOTE:\2"));
		command_success_nodata(si, _("If \2%s\2 and \2%s\2 do not use the \2SUPPORT\2 command within \2 7 days\2,"), supp1, supp2);
		command_success_nodata(si, _("-OR- if \2they inform us that they DO NOT SUPPORT \2%s\2, it will be \2rejected\2."), name);
		command_success_nodata(si, _("[\2REGPROC\2] \2%s\2 is now in the \2SUPPORTER NOTIFICATION\2 status."), name);
		mu = myuser_find(supp1);
		send_memo(si, mu, "[auto memo] I used you as a SUPPORTER for \2%s\2. If you support me, please use the \2SUPPORT %s\2 command. If you do NOT SUPPORT me, please alert \2%s Staff\2.", name, me.netname);
		mu = myuser_find(supp2);
		send_memo(si, mu, "[auto memo] I used you as a SUPPORTER for \2%s\2. If you support me, please use the \2SUPPORT %s\2 command. If you do NOT SUPPORT me, please alert \2%s Staff\2.", name, me.netname);
		logcommand(si, CMDLOG_REGISTER, "REGISTER:REGPROC:STAGE2 \2%s\2 (Status: \2SUPPORTER VERIFICATION\2) -- \2SUPPORTER1:\2 %s - \2SUPPORTER2:\2 %s", name, supp1, supp2);
		command_success_nodata(si, _("[\2REGPROC\2] \2%s\2 is now in the \2SUPPORTER VERIFICATION\2 status."), name);
		return;
	}
}

static void cs_cmd_waiting(sourceinfo_t *si, int parc, char *parv[])
{
	mowgli_node_t *n, *n2, *n3, *tn;
	csrq_t *l;
	csrs1_t *s1;
	csrs2_t *s2;
	metadata_t *md, *md2;
	const char *supp1;
	const char *supp2;
	char *supp1rply;
	char *supp2rply;
	char *status;
	unsigned int i = 0;
	char strfbuf[BUFSIZE];

	char buf[BUFSIZE];
	struct tm tm;

	MOWGLI_ITER_FOREACH(n, cs_csrqlist.head)
	{
		l = n->data;

		if ((!l->chan) || !match(l->chan, l->chan))
		{

		MOWGLI_ITER_FOREACH(n2, cs_csrs1list.head)
		{
			s1 = n2->data;

			if (match(l->supp1, s1->supp1))
			{
				i++;
			}
		}

		MOWGLI_ITER_FOREACH(n3, cs_csrs2list.head)
		{
			s2 = n3->data;

			if (match(l->supp2, s2->supp2))
			{
				i++;
			}
		}

		if (i == 2)
		{
			status = "PENDING REVIEW";
		}
		else if (i == 1)
		{
			status = "SUPPORTER VERIFICATION";
		}
		else
		{
			status = "SUPPORTER NOTIFICATION";
		}


			tm = *localtime(&l->chan_ts);
			strftime(strfbuf, sizeof strfbuf, TIME_FORMAT, &tm);
			command_success_nodata(si, _("Channel: \2%s\2 Founder: \2%s\2 [%s (%s ago)] Status: \2%s\2 -- SUPPORTERS: \2%s\2, \2%s\2"), l->chan, l->founder, strfbuf, time_ago(l->chan_ts), status, l->supp1, l->supp2);
			i = 0;
		}

	}
	command_success_nodata(si, _("End of WAITING list."));
	logcommand(si, CMDLOG_GET, "WAITING");
}

static void cs_cmd_reject(sourceinfo_t *si, int parc, char *parv[])
{
	char *chan = parv[0];
	mowgli_node_t *n, *tn;
	csrq_t *l;
	csrs1_t *s1;
	csrs2_t *s2;
	myuser_t *mu;

	if (!parv[0])
	{
		command_fail(si, fault_needmoreparams, STR_INSUFFICIENT_PARAMS, "REJECT");
		command_fail(si, fault_needmoreparams, _("Syntax: REJECT <#channel>"));
		return;
	}

	MOWGLI_ITER_FOREACH(n, cs_csrqlist.head)
	{
		l = n->data;

		if (!irccasecmp(l->chan, chan))
		{
			mu = myuser_find(l->founder);
			send_memo(si, mu, "[\2REGPROC\2 auto memo] Your Channel Registration request for \2%s\2 has been \2Rejected\2.", l->chan);
			logcommand(si, CMDLOG_ADMIN, "REJECT: \2%s\2", l->chan);

			mowgli_node_delete(n, &cs_csrqlist);

			free(l->chan);
			free(l);

		}
	}

	command_success_nodata(si, _("\2%s\2 was rejected."), parv[0]);
	logcommand(si, CMDLOG_ADMIN, "REJECT: \2%s\2", parv[0]);
}

static void cs_cmd_activate(sourceinfo_t *si, int parc, char *parv[])
{
	myentity_t *mt;
	myuser_t *mu;
	mychan_t *mc;
	chanuser_t *cu;
	user_t *u;
	channel_t *c;
	char str[BUFSIZE];
	hook_channel_req_t hdata;
	sourceinfo_t baked_si;
	unsigned int fl;
	char *chan = parv[0];
	mowgli_node_t *n, *tn;
	csrq_t *l;

	if (!parv[0])
	{
		command_fail(si, fault_needmoreparams, STR_INSUFFICIENT_PARAMS, "ACTIVATE");
		command_fail(si, fault_needmoreparams, _("Syntax: ACTIVATE <#channel>"));
		return;
	}

	MOWGLI_ITER_FOREACH(n, cs_csrqlist.head)
	{
		l = n->data;

		if (!irccasecmp(l->chan, chan))
		{
			logcommand(si, CMDLOG_ADMIN, "ACTIVATE: \2%s\2", l->chan);
			c = channel_find(chan);
			mu = myuser_find(l->founder);
			send_memo(si, mu, "[\2REGPROC\2 auto memo] Your Channel Registration request for \2%s\2 has been \2Accepted\2!", l->chan);
			mc = mychan_add(chan);
			mc->registered = CURRTIME;
			mc->used = CURRTIME;
			mc->mlock_on |= (CMODE_NOEXT | CMODE_TOPIC);
			if (c != NULL && c->limit == 0)
				mc->mlock_off |= CMODE_LIMIT;
			if (c != NULL && c->key == NULL)
				mc->mlock_off |= CMODE_KEY;
			mc->flags |= config_options.defcflags;
			slog(LG_DEBUG, "cs_cmd_activate(): defcflags = %d, mc->flags = %d, guard? %s", config_options.defcflags, mc->flags, (mc->flags & MC_GUARD) ? "YES" : "NO");

			chanacs_add(mc, myentity_find_ext(l->founder), custom_founder_check(), CURRTIME, entity(si->smu));

			command_success_nodata(si, _("\2%s\2 is now registered to \2%s\2."), l->chan, l->founder);

			if (c != NULL && c->ts > 0)
			{
				snprintf(str, sizeof str, "%lu", (unsigned long)c->ts);
				metadata_add(mc, "private:channelts", str);
			}

			if (chansvs.deftemplates != NULL && *chansvs.deftemplates != '\0')
				metadata_add(mc, "private:templates", chansvs.deftemplates);


			if (mc->flags & MC_GUARD)
				join(mc->name, chansvs.nick);
			logcommand(si, CMDLOG_ADMIN, "ACTIVATE: \2%s\2", l->chan);

			mowgli_node_delete(n, &cs_csrqlist);

			free(l->chan);
			free(l);

		}
	}
}

static void cs_cmd_support(sourceinfo_t *si, int parc, char *parv[])
{
	char *chan = parv[0];
	char *supp1 = NULL;
	char *supp1rply = NULL;
	char *supp2 = NULL;
	char *supp2rply = NULL;
	mychan_t *mc;
	myuser_t *mu;
	mowgli_node_t *n, *n2, *n3, *tn;
	csrq_t *l;
	csrs1_t *s1;
	csrs2_t *s2;
	char *status;
	unsigned int i = 0;


	if (!chan)
	{
		command_fail(si, fault_needmoreparams, STR_INSUFFICIENT_PARAMS, "SUPPORT");
		command_fail(si, fault_needmoreparams, _("To verify Support for a Channel Registration: SUPPORT <#channel>"));
		return;
	}

	else
	{
		MOWGLI_ITER_FOREACH(n, cs_csrqlist.head)
		{
			l = n->data;

			if ((!l->chan) || !match(l->supp1, entity(si->smu)->name))
			{
				s1 = smalloc(sizeof(csrs1_t));
				s1->chan = sstrdup(chan);
				s1->supp1 = sstrdup(entity(si->smu)->name);
				s1->supp1rply = "YES";
				logcommand(si, CMDLOG_REGISTER, "REGISTER:SUPPORT \2%s\2 -- \2SUPPORTER1: %s\2 SUPPORT VERIFIED", chan, entity(si->smu)->name);
				command_success_nodata(si, _("SUPPORT for \2%s\2 has been recorded for \2%s\2."), l->chan, entity(si->smu)->name);
				n = mowgli_node_create();
				mowgli_node_add(s1, n, &cs_csrs1list);

				MOWGLI_ITER_FOREACH(n2, cs_csrs1list.head)
				{
					s1 = n2->data;

					if (s1->supp1 != NULL)
					{
						i++;
					}
				}

				MOWGLI_ITER_FOREACH(n3, cs_csrs2list.head)
				{
					s2 = n3->data;

					if (s2->supp2 != NULL)
					{
						i++;
					}
				}

				if (i == 2)
				{
					logcommand(si, CMDLOG_REGISTER, "REGISTER:READY FOR REVIEW \2%s\2 -- Supporters have been verified.", l->chan);
					if (groupmemo != NULL)
					send_group_memo(si, "[\2REGPROC\2 auto memo] Channel \2%s\2 is now \2Ready for Review\2!", l->chan);
				}

				i = 0;

				return;
			}
			else if ((!l->chan) || !match(l->supp2, entity(si->smu)->name))
			{
				s2 = smalloc(sizeof(csrs2_t));
				s2->chan = sstrdup(chan);
				s2->supp2 = sstrdup(entity(si->smu)->name);
				s2->supp2rply = "YES";
				logcommand(si, CMDLOG_REGISTER, "REGISTER:SUPPORT \2%s\2 -- \2SUPPORTER2: %s\2 SUPPORT VERIFIED", chan, entity(si->smu)->name);
				command_success_nodata(si, _("SUPPORT for \2%s\2 has been recorded for \2%s\2."), l->chan, entity(si->smu)->name);
				n = mowgli_node_create();
				mowgli_node_add(s2, n, &cs_csrs2list);

				MOWGLI_ITER_FOREACH(n2, cs_csrs1list.head)
				{
					s1 = n2->data;

					if (s1->supp1 != NULL)
					{
						i++;
					}
				}

				MOWGLI_ITER_FOREACH(n3, cs_csrs2list.head)
				{
					s2 = n3->data;

					if (s2->supp2 != NULL)
					{
						i++;
					}
				}

				if (i == 2)
				{
					logcommand(si, CMDLOG_REGISTER, "REGISTER:READY FOR REVIEW \2%s\2 -- Supporters have been verified.", l->chan);
					if (groupmemo != NULL)
					send_group_memo(si, "[\2REGPROC\2 auto memo] Channel \2%s\2 is now \2Ready for Review\2!", l->chan);
				}

				i = 0;

				return;
			}
			else
			{
				command_fail(si, fault_noprivs, _("Sorry, \2%s\2, I do not see you listed as a Supporter on \2%s\2."), entity(si->smu)->name, chan);
				return;

			}
		}
	}

}

static void cs_cmd_nosupport(sourceinfo_t *si, int parc, char *parv[])
{
	char *action = parv[0];
	myuser_t *mu;

	if (!action)
	{
		command_fail(si, fault_needmoreparams, STR_INSUFFICIENT_PARAMS, "NOSUPPORT");
		command_fail(si, fault_needmoreparams, _("Usage: NOSUPPORT <ON|OFF>"));
		return;
	}

	if (!strcasecmp(action, "ON"))
	{

		if (metadata_find(si->smu, "private:nosupport"))
		{
			command_fail(si, fault_badparams, _("You have already turned ON \2NOSUPPORT\2."));
			return;
		}

		metadata_add(si->smu, "private:nosupport", "ON");
		logcommand(si, CMDLOG_DO, "NOSUPPORT:ON: \2%s\2", entity(si->smu)->name);
		command_success_nodata(si, _("\2NOSUPPORT\2 is now \2ON\2 for \2%s\2."), entity(si->smu)->name);
	}
	if (!strcasecmp(action, "OFF"))
	{
		if (!metadata_find(si->smu, "private:nosupport"))
		{
			command_fail(si, fault_badparams, _("You do not have \2NOSUPPORT\2 set."));
			return;
		}

		metadata_delete(si->smu, "private:nosupport");
		logcommand(si, CMDLOG_ADMIN, "NOSUPPORT:OFF: \2%s\2", entity(si->smu)->name);
		command_success_nodata(si, _("\2NOSUPPORT\2 is now \2OFF\2, \2%s\2."), entity(si->smu)->name);
	}
}
