/*
 * Copyright (c) 2014-2018 Xtheme Development Group (Xtheme.org)
 *
 * This file contains routines to handle the CService SET NOOP command.
 *
 */

#include "atheme.h"

DECLARE_MODULE_V1
(
	"chanserv/set_noop", false, _modinit, _moddeinit,
	PACKAGE_STRING,
	VENDOR_STRING
);

static void cs_cmd_set_noop(sourceinfo_t *si, int parc, char *parv[]);

command_t cs_set_noop = { "NOOP", N_("All op functions must be performed through Services."), AC_NONE, 2, cs_cmd_set_noop, { .path = "cservice/set_noop" } };

mowgli_patricia_t **cs_set_cmdtree;

void _modinit(module_t *m)
{
	MODULE_TRY_REQUEST_SYMBOL(m, cs_set_cmdtree, "chanserv/set_core", "cs_set_cmdtree");

	command_add(&cs_set_noop, *cs_set_cmdtree);
}

void _moddeinit(module_unload_intent_t intent)
{
	command_delete(&cs_set_noop, *cs_set_cmdtree);
}

static void cs_cmd_set_noop(sourceinfo_t *si, int parc, char *parv[])
{
	mychan_t *mc;

	if (!(mc = mychan_find(parv[0])))
	{
		command_fail(si, fault_nosuch_target, _("Channel \2%s\2 is not registered."), parv[0]);
		return;
	}

	if (metadata_find(mc, "private:frozen:freezer"))
	{
		command_fail(si, fault_noprivs, _("\2%s\2 is frozen."), parv[0]);
		return;
	}

	if (!parv[1])
	{
		command_fail(si, fault_needmoreparams, STR_INSUFFICIENT_PARAMS, "SET NOOP");
		return;
	}

	if (!chanacs_source_has_flag(mc, si, CA_SET))
	{
		command_fail(si, fault_noprivs, _("You are not authorized to perform this command."));
		return;
	}

	if (chanacs_source_has_flag(mc, si, CA_SUSPENDED))
	{
		command_fail(si, fault_noprivs, _("Your access in %s is \2suspended\2."), parv[0]);
		return;
	}

	if (!strcasecmp("ON", parv[1]))
	{
		if (MC_NOOP & mc->flags)
		{
			command_fail(si, fault_nochange, _("The \2%s\2 flag is already set for channel \2%s\2."), "NOOP", mc->name);
			return;
		}

		logcommand(si, CMDLOG_SET, "SET:NOOP:ON: \2%s\2", mc->name);
		verbose(mc, _("\2%s\2 enabled the NOOP flag - all Operator functions must be done through Services."), get_source_name(si));

		mc->flags |= MC_NOOP;

		command_success_nodata(si, _("The \2%s\2 flag has been set for channel \2%s\2."), "NOOP", mc->name);
		return;
	}
	else if (!strcasecmp("OFF", parv[1]))
	{
		if (!(MC_NOOP & mc->flags))
		{
			command_fail(si, fault_nochange, _("The \2%s\2 flag is not set for channel \2%s\2."), "NOOP", mc->name);
			return;
		}

		logcommand(si, CMDLOG_SET, "SET:NOOP:OFF: \2%s\2", mc->name);
		verbose(mc, _("\2%s\2 disabled the NOOP flag"), get_source_name(si));

		mc->flags &= ~MC_NOOP;

		command_success_nodata(si, _("The \2%s\2 flag has been removed for channel \2%s\2."), "NOOP", mc->name);
		return;
	}
	else
	{
		command_fail(si, fault_badparams, STR_INVALID_PARAMS, "NOOP");
		return;
	}
}
