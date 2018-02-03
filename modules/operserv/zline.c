/*
 * Copyright (c) 2014-2018 Xtheme Development Group
 * Rights to this code are documented in doc/LICENSE.
 *
 * This file contains functionality which implements
 * the OperServ ZLINE (ADD, DEL, LIST, SYNC) commands.
 *
 */

#include "atheme.h"

DECLARE_MODULE_V1
(
	"operserv/zline", false, _modinit, _moddeinit,
	PACKAGE_STRING,
	VENDOR_STRING
);

static void os_zline_newuser(hook_user_nick_t *data);

static void os_cmd_zline(sourceinfo_t *si, int parc, char *parv[]);
static void os_cmd_zline_add(sourceinfo_t *si, int parc, char *parv[]);
static void os_cmd_zline_del(sourceinfo_t *si, int parc, char *parv[]);
static void os_cmd_zline_list(sourceinfo_t *si, int parc, char *parv[]);
static void os_cmd_zline_sync(sourceinfo_t *si, int parc, char *parv[]);


command_t os_zline = { "ZLINE", N_("Manages network IP bans."), PRIV_AKILL, 3, os_cmd_zline, { .path = "oservice/zline" } };

command_t os_zline_add = { "ADD", N_("Adds a network zline or IP ban"), AC_NONE, 2, os_cmd_zline_add, { .path = "" } };
command_t os_zline_del = { "DEL", N_("Deletes a network zline or IP ban"), AC_NONE, 1, os_cmd_zline_del, { .path = "" } };
command_t os_zline_list = { "LIST", N_("Lists all network zlines or IP bans"), AC_NONE, 1, os_cmd_zline_list, { .path = "" } };
command_t os_zline_sync = { "SYNC", N_("Synchronises zlines or IP bans to servers"), AC_NONE, 0, os_cmd_zline_sync, { .path = "" } };

mowgli_patricia_t *os_zline_cmds;

void _modinit(module_t *m)
{
        service_named_bind_command("operserv", &os_zline);

	os_zline_cmds = mowgli_patricia_create(strcasecanon);

	/* Add sub-commands */
	command_add(&os_zline_add, os_zline_cmds);
	command_add(&os_zline_del, os_zline_cmds);
	command_add(&os_zline_list, os_zline_cmds);
	command_add(&os_zline_sync, os_zline_cmds);

	hook_add_event("user_add");
	hook_add_user_add(os_zline_newuser);
}

void _moddeinit(module_unload_intent_t intent)
{
	service_named_unbind_command("operserv", &os_zline);

	/* Delete sub-commands */
	command_delete(&os_zline_add, os_zline_cmds);
	command_delete(&os_zline_del, os_zline_cmds);
	command_delete(&os_zline_list, os_zline_cmds);
	command_delete(&os_zline_sync, os_zline_cmds);

	hook_del_user_add(os_zline_newuser);

	mowgli_patricia_destroy(os_zline_cmds, NULL, NULL);
}

static void os_zline_newuser(hook_user_nick_t *data)
{
	user_t *u = data->u;
	zline_t *z;

	/* If the user has been killed, don't do anything. */
	if (!u)
		return;

	if (is_internal_client(u))
		return;
	z = zline_find_user(u);
	if (z != NULL)
	{
		/* Server didn't have that zline, send it again.
		 * To ensure zline exempt works on zlines too, do
		 * not send a KILL. -- jilles */
		char reason[BUFSIZE];
		snprintf(reason, sizeof(reason), "[Z#%lu] %s", z->number, z->reason);
		dline_sts("*", z->host, z->duration ? z->expires - CURRTIME : 0, reason);

	}
}

static void os_cmd_zline(sourceinfo_t *si, int parc, char *parv[])
{
	/* Grab args */
	char *cmd = parv[0];
        command_t *c;

	/* Bad/missing arg */
	if (!cmd)
	{
		command_fail(si, fault_needmoreparams, STR_INSUFFICIENT_PARAMS, "ZLINE");
		command_fail(si, fault_needmoreparams, _("Syntax: ZLINE ADD|DEL|LIST"));
		return;
	}

	c = command_find(os_zline_cmds, cmd);
	if (c == NULL)
	{
		command_fail(si, fault_badparams, _("Invalid command. Use \2/%s%s help\2 for a command listing."), (ircd->uses_rcommand == false) ? "msg " : "", si->service->disp);
		return;
	}

	command_exec(si->service, si, c, parc - 1, parv + 1);
}

static void os_cmd_zline_add(sourceinfo_t *si, int parc, char *parv[])
{
	char *target = parv[0];
	char *token = strtok(parv[1], " ");
	char *treason, reason[BUFSIZE];
	long duration;
	char *s;
	zline_t *z;

	if (!target || !token)
	{
		command_fail(si, fault_needmoreparams, STR_INSUFFICIENT_PARAMS, "ZLINE ADD");
		command_fail(si, fault_needmoreparams, _("Syntax: ZLINE ADD <IP> [!P|!T <minutes>] <reason>"));
		return;
	}

	if (!strcasecmp(token, "!P"))
	{
		duration = 0;
		treason = strtok(NULL, "");

		if (treason)
			mowgli_strlcpy(reason, treason, BUFSIZE);
		else
			mowgli_strlcpy(reason, "No reason given", BUFSIZE);
	}
	else if (!strcasecmp(token, "!T"))
	{
		s = strtok(NULL, " ");
		treason = strtok(NULL, "");
		if (treason)
			mowgli_strlcpy(reason, treason, BUFSIZE);
		else
			mowgli_strlcpy(reason, "No reason given", BUFSIZE);
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
				command_fail(si, fault_badparams, _("Syntax: ZLINE ADD <IP> [!P|!T <minutes>] <reason>"));
				return;
			}
		}
		else {
			command_fail(si, fault_needmoreparams, STR_INSUFFICIENT_PARAMS, "ZLINE ADD");
			command_fail(si, fault_needmoreparams, _("Syntax: ZLINE ADD <IP> [!P|!T <minutes>] <reason>"));
			return;
		}

	}
	else
	{
		duration = config_options.zline_time;
		mowgli_strlcpy(reason, token, BUFSIZE);
		treason = strtok(NULL, "");

		if (treason)
		{
			mowgli_strlcat(reason, " ", BUFSIZE);
			mowgli_strlcat(reason, treason, BUFSIZE);
		}
	}

	char *p;
	int i = 0;

	if (zline_find(target))
	{
		command_fail(si, fault_nochange, _("ZLINE \2%s\2 is already matched in the database."), target);
		return;
	}

	z = zline_add(target, reason, duration, get_storage_oper_name(si));

	if (duration)
		command_success_nodata(si, _("Timed ZLINE on \2%s\2 was successfully added and will expire in %s."), z->host, timediff(duration));
	else
		command_success_nodata(si, _("ZLINE on \2%s\2 was successfully added."), z->host);

	verbose_wallops("\2%s\2 is \2adding\2 an \2SQLINE\2 for \2%s\2 -- reason: \2%s\2", get_oper_name(si), z->host,
		z->reason);
	logcommand(si, CMDLOG_ADMIN, "ZLINE:ADD: \2%s\2 (reason: \2%s\2)", z->host, z->reason);
}

static void os_cmd_zline_del(sourceinfo_t *si, int parc, char *parv[])
{
	char *target = parv[0];
	char *hostbuf;
	unsigned int number;
	char *s;
	zline_t *z;

	if (!target)
	{
		command_fail(si, fault_needmoreparams, STR_INSUFFICIENT_PARAMS, "ZLINE DEL");
		command_fail(si, fault_needmoreparams, _("Syntax: ZLINE DEL <IP|number>"));
		return;
	}

	if (strchr(target, ','))
	{
		unsigned int start = 0, end = 0, i;
		char t[16];

		s = strtok(target, ",");

		do
		{
			if (strchr(s, ':'))
			{
				for (i = 0; *s != ':'; s++, i++)
					t[i] = *s;

				t[++i] = '\0';
				start = atoi(t);

				s++;	/* skip past the : */

				for (i = 0; *s != '\0'; s++, i++)
					t[i] = *s;

				t[++i] = '\0';
				end = atoi(t);

				for (i = start; i <= end; i++)
				{
					if (!(z = zline_find_num(i)))
					{
						command_fail(si, fault_nosuch_target, _("No such ZLINE with number \2%d\2."), i);
						continue;
					}

					command_success_nodata(si, _("ZLINE on \2%s\2 has been successfully removed."), z->host);
					verbose_wallops("\2%s\2 is \2removing\2 a \2ZLINE\2 for \2%s\2 -- reason: \2%s\2",
						get_oper_name(si), z->host, z->reason);

					logcommand(si, CMDLOG_ADMIN, "ZLINE:DEL: \2%s\2", z->host);
					zline_delete(z);
				}

				continue;
			}

			number = atoi(s);

			if (!(z = zline_find_num(number)))
			{
				command_fail(si, fault_nosuch_target, _("No such ZLINE with number \2%d\2."), number);
				return;
			}

			command_success_nodata(si, _("ZLINE on \2%s\2 has been successfully removed."), z->host);
			verbose_wallops("\2%s\2 is \2removing\2 a \2ZLINE\2 for \2%s\2 -- reason: \2%s\2",
				get_oper_name(si), z->host, z->reason);

			logcommand(si, CMDLOG_ADMIN, "ZLINE:DEL: \2%s\2", z->host);
			zline_delete(z);
		} while ((s = strtok(NULL, ",")));

		return;
	}

	if (target != NULL)
	{

		if (!(z = zline_find(target)))
		{
			command_fail(si, fault_nosuch_target, _("No such ZLINE found in the database: \2%s\2."), target);
			return;
		}
		else

		command_success_nodata(si, _("ZLINE on \2%s\2 has been successfully removed."), z->host);

		verbose_wallops("\2%s\2 is \2removing\2 a \2ZLINE\2 for \2%s\2 -- reason: \2%s\2",
			get_oper_name(si), z->host, z->reason);

		logcommand(si, CMDLOG_ADMIN, "ZLINE:DEL: \2%s\2 (reason: \2%s\2)", z->host, z->reason);
		zline_delete(z);
		return;
	}

	hostbuf = strtok(NULL, "");

	if (!(z = zline_find(hostbuf)))
	{
		command_fail(si, fault_nosuch_target, _("No such ZLINE: \2%s\2."), hostbuf);
		return;
	}

	command_success_nodata(si, _("ZLINE on \2%s\2 has been successfully removed."), hostbuf);

	verbose_wallops("\2%s\2 is \2removing\2 a \2ZLINE\2 for \2%s\2 -- reason: \2%s\2",
		get_oper_name(si), z->host, z->reason);

	logcommand(si, CMDLOG_ADMIN, "ZLINE:DEL: \2%s\2 (reason: \2%s\2)", z->host, z->reason);
	zline_delete(z);
}

static void os_cmd_zline_list(sourceinfo_t *si, int parc, char *parv[])
{
	char *param = parv[0];
	char *host = NULL;
	unsigned long num = 0;
	bool full = false;
	mowgli_node_t *n;
	zline_t *z;

	if (param != NULL)
	{
		if (!strcasecmp(param, "FULL"))
			full = true;
		else if ((host = strchr(param, '@')) != NULL)
		{
			*host++ = '\0';
			full = true;
		}
		else if (strchr(param, '.') || strchr(param, ':'))
		{
			host = param;
			full = true;
		}
		else if (isdigit((unsigned char)param[0]) &&
				(num = strtoul(param, NULL, 10)) != 0)
			full = true;
		else
		{
			command_fail(si, fault_badparams, STR_INVALID_PARAMS, "ZLINE LIST");
			return;
		}
	}

	if (host || num)
		command_success_nodata(si, _("ZLINE list matching given criteria (with reasons):"));
	else if (full)
		command_success_nodata(si, _("ZLINE list (with reasons):"));
	else
		command_success_nodata(si, _("ZLINE list:"));

	MOWGLI_ITER_FOREACH(n, zlnlist.head)
	{
		struct tm tm;
		char settime[64];

		z = (zline_t *)n->data;

		tm = *localtime(&z->settime);
		strftime(settime, sizeof settime, TIME_FORMAT, &tm);

		if (num != 0 && z->number != num)
			continue;
		if (host != NULL && match(z->host, host) && match_ips(z->host, host))
			continue;

		if (z->duration && full)
			command_success_nodata(si, _("Z%lu: %s - by \2%s\2 on %s - expires in \2%s\2 - (%s)"), z->number, z->host, z->setby, settime, timediff(z->expires > CURRTIME ? z->expires - CURRTIME : 0), z->reason);
		else if (z->duration && !full)
			command_success_nodata(si, _("Z%lu: %s - by \2%s\2 on %s - expires in \2%s\2"), z->number, z->host, z->setby, settime, timediff(z->expires > CURRTIME ? z->expires - CURRTIME : 0));
		else if (!z->duration && full)
			command_success_nodata(si, _("Z%lu: %s - by \2%s\2 on %s - \2permanent\2 - (%s)"), z->number, z->host, z->setby, settime, z->reason);
		else
			command_success_nodata(si, _("Z%lu: %s - by \2%s\2 on %s - \2permanent\2"), z->number, z->host, z->setby, settime);
	}

	if (host || num)
		command_success_nodata(si, _("End of ZLINE list."));
	else
		command_success_nodata(si, _("Total of \2%zu\2 %s in ZLINE list."), zlnlist.count, (zlnlist.count == 1) ? "entry" : "entries");
	if (host)
		logcommand(si, CMDLOG_GET, "ZLINE:LIST: \2%s\2", host ? host : "*");
	else if (num)
		logcommand(si, CMDLOG_GET, "ZLINE:LIST: \2%lu\2", num);
	else
		logcommand(si, CMDLOG_GET, "ZLINE:LIST: \2%s\2", full ? " FULL" : "");
}

static void os_cmd_zline_sync(sourceinfo_t *si, int parc, char *parv[])
{
	mowgli_node_t *n;
	zline_t *z;

	logcommand(si, CMDLOG_DO, "ZLINE:SYNC");

	MOWGLI_ITER_FOREACH(n, zlnlist.head)
	{
		z = (zline_t *)n->data;


		char reason[BUFSIZE];
		snprintf(reason, sizeof(reason), "[Z#%lu] %s", z->number, z->reason);

		if (z->duration == 0)
			dline_sts("*", z->host, 0, reason);
		else if (z->expires > CURRTIME)
			dline_sts("*", z->host, z->expires - CURRTIME, reason);
	}

	command_success_nodata(si, _("ZLINE list synchronized to servers."));
}

/* vim:cinoptions=>s,e0,n0,f0,{0,}0,^0,=s,ps,t0,c3,+s,(2s,us,)20,*30,gs,hs
 * vim:ts=8
 * vim:sw=8
 * vim:noexpandtab
 */
