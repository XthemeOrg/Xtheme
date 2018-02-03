/*
 * Copyright (c) 2014-2018 Xtheme Development Group (Xtheme.org)
 * Rights to this code are as documented in doc/LICENSE.
 *
 * This file contains code for the ChanServ CLEAR OPS function.
 *
 */

#include "atheme.h"

DECLARE_MODULE_V1
(
	"chanserv/clear_ops", false, _modinit, _moddeinit,
	PACKAGE_STRING,
	VENDOR_STRING
);

static void cs_cmd_clear_ops(sourceinfo_t *si, int parc, char *parv[]);

command_t cs_clear_ops = { "OPS", N_("Deops all channel operators on a channel."),
	AC_NONE, 2, cs_cmd_clear_ops, { .path = "cservice/clear_ops" } };

mowgli_patricia_t **cs_clear_cmds;

void _modinit(module_t *m)
{
	MODULE_TRY_REQUEST_SYMBOL(m, cs_clear_cmds, "chanserv/clear", "cs_clear_cmds");

	command_add(&cs_clear_ops, *cs_clear_cmds);
}

void _moddeinit(module_unload_intent_t intent)
{
	command_delete(&cs_clear_ops, *cs_clear_cmds);
}

static void cs_cmd_clear_ops(sourceinfo_t *si, int parc, char *parv[])
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

	/* deop everyone */
			MOWGLI_ITER_FOREACH(n, mc->chan->members.head)
			{
					cu = (chanuser_t *)n->data;

					if (cu->user == si->su)
							origin_cu = cu;
					else if (is_internal_client(cu->user))
							;
					else
					{
						if (ircd->uses_owner && (ircd->owner_mode & cu->modes))
						{
							modestack_mode_param(chansvs.nick, mc->chan, MTYPE_DEL, ircd->owner_mchar[1], CLIENT_NAME(cu->user));
							cu->modes &= ~ircd->owner_mode;
						}
						if (ircd->uses_protect && (ircd->protect_mode & cu->modes))
						{
							modestack_mode_param(chansvs.nick, mc->chan, MTYPE_DEL, ircd->protect_mchar[1], CLIENT_NAME(cu->user));
							cu->modes &= ~ircd->protect_mode;
						}
						if ((CSTATUS_OP & cu->modes))
						{
							modestack_mode_param(chansvs.nick, mc->chan, MTYPE_DEL, 'o', CLIENT_NAME(cu->user));
							cu->modes &= ~CSTATUS_OP;
						}
						if (ircd->uses_halfops && (ircd->halfops_mode & cu->modes))
						{
							modestack_mode_param(chansvs.nick, mc->chan, MTYPE_DEL, ircd->halfops_mchar[1], CLIENT_NAME(cu->user));
							cu->modes &= ~ircd->halfops_mode;
						}
					}
			}

	logcommand(si, CMDLOG_DO, "CLEAR:OPS: \2%s\2", mc->name);

	command_success_nodata(si, _("Cleared channel ops in \2%s\2."), channel);
}

/* vim:cinoptions=>s,e0,n0,f0,{0,}0,^0,=s,ps,t0,c3,+s,(2s,us,)20,*30,gs,hs
 * vim:ts=8
 * vim:sw=8
 * vim:noexpandtab
 */
