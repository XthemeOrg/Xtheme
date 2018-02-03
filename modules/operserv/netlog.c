/*
 * Copyright (c) 2017-2018 siniStar (Austin Ellis)
 * Xtheme Development Group (Xtheme.org)
 * Rights to this code are as documented in doc/LICENSE.
 *
 * operserv/netlog - Network logging routines for Xtheme.
 * Provides network logging of: connections & disconnects,
 * joins & parts and topic changes.
 *
 */

#include "atheme.h"

DECLARE_MODULE_V1
(
	"operserv/netlog", false, _modinit, _moddeinit,
	PACKAGE_STRING,
	VENDOR_STRING
);

static void watch_net_joins(hook_channel_joinpart_t *hdata);
static void watch_net_parts(hook_channel_joinpart_t *hdata);
static void watch_net_topics(channel_t *c);
static void watch_net_connects(hook_user_nick_t *hdata);
static void user_delete_info(hook_user_delete_t *hdata);
static void watch_net_nicks(hook_user_nick_t *hdata);

static void watch_net_joins(hook_channel_joinpart_t *hdata)
{
	chanuser_t *cu = hdata->cu;

	if (cu == NULL)
		return;

	if (!(cu->user->server->flags & SF_EOB))
		return;

	/* If the IP is NULL, ignore. (it's of no use to us anyway) --siniStar */
	if (cu->user->ip == NULL)
		return;

	slog(LG_NETLOG, "\2JOIN:\2 %s (%s@%s) [%s] joined \2%s\2",
			cu->user->nick, cu->user->user, cu->user->host, cu->user->ip, cu->chan->name);
	return;

}

static void watch_net_parts(hook_channel_joinpart_t *hdata)
{
	chanuser_t *cu = hdata->cu;

	if (cu == NULL)
		return;

	if (!(cu->user->server->flags & SF_EOB))
		return;

	/* If the IP is NULL, ignore. (it's of no use to us anyway) --siniStar */
        if (cu->user->ip == NULL)
                return;

	slog(LG_NETLOG, "\2PART:\2 %s (%s@%s) [%s] left \2%s\2",
			cu->user->nick, cu->user->user, cu->user->host, cu->user->ip, cu->chan->name);
	return;

}

static void watch_net_topics(channel_t *c)
{
	mychan_t *mc;

	mc = mychan_find(c->name);

	if (mc == NULL)
		return;

	slog(LG_NETLOG, "\2TOPIC:\2 %s changed topic on \2%s\2 to '%s'",
			c->topic_setter, c->name, c->topic);
	return;

}

static void user_delete_info_hook(hook_user_delete_t *hdata)
{
	if (hdata->u->nick == NULL)
		return;

	/* If the IP is NULL, ignore. (it's of no use to us anyway) --siniStar */
        if (hdata->u->ip == NULL)
                return;

	slog(LG_NETLOG, "\2QUIT:\2 %s (%s@%s) [%s] disconnected from \2%s\2 (%s)",
			hdata->u->nick, hdata->u->user, hdata->u->host, hdata->u->ip, hdata->u->server->name, hdata->comment);

}

static void watch_net_connects(hook_user_nick_t *hdata)
{
	user_t *u = hdata->u;

	if (u == NULL)
		return;

	/* If the IP is NULL, ignore. (it's of no use to us anyway) --siniStar */
        if (hdata->u->ip == NULL)
                return;

	slog(LG_NETLOG, "\2CONNECT:\2 %s (%s@%s) [%s] connected to \2%s\2",
			u->nick, u->user, u->host, u->ip, u->server->name);
	return;

}

static void watch_net_nicks(hook_user_nick_t *hdata)
{
	user_t *u = hdata->u;

	if (u == NULL)
		return;

	/* If the IP is NULL, ignore. (it's of no use to us anyway) --siniStar */
        if (u->ip == NULL)
                return;

	slog(LG_NETLOG, "\2NICK:\2 %s (%s@%s) [%s] is now \2%s\2",
			hdata->oldnick, u->user, u->host, u->ip, u->nick);
	return;

}

void _modinit(module_t *m)
{
	hook_add_event("channel_join");
	hook_add_channel_join(watch_net_joins);
	hook_add_event("channel_part");
	hook_add_channel_part(watch_net_parts);
	hook_add_event("channel_topic");
	hook_add_channel_topic(watch_net_topics);
	hook_add_event("user_add");
	hook_add_user_add(watch_net_connects);
	hook_add_event("user_delete_info");
	hook_add_user_delete_info(user_delete_info_hook);
	hook_add_event("user_nickchange");
	hook_add_user_nickchange(watch_net_nicks);

}

void _moddeinit(module_unload_intent_t intent)
{
	hook_del_event("channel_join");
	hook_del_channel_join(watch_net_joins);
	hook_del_event("channel_part");
	hook_del_channel_part(watch_net_parts);
	hook_del_event("channel_topic");
	hook_del_channel_topic(watch_net_topics);
	hook_del_event("user_add");
	hook_del_user_add(watch_net_connects);
	hook_del_event("user_delete_info");
	hook_del_user_delete_info(user_delete_info_hook);
	hook_del_event("user_nickchange");
	hook_del_user_nickchange(watch_net_nicks);

}
