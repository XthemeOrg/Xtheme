/*
 * Copyright (c) 2014-2018 Xtheme Development Group (Xtheme.org)
 * Copyright (c) 2005 William Pitcock, et al.
 * Rights to this code are as documented in doc/LICENSE.
 *
 * This file contains code for the CService GETKEY functions.
 *
 */

#include "atheme.h"

DECLARE_MODULE_V1
(
	"chanserv/getkey", false, _modinit, _moddeinit,
	PACKAGE_STRING,
	VENDOR_STRING
);

static void cs_cmd_getkey(sourceinfo_t *si, int parc, char *parv[]);

command_t cs_getkey = { "GETKEY", N_("Returns the key (+k) of a channel."),
                        AC_NONE, 1, cs_cmd_getkey, { .path = "cservice/getkey" } };

void _modinit(module_t *m)
{
        service_named_bind_command("chanserv", &cs_getkey);
}

void _moddeinit(module_unload_intent_t intent)
{
	service_named_unbind_command("chanserv", &cs_getkey);
}

static void cs_cmd_getkey(sourceinfo_t *si, int parc, char *parv[])
{
	char *chan = parv[0];
	mychan_t *mc;
	metadata_t *md, *md2;

	if (!chan)
	{
		command_fail(si, fault_needmoreparams, STR_INSUFFICIENT_PARAMS, "GETKEY");
		command_fail(si, fault_needmoreparams, _("Syntax: GETKEY <#channel>"));
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

	if (!chanacs_source_has_flag(mc, si, CA_INVITE))
	{
		command_fail(si, fault_noprivs, _("You are not authorized to perform this operation."));
		return;
	}

	if (chanacs_source_has_flag(mc, si, CA_SUSPENDED))
	{
		command_fail(si, fault_noprivs, _("Your access in %s is \2suspended\2."), chan);
		md = metadata_find(ca, "sreason");
		if ((md2 = metadata_find(ca, "expires")))
		{
			snprintf(expiry, sizeof expiry, "%s", md2->value);
			expires_on = (time_t)atol(expiry);
			time_left = difftime(expires_on, CURRTIME);
		}

		if (md != NULL)
		{
			command_fail(si, fault_noprivs, _("Suspension reason: %s -- Expiration: %s", md->value, timediff(time_left)));
		}
		return;
	}

	if (!mc->chan)
	{
		command_fail(si, fault_nosuch_target, _("\2%s\2 is currently empty."), mc->name);
		return;
	}

	if (!mc->chan->key)
	{
		command_fail(si, fault_nosuch_key, _("\2%s\2 is not keyed."), mc->name);
		return;
	}
	logcommand(si, CMDLOG_GET, "GETKEY: \2%s\2", mc->name);
	command_success_string(si, mc->chan->key, _("Channel \2%s\2 key is: %s"),
			mc->name, mc->chan->key);
}

/* vim:cinoptions=>s,e0,n0,f0,{0,}0,^0,=s,ps,t0,c3,+s,(2s,us,)20,*30,gs,hs
 * vim:ts=8
 * vim:sw=8
 * vim:noexpandtab
 */
