/*
 * Copyright (c) 2016-2018 Xtheme Development Group <www.Xtheme.org>
 * Rights to this code are as documented in doc/LICENSE.
 *
 * When set to ON, only IRCops can SENDPASS the account.
 */

#include "atheme.h"
#include "uplink.h"

DECLARE_MODULE_V1
(
	"nickserv/set_blocksendpass", false, _modinit, _moddeinit,
	PACKAGE_STRING,
	VENDOR_STRING
);

mowgli_patricia_t **ns_set_cmdtree;

static void ns_cmd_set_blocksendpass(sourceinfo_t *si, int parc, char *parv[]);

command_t ns_set_blocksendpass = { "BLOCKSENDPASS", N_("Allows only IRC Operators to SENDPASS your account."), AC_NONE, 1, ns_cmd_set_blocksendpass, { .path = "nickserv/set_blocksendpass" } };

void _modinit(module_t *m)
{
	MODULE_TRY_REQUEST_SYMBOL(m, ns_set_cmdtree, "nickserv/set_core", "ns_set_cmdtree");

	command_add(&ns_set_blocksendpass, *ns_set_cmdtree);

}

void _moddeinit(module_unload_intent_t intent)
{
	command_delete(&ns_set_blocksendpass, *ns_set_cmdtree);
}

static void ns_cmd_set_blocksendpass(sourceinfo_t *si, int parc, char *parv[])
{
	char *params = parv[0];


	if (!params)
	{
		command_fail(si, fault_needmoreparams, STR_INSUFFICIENT_PARAMS, "BLOCKSENDPASS");
		return;
	}

	if (!strcasecmp("ON", params))
	{
		if (metadata_find(si->smu, "private:blocksendpass:on"))
		{
			command_fail(si, fault_nochange, _("The \2%s\2 flag is already set for account \2%s\2."), "BLOCKSENDPASS", entity(si->smu)->name);
			return;
		}

		logcommand(si, CMDLOG_SET, "SET:BLOCKSENDPASS:ON");

		metadata_add(si->smu, "private:blocksendpass:on", "ON");

		command_success_nodata(si, _("The \2%s\2 flag has been set for account \2%s\2."), "BLOCKSENDPASS", entity(si->smu)->name);

		return;
	}
	else if (!strcasecmp("OFF", params))
	{
		if (!metadata_find(si->smu, "private:blocksendpass:on"))
		{
			command_fail(si, fault_nochange, _("The \2%s\2 flag is not set for account \2%s\2."), "BLOCKSENDPASS", entity(si->smu)->name);
			return;
		}

		logcommand(si, CMDLOG_SET, "SET:BLOCKSENDPASS:OFF");

		metadata_delete(si->smu, "private:blocksendpass:on");

		command_success_nodata(si, _("The \2%s\2 flag has been removed for account \2%s\2."), "BLOCKSENDPASS", entity(si->smu)->name);

		return;
	}
	else
	{
		command_fail(si, fault_badparams, STR_INVALID_PARAMS, "BLOCKSENDPASS");
		return;
	}
}
