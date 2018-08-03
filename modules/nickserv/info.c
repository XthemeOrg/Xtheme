/*
 * Copyright (c) 2014-2018 Xtheme Development Group
 * Copyright (c) 2005-2006 William Pitcock, et al.
 * Rights to this code are as documented in doc/LICENSE.
 *
 * This file contains code for the NickServ INFO functions.
 *
 */

#include "atheme.h"

DECLARE_MODULE_V1
(
	"nickserv/info", false, _modinit, _moddeinit,
	PACKAGE_STRING,
	VENDOR_STRING
);

static void ns_cmd_info(sourceinfo_t *si, int parc, char *parv[]);

command_t ns_info = { "INFO", N_("Displays information on registrations."), AC_NONE, 2, ns_cmd_info, { .path = "nickserv/info" } };

void _modinit(module_t *m)
{
	service_named_bind_command("nickserv", &ns_info);
}

void _moddeinit(module_unload_intent_t intent)
{
	service_named_unbind_command("nickserv", &ns_info);
}

static void ns_cmd_info(sourceinfo_t *si, int parc, char *parv[])
{
	myuser_t *mu;
	mynick_t *mn = NULL;
	myuser_name_t *mun;
	user_t *u = NULL;
	bool recognized = false;
	const char *name = parv[0];
	char buf[BUFSIZE], strfbuf[BUFSIZE], lastlogin[BUFSIZE], *p;
	size_t buflen;
	time_t registered;
	struct tm tm, tm2;
	metadata_t *md;
	mowgli_node_t *n;
	mowgli_patricia_iteration_state_t state;
	const char *vhost;
	const char *vhost_timestring;
	const char *vhost_assigner;
	time_t vhost_time;
	bool has_user_auspex;
	bool hide_info;
	hook_user_req_t req;
	hook_info_noexist_req_t noexist_req;

	/* On IRC, default the name to something.
	 * Not currently documented.
	 */
	if (!name && si->su)
	{
		if (!nicksvs.no_nick_ownership)
			name = si->su->nick;
		else if (si->smu)
			name = entity(si->smu)->name;
	}

	if (!name)
	{
		command_fail(si, fault_needmoreparams, STR_INSUFFICIENT_PARAMS, "INFO");
		command_fail(si, fault_needmoreparams, _("Syntax: INFO <nickname>"));
		return;
	}

	noexist_req.si = si;
	noexist_req.nick = name;

	has_user_auspex = has_priv(si, PRIV_USER_AUSPEX);

	if (!(mu = myuser_find_ext(name)))
	{
		if (has_user_auspex && (mun = myuser_name_find(name)) != NULL)
		{
			const char *setter;
			const char *reason;
			time_t ts;

			md = metadata_find(mun, "private:mark:setter");
			setter = md != NULL ? md->value : "unknown";
			md = metadata_find(mun, "private:mark:reason");
			reason = md != NULL ? md->value : "unknown";
			md = metadata_find(mun, "private:mark:timestamp");
			ts = md != NULL ? atoi(md->value) : 0;

			tm = *localtime(&ts);
			strftime(strfbuf, sizeof strfbuf, TIME_FORMAT, &tm);

			command_fail(si, fault_nosuch_target, _("\2%s\2 is not registered anymore, but was marked by %s on %s (%s)."), mun->name, setter, strfbuf, reason);
			hook_call_user_info_noexist(&noexist_req);
		}
		else {
			hook_call_user_info_noexist(&noexist_req);
			command_fail(si, fault_nosuch_target, _("\2%s\2 is not registered."), name);
		}
		return;
	}

	hide_info = use_account_private && mu->flags & MU_PRIVATE &&
		mu != si->smu && !has_user_auspex;

	if (!nicksvs.no_nick_ownership)
	{
		mn = mynick_find(*name == '=' ? name + 1 : name);
		if (mn != NULL && mn->owner != mu)
			mn = NULL;
		if (mn == NULL)
			mn = mynick_find(entity(mu)->name);
		u = user_find_named(mn->nick);
		if (u != NULL && u->myuser != mu)
		{
			if (myuser_access_verify(u, mu))
				recognized = true;
			u = NULL;
		}
	}

	if (mn != NULL)
		command_success_nodata(si, _("Information on \2%s\2 (account \2%s\2):"), mn->nick, entity(mu)->name);
	else
		command_success_nodata(si, _("Information on \2%s\2:"), entity(mu)->name);

	registered = mn != NULL ? mn->registered : mu->registered;
	tm = *localtime(&registered);
	strftime(strfbuf, sizeof strfbuf, TIME_FORMAT, &tm);
	command_success_nodata(si, _("Registered : %s (%s ago)"), strfbuf, time_ago(registered));
	/* show account's time if it's different from nick's time */
	if (mu->registered != registered)
	{
		tm = *localtime(&mu->registered);
		strftime(strfbuf, sizeof strfbuf, TIME_FORMAT, &tm);
		command_success_nodata(si, _("User reg.  : %s (%s ago)"), strfbuf, time_ago(mu->registered));
	}

	if (config_options.show_entity_id || has_user_auspex)
		command_success_nodata(si, _("Entity ID  : %s"), entity(mu)->id);

	md = metadata_find(mu, "private:usercloak");
	vhost = md ? md->value : NULL;

	md = metadata_find(mu, "private:usercloak-timestamp");
	vhost_timestring = md ? md->value : NULL;

	md = metadata_find(mu, "private:usercloak-assigner");
	vhost_assigner = md ? md->value : NULL;

	if (!hide_info && (md = metadata_find(mu, "private:host:vhost")))
	{
		mowgli_strlcpy(buf, md->value, sizeof buf);
		if (vhost != NULL)
		{
			p = strchr(buf, '@');
			if (p == NULL)
				p = buf;
			else
				p++;
			mowgli_strlcpy(p, vhost, sizeof buf - (p - buf));
		}
		command_success_nodata(si, _("Last addr  : %s"), buf);
	}

	if ((vhost || vhost_timestring || vhost_assigner) && (si->smu == mu || has_user_auspex))
	{
		buf[0] = '\0';
		buflen = 0;

		if (vhost_timestring && (vhost || has_user_auspex))
		{
			vhost_time = atoi(vhost_timestring);
			tm2 = *localtime(&vhost_time);
			strftime(strfbuf, sizeof strfbuf, TIME_FORMAT, &tm2);
			buflen += snprintf(buf + buflen, BUFSIZE - buflen, _(" on %s (%s ago)"), strfbuf, time_ago(vhost_time));
		}

		if (has_user_auspex && vhost_assigner)
			buflen += snprintf(buf + buflen, BUFSIZE - buflen, _(" by %s"), vhost_assigner);

		if (vhost && !buf[0])
			command_success_nodata(si, _("vHost      : %s"), vhost);
		else if (vhost && buf[0])
			command_success_nodata(si, _("vHost      : %s (assigned%s)"), vhost, buf);
		else if (!vhost && buf[0])
			command_success_nodata(si, _("vHost      : unassigned%s"), buf);
	}

	if (has_user_auspex)
	{
		if ((md = metadata_find(mu, "private:host:actual")))
			command_success_nodata(si, _("Real addr  : %s"), md->value);
	}

	if (recognized)
		command_success_nodata(si, _("Recognized : now (matches access list)"));
	/* show nick's lastseen/online, if we have a nick */
	if (u != NULL)
		command_success_nodata(si, _("Last seen  : now"));
	else if (mn != NULL)
	{
		tm2 = *localtime(&mn->lastseen);
		strftime(lastlogin, sizeof lastlogin, TIME_FORMAT, &tm2);
		if (hide_info)
			command_success_nodata(si, _("Last seen  : (about %d weeks ago)"), (int)((CURRTIME - mn->lastseen) / 604800));
		else
			command_success_nodata(si, _("Last seen  : %s (%s ago)"), lastlogin, time_ago(mn->lastseen));
	}

	/* if noone is logged in to this account, show account's lastseen,
	 * unless we have a nick and it quit at the same time as the account
	 */
	if (MOWGLI_LIST_LENGTH(&mu->logins) == 0)
	{
		tm2 = *localtime(&mu->lastlogin);
		strftime(lastlogin, sizeof lastlogin, TIME_FORMAT, &tm2);
		if (mn == NULL)
		{
			if (hide_info)
				command_success_nodata(si, _("Last seen  : (about %d weeks ago)"), (int)((CURRTIME - mu->lastlogin) / 604800));
			else
				command_success_nodata(si, _("Last seen  : %s (%s ago)"), lastlogin, time_ago(mu->lastlogin));
		}
		else if (mn->lastseen != mu->lastlogin)
		{
			if (hide_info)
				command_success_nodata(si, _("User seen  : (about %d weeks ago)"), (int)((CURRTIME - mu->lastlogin) / 604800));
			else
				command_success_nodata(si, _("User seen  : %s (%s ago)"), lastlogin, time_ago(mu->lastlogin));
		}
	}
	/* someone is logged in to this account
	 * if they're privileged, show them the sessions
	 */
	else if (mu == si->smu || has_user_auspex)
	{
		buf[0] = '\0';
		MOWGLI_ITER_FOREACH(n, mu->logins.head)
		{
			if (strlen(buf) > 80)
			{
				command_success_nodata(si, _("Logins from: %s"), buf);
				buf[0] = '\0';
			}
			if (buf[0])
				mowgli_strlcat(buf, " ", sizeof buf);
			mowgli_strlcat(buf, ((user_t *)(n->data))->nick, sizeof buf);
		}
		if (buf[0])
			command_success_nodata(si, _("Logins from: %s"), buf);
	}
	/* tell them this account is online, but not which nick
	 * unless we have already told them above
	 */
	else if (u == NULL)
	{
		if (mn != NULL)
			command_success_nodata(si, _("User seen  : now"));
		else
			command_success_nodata(si, _("Last seen  : now"));
	}

	if (!nicksvs.no_nick_ownership)
	{
		/* list registered nicks if privileged */
		if (mu == si->smu || has_user_auspex)
		{
			buf[0] = '\0';
			MOWGLI_ITER_FOREACH(n, mu->nicks.head)
			{
				if (strlen(buf) > 80)
				{
					command_success_nodata(si, _("Nicks      : %s"), buf);
					buf[0] = '\0';
				}
				if (buf[0])
					mowgli_strlcat(buf, " ", sizeof buf);
				mowgli_strlcat(buf, ((mynick_t *)(n->data))->nick, sizeof buf);
			}
			if (buf[0])
				command_success_nodata(si, _("Nicks      : %s"), buf);
		}
	}

	if (!(mu->flags & MU_HIDEMAIL)
		|| (si->smu == mu || has_user_auspex))
		command_success_nodata(si, _("Email      : %s%s"), mu->email,
					(mu->flags & MU_HIDEMAIL) ? " (hidden)": "");

	MOWGLI_PATRICIA_FOREACH(md, &state, object(mu)->metadata)
	{
		if (!strncmp(md->name, "private:", 8))
			continue;
		command_success_nodata(si, _("Metadata   : %s = %s"),
				md->name, md->value);
	}

	*buf = '\0';

	if (MU_HIDEMAIL & mu->flags)
		strcat(buf, "HideMail");

	if (MU_HOLD & mu->flags)
	{
		if (*buf)
			strcat(buf, ", ");

		strcat(buf, "Hold");
	}
	if (MU_NEVEROP & mu->flags)
	{
		if (*buf)
			strcat(buf, ", ");

		strcat(buf, "NeverOp");
	}
	if (MU_NOOP & mu->flags)
	{
		if (*buf)
			strcat(buf, ", ");

		strcat(buf, "NoOp");
	}
	if (MU_NOMEMO & mu->flags)
	{
		if (*buf)
			strcat(buf, ", ");

		strcat(buf, "NoMemo");
	}
	if (MU_EMAILMEMOS & mu->flags)
	{
		if (*buf)
			strcat(buf, ", ");

		strcat(buf, "EMailMemos");
	}
	if (MU_NEVERGROUP & mu->flags)
	{
		if (*buf)
			strcat(buf, ", ");

		strcat(buf, "NeverGroup");
	}
	if (use_account_private && MU_PRIVATE & mu->flags)
	{
		if (*buf)
			strcat(buf, ", ");

		strcat(buf, "Private");
	}
	if (MU_REGNOLIMIT & mu->flags)
	{
		if (*buf)
			strcat(buf, ", ");

		strcat(buf, "RegNoLimit");
	}
    if (MU_NOGREET & mu->flags)
    {
        if (*buf)
            strcat(buf, ", ");

        strcat(buf, "NoGreet");
    }
	if (MU_NOPASSWORD & mu->flags)
	{
		if (*buf)
			strcat(buf, ", ");

		strcat(buf, "NoPassword");
	}

	if (*buf)
		command_success_nodata(si, _("Flags      : %s"), buf);

#ifdef ENABLE_NLS
	if (mu == si->smu || has_user_auspex)
		command_success_nodata(si, _("Language   : %s"),
				language_get_name(mu->language));
#endif

	if (mu->soper && (mu == si->smu || has_priv(si, PRIV_VIEWPRIVS)))
	{
		command_success_nodata(si, _("Oper class : %s"), mu->soper->operclass ? mu->soper->operclass->name : mu->soper->classname);
	}

	if (mu == si->smu || has_user_auspex || has_priv(si, PRIV_CHAN_AUSPEX))
	{
		chanacs_t *ca;
		int founder = 0, other = 0;

		MOWGLI_ITER_FOREACH(n, entity(mu)->chanacs.head)
		{
			ca = n->data;
			if (ca->level & CA_FOUNDER)
				founder++;
			else if (ca->level != CA_SUSPENDED)
				other++;
			else if (ca->level != CA_AKICK)
				other++;
		}
		command_success_nodata(si, _("Channels   : %d founder, %d other"), founder, other);
	}

	if (has_user_auspex && (md = metadata_find(mu, "private:freeze:freezer")))
	{
		const char *setter = md->value;
		const char *reason;
		time_t ts;

		md = metadata_find(mu, "private:freeze:reason");
		reason = md != NULL ? md->value : "unknown";

		md = metadata_find(mu, "private:freeze:timestamp");
		ts = md != NULL ? atoi(md->value) : 0;

		tm = *localtime(&ts);
		strftime(strfbuf, sizeof strfbuf, TIME_FORMAT, &tm);

		command_success_nodata(si, _("%s was \2FROZEN\2 by %s on %s (%s)"), entity(mu)->name, setter, strfbuf, reason);
	}
	else if (metadata_find(mu, "private:freeze:freezer"))
		command_success_nodata(si, _("%s has been frozen by the %s administration."), entity(mu)->name, me.netname);

	if (has_user_auspex && (md = metadata_find(mu, "private:forbid:forbidder")))
	{
		const char *setter = md->value;
		const char *reason;
		time_t ts;

		md = metadata_find(mu, "private:forbid:reason");
		reason = md != NULL ? md->value : "unknown";

		md = metadata_find(mu, "private:forbid:timestamp");
		ts = md != NULL ? atoi(md->value) : 0;

		tm = *localtime(&ts);
		strftime(strfbuf, sizeof strfbuf, TIME_FORMAT, &tm);

		command_success_nodata(si, _("%s was \2FORBIDDEN\2 by %s on %s (%s)"), entity(mu)->name, setter, strfbuf, reason);
	}
	else if (metadata_find(mu, "private:forbid:forbidder"))
		command_success_nodata(si, _("%s has been forbidden by the %s administration."), entity(mu)->name, me.netname);

	if (has_user_auspex && (md = metadata_find(mu, "private:mark:setter")))
	{
		const char *setter = md->value;
		const char *reason;
		time_t ts;

		md = metadata_find(mu, "private:mark:reason");
		reason = md != NULL ? md->value : "unknown";

		md = metadata_find(mu, "private:mark:timestamp");
		ts = md != NULL ? atoi(md->value) : 0;

		tm = *localtime(&ts);
		strftime(strfbuf, sizeof strfbuf, TIME_FORMAT, &tm);

		command_success_nodata(si, _("%s was \2MARKED\2 by %s on %s (%s)"), entity(mu)->name, setter, strfbuf, reason);
	}

	if (has_user_auspex && (md = metadata_find(mu, "private:held:holder")))
	{
		const char *setter = md->value;
		time_t ts;

		md = metadata_find(mu, "private:held:timestamp");
		ts = md != NULL ? atoi(md->value) : 0;

		tm = *localtime(&ts);
		strftime(strfbuf, sizeof strfbuf, TIME_FORMAT, &tm);

		command_success_nodata(si, _("%s was \2HELD\2 by %s on %s"), entity(mu)->name, setter, strfbuf);
	}
	else if (metadata_find(mu, "private:held:holder"))
		command_success_nodata(si, _("%s has been HELD by the %s administration."), entity(mu)->name, me.netname);

	if (metadata_find(mu, "private:blocksendpass:on"))
		command_success_nodata(si, _("%s has blocked SENDPASS, except by the %s administration."), entity(mu)->name, me.netname);

	if (MU_WAITAUTH & mu->flags)
		command_success_nodata(si, _("%s has \2NOT COMPLETED\2 registration verification"), entity(mu)->name);

	if ((mu == si->smu || has_user_auspex) &&
			(md = metadata_find(mu, "private:verify:emailchg:newemail")))
	{
		const char *newemail = md->value;
		time_t ts;

		md = metadata_find(mu, "private:verify:emailchg:timestamp");
		ts = md != NULL ? atoi(md->value) : 0;
		tm = *localtime(&ts);
		strftime(strfbuf, sizeof strfbuf, TIME_FORMAT, &tm);

		command_success_nodata(si, _("%s has requested an email address change to %s on %s"), entity(mu)->name, newemail, strfbuf);
	}

	req.si = si;
	req.mu = mu;
	req.mn = mn;
	hook_call_user_info(&req);

	command_success_nodata(si, _("*** \2End of Info\2 ***"));

	if (mn == NULL)
		logcommand(si, CMDLOG_GET, "INFO: \2%s\2", entity(mu)->name);
	else
		logcommand(si, CMDLOG_GET, "INFO: \2%s\2 (%s)", mn->nick, entity(mu)->name);
}

/* vim:cinoptions=>s,e0,n0,f0,{0,}0,^0,=s,ps,t0,c3,+s,(2s,us,)20,*30,gs,hs
 * vim:ts=8
 * vim:sw=8
 * vim:noexpandtab
 */
