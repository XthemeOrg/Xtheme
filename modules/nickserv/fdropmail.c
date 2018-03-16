/*
 * Copyright (c) 2018 Xtheme Development Group <Xtheme.org>
 * Rights to this code are as documented in doc/LICENSE.
 *
 * This file contains code for the NickServ LISTMAIL function.
 *
 */

#include "atheme.h"

DECLARE_MODULE_V1
(
	"nickserv/fdropmail", false, _modinit, _moddeinit,
	PACKAGE_STRING,
	VENDOR_STRING
);

static void ns_cmd_fdropmail(sourceinfo_t *si, int parc, char *parv[]);

command_t ns_fdropmail = { "FDROPMAIL", N_("Forcibly drops accounts registered to an e-mail address."), PRIV_USER_ADMIN, 1, ns_cmd_fdropmail, { .path = "nickserv/fdropmail" } };

void _modinit(module_t *m)
{
	service_named_bind_command("nickserv", &ns_fdropmail);
}

void _moddeinit(module_unload_intent_t intent)
{
	service_named_unbind_command("nickserv", &ns_fdropmail);
}

struct fdropmail_state
{
	sourceinfo_t *origin;
	char *pattern;
	stringref email_canonical;
	int matches;
};

static int fdropmail_foreach_cb(myentity_t *mt, void *privdata)
{
	struct fdropmail_state *state = (struct fdropmail_state *) privdata;
	myuser_t *mu = user(mt);
	mowgli_node_t *n;
	mynick_t *mn;


	if (state->email_canonical == mu->email_canonical || !match(state->pattern, mu->email))
	{

		if (is_soper(mu))
		{
			command_success_nodata(state->origin, "- %s (%s) (Services Operator)", entity(mu)->name, mu->email);
		}

		if (mu->flags & MU_HOLD)
		{
			command_success_nodata(state->origin, "- %s (%s) *HELD", entity(mu)->name, mu->email);
		}

		else
		{
			/* in the future we could add a LIMIT parameter */
			if (state->matches == 0)
				command_success_nodata(state->origin, "Accounts matching e-mail address \2%s\2:", state->pattern);

			command_success_nodata(state->origin, "- %s (%s)", entity(mu)->name, mu->email);
			state->matches++;
			hook_call_user_drop(mu);
			if (!nicksvs.no_nick_ownership)
			{
				MOWGLI_ITER_FOREACH(n, mu->nicks.head)
				{
					mn = n->data;
					holdnick_sts(nicksvs.me->me, 0, mn->nick, NULL);
				}
			}
			command_success_nodata(state->origin, _("The account \2%s\2 has been dropped."), entity(mu)->name);
			object_dispose(mu);
		}
	}
	return 0;
}

static void ns_cmd_fdropmail(sourceinfo_t *si, int parc, char *parv[])
{
	char *email = parv[0];
	struct fdropmail_state state;
	myuser_t *mu;

	if (!email)
	{
		command_fail(si, fault_needmoreparams, STR_INSUFFICIENT_PARAMS, "FDROPMAIL");
		command_fail(si, fault_needmoreparams, _("Syntax: FDROPMAIL <email>"));
		return;
	}

	state.matches = 0;
	state.pattern = email;
	state.email_canonical = canonicalize_email(email);
	state.origin = si;
	myentity_foreach_t(ENT_USER, fdropmail_foreach_cb, &state);
	strshare_unref(state.email_canonical);

	logcommand(si, CMDLOG_ADMIN, "FDROPMAIL: \2%s\2 (\2%d\2 matches)", email, state.matches);
	if (state.matches == 0)
		command_success_nodata(si, _("No accounts matched e-mail address \2%s\2"), email);
	else
		command_success_nodata(si, ngettext(N_("\2%d\2 match for e-mail address \2%s\2"),
						    N_("\2%d\2 matches for e-mail address \2%s\2"), state.matches), state.matches, email);
}


