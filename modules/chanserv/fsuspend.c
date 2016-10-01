/*
 * Copyright (c) 2016 Xtheme Development Group (Xtheme.org)
 * Rights to this code are as documented in doc/LICENSE.
 *
 * This file contains code for the CService FSUSPEND functions
 * for Services Operators with the chan:admin privilege to 
 * suspend any user's channel access (including Founders)
 *
 */

#include "atheme.h"

static void cs_cmd_fsuspend(sourceinfo_t *si, int parc, char *parv[]);
static void cs_cmd_fsuspend_add(sourceinfo_t *si, int parc, char *parv[]);
static void cs_cmd_fsuspend_del(sourceinfo_t *si, int parc, char *parv[]);

DECLARE_MODULE_V1
(
	"chanserv/fsuspend", false, _modinit, _moddeinit,
	PACKAGE_STRING,
	VENDOR_STRING
);

command_t cs_fsuspend = { "FSUSPEND", N_("Manipulates a channel's SUSPEND list."),
                        PRIV_CHAN_ADMIN, 4, cs_cmd_fsuspend, { .path = "cservice/fsuspend" } };
command_t cs_fsuspend_add = { "ADD", N_("Adds a user to the channel SUSPEND list."),
                        PRIV_CHAN_ADMIN, 4, cs_cmd_fsuspend_add, { .path = "" } };
command_t cs_fsuspend_del = { "DEL", N_("Deletes a user from the channel SUSPEND list."),
                        PRIV_CHAN_ADMIN, 3, cs_cmd_fsuspend_del, { .path = "" } };

mowgli_patricia_t *cs_fsuspend_cmds;

void _modinit(module_t *m)
{
        service_named_bind_command("chanserv", &cs_fsuspend);

	cs_fsuspend_cmds = mowgli_patricia_create(strcasecanon);

	/* Add sub-commands */
	command_add(&cs_fsuspend_add, cs_fsuspend_cmds);
	command_add(&cs_fsuspend_del, cs_fsuspend_cmds);

}

void _moddeinit(module_unload_intent_t intent)
{
	service_named_unbind_command("chanserv", &cs_fsuspend);

	/* Delete sub-commands */
	command_delete(&cs_fsuspend_add, cs_fsuspend_cmds);
	command_delete(&cs_fsuspend_del, cs_fsuspend_cmds);

	mowgli_patricia_destroy(cs_fsuspend_cmds, NULL, NULL);
}

static void cs_cmd_fsuspend(sourceinfo_t *si, int parc, char *parv[])
{
	char *chan;
	char *cmd;
	command_t *c;

	if (parc < 2)
	{
		command_fail(si, fault_needmoreparams, STR_INSUFFICIENT_PARAMS, "FSUSPEND");
		command_fail(si, fault_needmoreparams, _("Syntax: FSUSPEND <#channel> <ADD|DEL> [parameters]"));
		return;
	}

	if (parv[0][0] == '#')
		chan = parv[0], cmd = parv[1];
	else if (parv[1][0] == '#')
		cmd = parv[0], chan = parv[1];
	else
	{
		command_fail(si, fault_badparams, STR_INVALID_PARAMS, "FSUSPEND");
		command_fail(si, fault_badparams, _("Syntax: FSUSPEND <#channel> <ADD|DEL> [parameters]"));
		return;
	}

	c = command_find(cs_fsuspend_cmds, cmd);
	if (c == NULL)
	{
		command_fail(si, fault_badparams, _("Invalid command. Use \2/%s%s help\2 for a command listing."), (ircd->uses_rcommand == false) ? "msg " : "", chansvs.me->disp);
		return;
	}

	parv[1] = chan;
	command_exec(si->service, si, c, parc - 1, parv + 1);
}

void cs_cmd_fsuspend_add(sourceinfo_t *si, int parc, char *parv[])
{
	myentity_t *mt;
	mychan_t *mc;
	user_t *tu;
	hook_channel_acl_req_t req;
	chanacs_t *ca, *ca2;
	char *chan = parv[0];
	long duration;
	char expiry[512];
	char *s;
	char *target;
	char *uname;
	char *token;
	char *treason, reason[BUFSIZE];

	target = parv[1];
	token = strtok(parv[2], " ");

	if (!target)
	{
		command_fail(si, fault_needmoreparams, STR_INSUFFICIENT_PARAMS, "SUSPEND");
		command_fail(si, fault_needmoreparams, _("Syntax: FSUSPEND <#channel> ADD <nickname> [!P|!T <minutes>] [reason]"));
		return;
	}

	mc = mychan_find(chan);
	if (!mc)
	{
		command_fail(si, fault_nosuch_target, _("Channel \2%s\2 is not registered."), chan);
		return;
	}
	
	if (metadata_find(mc, "private:frozen:freezer"))
	{
		command_fail(si, fault_noprivs, _("\2%s\2 is frozen."), chan);
		return;
	}

	if (metadata_find(mc, "private:close:closer"))
	{
		command_fail(si, fault_noprivs, _("\2%s\2 is closed."), chan);
		return;
	}

	/* A duration, reason or duration and reason. */
	if (token)
	{
		if (!strcasecmp(token, "!P")) /* A duration [permanent] */
		{
			duration = 0;
			treason = strtok(NULL, "");

			if (treason)
				mowgli_strlcpy(reason, treason, BUFSIZE);
			else
				reason[0] = 0;
		}
		else if (!strcasecmp(token, "!T")) /* A duration [temporary] */
		{
			s = strtok(NULL, " ");
			treason = strtok(NULL, "");

			if (treason)
				mowgli_strlcpy(reason, treason, BUFSIZE);
			else
				reason[0] = 0;

			if (s)
			{
				duration = (atol(s) * 60);
				while (isdigit((unsigned char)*s))
					s++;
				if (*s == 'h' || *s == 'H')
					duration *= 60;
				else if (*s == 'd' || *s == 'D')
					duration *= 1440;
				else if (*s == 'w' || *s == 'W')
					duration *= 10080;
				else if (*s == '\0')
					;
				else
					duration = 0;

				if (duration == 0)
				{
					command_fail(si, fault_badparams, _("Invalid duration given."));
					command_fail(si, fault_badparams, _("Syntax: FSUSPEND <#channel> ADD <nick> [!P|!T <minutes>] [reason]"));
					return;
				}
			}
			else
			{
				command_fail(si, fault_needmoreparams, STR_INSUFFICIENT_PARAMS, "SUSPEND ADD");
				command_fail(si, fault_needmoreparams, _("Syntax: FSUSPEND <#channel> ADD <nick> [!P|!T <minutes>] [reason]"));
				return;
			}
		}
		else
		{
			duration = chansvs.suspend_time;
			mowgli_strlcpy(reason, token, BUFSIZE);
			treason = strtok(NULL, "");

			if (treason)
			{
				mowgli_strlcat(reason, " ", BUFSIZE);
				mowgli_strlcat(reason, treason, BUFSIZE);
			}
		}
	}
	else
	{ /* No reason and no duration */
		duration = chansvs.suspend_time;
		reason[0] = 0;
	}

	mt = myentity_find_ext(target);
	if (!mt)
	{
			command_fail(si, fault_badparams, _("Please use a valid account name to suspend."));
			return;
	}
	else
	{

		ca = chanacs_find_literal(mc, mt, 0);

		if (ca == NULL)
		{
			command_fail(si, fault_nochange, _("\2%s\2 has no flags on \2%s\2"), mt->name, mc->name);
			return;
		}

		if ((ca = chanacs_find_literal(mc, mt, 0x0)))
		{
			if (ca->level & CA_SUSPENDED)
			{
				command_fail(si, fault_nochange, _("\2%s\2 is already on the SUSPEND list for \2%s\2"), mt->name, mc->name);
				return;
			}
		}

		/* new entry */
		ca2 = chanacs_open(mc, mt, NULL, true, entity(si->smu));
		if (chanacs_is_table_full(ca2))
		{
			command_fail(si, fault_toomany, _("Channel %s access list is full."), mc->name);
			chanacs_close(ca2);
			return;
		}

		req.ca = ca2;
		req.oldlevel = ca2->level;

		chanacs_modify_simple(ca2, CA_SUSPENDED, 0, si->smu);

		req.newlevel = ca2->level;

		if (reason[0])
			if ((metadata_find(ca2, "sreason") != NULL))
			{
				metadata_delete(ca2, "sreason");
			}
			metadata_add(ca2, "sreason", reason);

		if (duration > 0)
		{
			time_t expireson = ca2->tmodified+duration;

			snprintf(expiry, sizeof expiry, "%ld", expireson);

			if ((metadata_find(ca2, "expires") != NULL))
			{
				metadata_delete(ca2, "expires");
			}

			metadata_add(ca2, "expires", expiry);

			command_success_nodata(si, _("FORCED SUSPENSION on \2%s\2 was successfully added for \2%s\2 and will expire in %s."), mt->name, mc->name, timediff(duration));
			verbose(mc, "\2%s\2 added \2%s\2 to the SUSPEND list by FORCE, expires in %s.", get_source_name(si), mt->name, timediff(duration));
			logcommand(si, CMDLOG_SET, "FSUSPEND:ADD: \2%s\2 on \2%s\2, expires in %s", mt->name, mc->name, timediff(duration));

		}
		else
		{

			if ((metadata_find(ca2, "sreason") != NULL))
			{
				metadata_delete(ca2, "sreason");
			}
				metadata_add(ca2, "sreason", reason);
			
			if ((metadata_find(ca2, "expires") != NULL))
			{
				metadata_delete(ca2, "expires");
			}

			command_success_nodata(si, _("SUSPENSION on \2%s\2 was successfully added to the SUSPEND list for \2%s\2."), mt->name, mc->name);

			verbose(mc, "\2%s\2 added \2%s\2 to the SUSPEND list.", get_source_name(si), mt->name);
			logcommand(si, CMDLOG_SET, "SUSPEND:ADD: \2%s\2 on \2%s\2", mt->name, mc->name);
		}

		hook_call_channel_acl_change(&req);
		chanacs_close(ca2);
		return;
	}
}

void cs_cmd_fsuspend_del(sourceinfo_t *si, int parc, char *parv[])
{
	myentity_t *mt;
	mychan_t *mc;
	hook_channel_acl_req_t req;
	chanacs_t *ca;
	mowgli_node_t *n, *tn;
	char *chan = parv[0];
	char *uname = parv[1];

	if (!chan || !uname)
	{
		command_fail(si, fault_needmoreparams, STR_INSUFFICIENT_PARAMS, "SUSPEND");
		command_fail(si, fault_needmoreparams, _("Syntax: SUSPEND <#channel> DEL <nickname>"));
		return;
	}

	mc = mychan_find(chan);
	if (!mc)
	{
		command_fail(si, fault_nosuch_target, _("Channel \2%s\2 is not registered."), chan);
		return;
	}

	if (metadata_find(mc, "private:frozen:freezer"))
	{
		command_fail(si, fault_noprivs, _("\2%s\2 is frozen."), chan);
		return;
	}
	
	if (metadata_find(mc, "private:close:closer"))
	{
		command_fail(si, fault_noprivs, _("\2%s\2 is closed."), chan);
		return;
	}

	mt = myentity_find_ext(uname);
	if (!mt)
	{
		/* we might be deleting a hostmask */
		ca = chanacs_find_host_literal(mc, uname, CA_SUSPENDED);
		if (ca == NULL)
		{
			ca = chanacs_find_host(mc, uname, CA_SUSPENDED);
			if (ca != NULL)
				command_fail(si, fault_nosuch_key, _("\2%s\2 is not on the SUSPEND list for \2%s\2, however \2%s\2 is."), uname, mc->name, ca->host);
			else
				command_fail(si, fault_nosuch_key, _("\2%s\2 is not on the SUSPEND list for \2%s\2."), uname, mc->name);
			return;
		}

		req.ca = ca;
		req.oldlevel = ca->level;

		chanacs_modify_simple(ca, 0, CA_SUSPENDED, si->smu);

		metadata_delete(ca, "sreason");
		metadata_delete(ca, "expires");

		req.newlevel = ca->level;

		hook_call_channel_acl_change(&req);
		chanacs_close(ca);

		verbose(mc, "\2%s\2 removed \2%s\2 from the SUSPEND list by FORCE.", get_source_name(si), uname);
		logcommand(si, CMDLOG_SET, "FSUSPEND:DEL: \2%s\2 on \2%s\2", uname, mc->name);
		command_success_nodata(si, _("\2%s\2 has been removed from the SUSPEND list for \2%s\2 by FORCE."), uname, mc->name);

		return;
	}

	if (!(ca = chanacs_find_literal(mc, mt, CA_SUSPENDED)))
	{
		command_fail(si, fault_nosuch_key, _("\2%s\2 is not on the SUSPEND list for \2%s\2."), mt->name, mc->name);
		return;
	}

	req.ca = ca;
	req.oldlevel = ca->level;

	chanacs_modify_simple(ca, 0, CA_SUSPENDED, NULL);

	metadata_delete(ca, "sreason");
	metadata_delete(ca, "expires");

	req.newlevel = ca->level;

	hook_call_channel_acl_change(&req);
	chanacs_close(ca);

	command_success_nodata(si, _("\2%s\2 has been removed from the SUSPEND list for \2%s\2 by FORCE."), mt->name, mc->name);
	logcommand(si, CMDLOG_SET, "FSUSPEND:DEL: \2%s\2 on \2%s\2", mt->name, mc->name);
	verbose(mc, "\2%s\2 removed \2%s\2 from the SUSPEND list by FORCE.", get_source_name(si), mt->name);

	return;
}


/* vim:cinoptions=>s,e0,n0,f0,{0,}0,^0,=s,ps,t0,c3,+s,(2s,us,)20,*30,gs,hs
 * vim:ts=8
 * vim:sw=8
 * vim:noexpandtab
 */
