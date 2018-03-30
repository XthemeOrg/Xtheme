/*
 * Copyright (c) 2014-2018 Xtheme Development Group (Xtheme.org)
 * Rights to this code are as documented in doc/LICENSE.
 *
 * This file contains code for the ChanServ MASS VOICE function.
 *
 */

#include "atheme.h"

DECLARE_MODULE_V1
(
	"chanserv/mass_voice", false, _modinit, _moddeinit,
	PACKAGE_STRING,
	VENDOR_STRING
);

static void cs_cmd_mass_voice(sourceinfo_t *si, int parc, char *parv[]);
static void cs_cmd_mass_devoice(sourceinfo_t *si, int parc, char *parv[]);

command_t cs_mass_voice = { "VOICE", N_("Voices all users on a channel."),
	AC_NONE, 2, cs_cmd_mass_voice, { .path = "cservice/mass_voice" } };
command_t cs_mass_devoice = { "DEVOICE", N_("De-voices all users on a channel."),
	AC_NONE, 2, cs_cmd_mass_devoice, { .path = "cservice/mass_devoice" } };

mowgli_patricia_t **cs_mass_cmds;

void _modinit(module_t *m)
{
	MODULE_TRY_REQUEST_SYMBOL(m, cs_mass_cmds, "chanserv/mass", "cs_mass_cmds");

	command_add(&cs_mass_voice, *cs_mass_cmds);
	command_add(&cs_mass_devoice, *cs_mass_cmds);
}

void _moddeinit(module_unload_intent_t intent)
{
	command_delete(&cs_mass_voice, *cs_mass_cmds);
	command_delete(&cs_mass_devoice, *cs_mass_cmds);
}

static void cs_cmd_mass_voice(sourceinfo_t *si, int parc, char *parv[])
{
	channel_t *c;
	char *channel = parv[0];
	mychan_t *mc = mychan_find(channel);
	chanuser_t *cu, *origin_cu = NULL;
	mowgli_node_t *n, *tn;

	if (!mc)
	{
		command_fail(si, fault_nosuch_target, _("Channel \2%s\2 is not registered."), channel);
		return;
	}

	if (metadata_find(mc, "private:frozen:freezer"))
	{
		command_fail(si, fault_noprivs, _("\2%s\2 is frozen."), channel);
		return;
	}

	if (!(c = channel_find(channel)))
	{
		command_fail(si, fault_nosuch_target, _("\2%s\2 is currently empty."), channel);
		return;
	}

	if (!chanacs_source_has_flag(mc, si, CA_RECOVER))
	{
		command_fail(si, fault_noprivs, _("You are not authorized to perform this operation."));
		return;
	}

	if (!chanacs_source_has_flag(mc, si, CA_VOICE))
	{
		command_fail(si, fault_noprivs, _("You are not authorized to perform this operation."));
		return;
	}

	if (chanacs_source_has_flag(mc, si, CA_SUSPENDED))
	{
		command_fail(si, fault_noprivs, _("Your access in %s is \2suspended\2."), channel);
		return;
	}

	if (metadata_find(mc, "private:close:closer"))
	{
		command_fail(si, fault_noprivs, _("\2%s\2 is closed."), channel);
		return;
	}

	/* voice everyone */
			MOWGLI_ITER_FOREACH(n, mc->chan->members.head)
			{
					cu = (chanuser_t *)n->data;
					if (is_internal_client(cu->user))
							;
					else
					{
							modestack_mode_param(chansvs.nick, mc->chan, MTYPE_ADD, 'v', CLIENT_NAME(cu->user));
							cu->modes |= CSTATUS_VOICE;
					}
			}

	logcommand(si, CMDLOG_DO, "MASS:VOICE: \2%s\2", mc->name);

	command_success_nodata(si, _("Mass Voiced channel users in \2%s\2."), channel);
}

static void cs_cmd_mass_devoice(sourceinfo_t *si, int parc, char *parv[])
{
	channel_t *c;
	char *channel = parv[0];
	mychan_t *mc = mychan_find(channel);
	chanuser_t *cu, *origin_cu = NULL;
	mowgli_node_t *n, *tn;

	if (!mc)
	{
		command_fail(si, fault_nosuch_target, _("Channel \2%s\2 is not registered."), channel);
		return;
	}

	if (metadata_find(mc, "private:frozen:freezer"))
	{
		command_fail(si, fault_noprivs, _("\2%s\2 is frozen."), channel);
		return;
	}

	if (!(c = channel_find(channel)))
	{
		command_fail(si, fault_nosuch_target, _("\2%s\2 is currently empty."), channel);
		return;
	}

	if (!chanacs_source_has_flag(mc, si, CA_RECOVER))
	{
		command_fail(si, fault_noprivs, _("You are not authorized to perform this operation."));
		return;
	}

	if (!chanacs_source_has_flag(mc, si, CA_VOICE))
	{
		command_fail(si, fault_noprivs, _("You are not authorized to perform this operation."));
		return;
	}

	if (chanacs_source_has_flag(mc, si, CA_SUSPENDED))
	{
		command_fail(si, fault_noprivs, _("Your access in %s is \2suspended\2."), channel);
		return;
	}

	if (metadata_find(mc, "private:close:closer"))
	{
		command_fail(si, fault_noprivs, _("\2%s\2 is closed."), channel);
		return;
	}

	/* de-voice everyone */
			MOWGLI_ITER_FOREACH(n, mc->chan->members.head)
			{
					cu = (chanuser_t *)n->data;
					if (is_internal_client(cu->user))
							;
					else
					{
							modestack_mode_param(chansvs.nick, mc->chan, MTYPE_DEL, 'v', CLIENT_NAME(cu->user));
							cu->modes &= ~CSTATUS_VOICE;
					}
			}

	logcommand(si, CMDLOG_DO, "MASS:DEVOICE: \2%s\2", mc->name);

	command_success_nodata(si, _("Mass De-Voiced channel users in \2%s\2."), channel);
}
