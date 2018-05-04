/*
 * Copyright (c) 2015-2018 Xtheme Development Group (Xtheme.org)
 * Rights to this code are as documented in doc/LICENSE.
 *
 * Similar to ChanServ CLOSE, but allows for channels to
 * remain open while all access on the channel is suspended.
 *
 */

#include "atheme.h"

DECLARE_MODULE_V1
(
	"chanserv/freeze", false, _modinit, _moddeinit,
	PACKAGE_STRING,
	VENDOR_STRING
);

static void cs_cmd_freeze(sourceinfo_t *si, int parc, char *parv[]);

/* FREEZE ON|OFF -- don't pollute the root with REOPEN */
command_t cs_freeze = { "FREEZE", N_("Suspends all channel access."),
			PRIV_CHAN_ADMIN, 3, cs_cmd_freeze , { .path = "cservice/freeze" }};

static void freeze_check_join(hook_channel_joinpart_t *data);

void _modinit(module_t *m)
{
	service_named_bind_command("chanserv", &cs_freeze);
	hook_add_event("channel_join");
	hook_add_first_channel_join(freeze_check_join);
}

void _moddeinit(module_unload_intent_t intent)
{
	service_named_unbind_command("chanserv", &cs_freeze);
	hook_del_channel_join(freeze_check_join);
}

static void freeze_check_join(hook_channel_joinpart_t *data)
{
	mychan_t *mc;
	chanuser_t *cu = data->cu;

	if (cu == NULL || is_internal_client(cu->user))
		return;
	mc = mychan_find(cu->chan->name);
	if (mc == NULL)
		return;

	if (metadata_find(mc, "private:frozen:freezer"))
	{
		/* don't join if we're already in there */
		if (!chanuser_find(cu->chan, user_find_named(chansvs.nick)))
			join(cu->chan->name, chansvs.nick);

		/* stay to prevent rejoins for ops */
		mc->flags |= MC_INHABIT;
	}
}

static void cs_cmd_freeze(sourceinfo_t *si, int parc, char *parv[])
{
	char *target = parv[0];
	char *action = parv[1];
	char *reason = parv[2];
	mychan_t *mc;
	metadata_t *md;
	channel_t *c;
	chanuser_t *cu, *origin_cu = NULL;
	mowgli_node_t *n, *tn;
	const char *topicsetter;
	time_t prevtopicts;

	if (!target || !action)
	{
		command_fail(si, fault_needmoreparams, STR_INSUFFICIENT_PARAMS, "FREEZE");
		command_fail(si, fault_needmoreparams, _("Usage: FREEZE <#channel> <ON|OFF> [reason]"));
		return;
	}

	if (!(mc = mychan_find(target)))
	{
		command_fail(si, fault_nosuch_target, _("Channel \2%s\2 is not registered."), target);
		return;
	}

	if (!strcasecmp(action, "ON"))
	{
		if (!reason)
		{
			command_fail(si, fault_needmoreparams, STR_INSUFFICIENT_PARAMS, "FREEZE");
			command_fail(si, fault_needmoreparams, _("Usage: FREEZE <#channel> ON <reason>"));
			return;
		}

		if (mc->flags & CHAN_LOG)
		{
			command_fail(si, fault_noprivs, _("\2%s\2 cannot be frozen."), target);
			return;
		}

		if (metadata_find(mc, "private:frozen:freezer"))
		{
			command_fail(si, fault_nochange, _("\2%s\2 is already frozen."), target);
			return;
		}

		metadata_add(mc, "private:frozen:freezer", get_oper_name(si));
		metadata_add(mc, "private:frozen:reason", reason);
		metadata_add(mc, "private:frozen:timestamp", number_to_string(CURRTIME));

		/* if there is a BotServ bot, remove it */
		if ((md = metadata_find(mc, "private:botserv:bot-assigned")) != NULL)
		{
			part(mc->name, md->value);
			metadata_delete(mc, "private:botserv:bot-assigned");
			metadata_delete(mc, "private:botserv:bot-handle-fantasy");
		}

		if ((c = channel_find(target)))
		{
			if (!chanuser_find(c, user_find_named(chansvs.nick)))
				join(target, chansvs.nick);

			/* stay for a bit to stop rejoin floods */
			mc->flags |= MC_INHABIT;

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

		}

		topicsetter = "Services";
		prevtopicts = c->topicts;
		handle_topic(c, topicsetter, CURRTIME, "Channel has been mass suspended (FROZEN) by Network Staff");
		topic_sts(c, chansvs.me->me, topicsetter, CURRTIME, prevtopicts, "Channel has been mass suspended (FROZEN) by Network Staff");

		wallops("%s froze the channel \2%s\2 (%s).", get_oper_name(si), target, reason);
		logcommand(si, CMDLOG_ADMIN, "FREEZE:ON: \2%s\2 (reason: \2%s\2)", target, reason);
		command_success_nodata(si, _("\2%s\2 is now frozen."), target);
	}
	else if (!strcasecmp(action, "OFF"))
	{
		if (!metadata_find(mc, "private:frozen:freezer"))
		{
			command_fail(si, fault_nochange, _("\2%s\2 is not frozen."), target);
			return;
		}

		metadata_delete(mc, "private:frozen:freezer");
		metadata_delete(mc, "private:frozen:reason");
		metadata_delete(mc, "private:frozen:timestamp");
		c = channel_find(target);
		if (c != NULL)
		{
			/* hmm, channel still exists, probably permanent? */
			check_modes(mc, true);
			mc->flags &= ~MC_INHABIT;
			if (!(mc->flags & MC_GUARD) && !(mc->flags & CHAN_LOG) && chanuser_find(c, user_find_named(chansvs.nick)))
				part(target, chansvs.nick);
		}

		topicsetter = "Services";
		prevtopicts = c->topicts;
		handle_topic(c, topicsetter, CURRTIME, "Channel has been un-suspended (UN-FROZEN) by Network Staff");
		topic_sts(c, chansvs.me->me, topicsetter, CURRTIME, prevtopicts, "Channel has been un-suspended (UN-FROZEN) by Network Staff");

		wallops("%s unfroze the channel \2%s\2.", get_oper_name(si), target);
		logcommand(si, CMDLOG_ADMIN, "FREEZE:OFF: \2%s\2", target);
		command_success_nodata(si, _("\2%s\2 has been unfrozen."), target);
	}
	else
	{
		command_fail(si, fault_badparams, STR_INVALID_PARAMS, "FREEZE");
		command_fail(si, fault_badparams, _("Usage: FREEZE <#channel> <ON|OFF> [reason]"));
	}
}

/* vim:cinoptions=>s,e0,n0,f0,{0,}0,^0,=s,ps,t0,c3,+s,(2s,us,)20,*30,gs,hs
 * vim:ts=8
 * vim:sw=8
 * vim:noexpandtab
 */
