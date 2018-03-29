/*
 * Copyright (c) 2014-2018 Xtheme Development Group <Xtheme.org>
 *
 * This file contains routines to handle the Proxyscan HELP command.
 *
 */

#include "atheme.h"

DECLARE_MODULE_V1
(
	"proxyscan/help", false, _modinit, _moddeinit,
	PACKAGE_STRING,
	VENDOR_STRING
);

static void ps_cmd_help(sourceinfo_t *si, int parc, char *parv[]);

command_t ps_help = { "HELP", N_("Displays contextual help information."), AC_NONE, 1, ps_cmd_help, { .path = "help" } };

void _modinit(module_t *m)
{
	service_named_bind_command("proxyscan", &ps_help);
}

void _moddeinit(module_unload_intent_t intent)
{
	service_named_unbind_command("proxyscan", &ps_help);
}

static void ps_cmd_help(sourceinfo_t *si, int parc, char *parv[])
{
	char *command = parv[0];

	if (!has_any_privs(si))
	{
		command_fail(si, fault_noprivs, _("You are not authorized to use %s."), si->service->nick);
		return;
	}

	if (!command)
	{
		command_success_nodata(si, _("***** \2%s Help\2 *****"), si->service->nick);
		command_success_nodata(si, _("\2%s\2 provides DNS Blacklist scanning, automated\n"
					"or manually initiated to \2%s\2!"),
				si->service->nick, si->service->nick);
		command_success_nodata(si, " ");
		command_success_nodata(si, _("For information on a command, type:"));
		command_success_nodata(si, "\2/%s%s help <command>\2", (ircd->uses_rcommand == false) ? "msg " : "", si->service->disp);
		command_success_nodata(si, " ");

		command_help(si, si->service->commands);

		command_success_nodata(si, _("***** \2End of Help\2 *****"));
		return;
	}

	/* take the command through the hash table */
	help_display(si, si->service, command, si->service->commands);
}


