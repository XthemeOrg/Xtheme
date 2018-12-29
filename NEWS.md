Xtheme IRC Services 8.0.3 Development Notes  
========================================================  
nickserv  
--------  
* badmail: Has been reverted to it's original version. The option to AKILL is really not necessary, and should not have been added.
!NOTE:  
 1. You will want to perform a BADMAIL LIST and save your entries.  
 2. You will need to DELETE all previous entries (or edit services.db and remove all lines with 'BE2')  
 3. You will need to re-add all previous entries.
  NOTE!

operserv  
--------  
* rwatch: Update to include all RWATCH matches to the AKILL management system instead of just being sent server2server. It makes hits and matches much easier to manage.

Xtheme Development is winding down.  It has been fun working on this project and it's offerings throughout the years - but all good things come to an end. Most of the (sensible) goals have been accomplished. Support will cease in February of 2019, but in the meantime can be obtained via GitHub Issues or via IRC4Fun in #Xtheme  


Xtheme IRC Services 8.0.2 Development Notes  
========================================================  
nickserv  
--------  
* nickserv: Fixed bug that allowed users to crash services.    

## STAY UP TO DATE WITH XthemeOrg NEWS, UPDATES & SECURITY ADVISORIES  
Subscribe to the XthemeOrg News at:  
https://xtheme.org/org-tools/newsletter/   


Xtheme IRC Services 8.0.1 Development Notes  
========================================================  
chanserv  
--------  
* SUSPENDed users will now be notified of their suspension of access/flags upon joining the channel.  
* SUSPEND has been modified to no longer refer to a list _(missed in prior removals of 'list' reference)_  
* ANTIFLOOD enhancements have been reduced back to their original values after complaints of innocent users being caught by ANTIFLOOD on _larger_ networks.  _(Services will still inform the channel each time it is triggered, and by whom triggered it.)_  
* FROZEN channels are now topic-changed, informing of the mass-suspension (FROZEN) channel. _(The channel can continue being used, but no operator commands)_  
* FROZEN channels are now topic-changed, informing of the mass-suspension (FROZEN) channel AT THE TIME OF ISSUE.  Recreated channels will also have their topics changed, reflecting the status.  
* UNFROZEN channels are now topic-changed, informing of the mass UN-suspension (UNFROZEN) channel.  _(This topic will continue to get re-set upon channel's creation until changed)_  

logging  
-------  
* VERBOSE logging will now show refreshing (saving) of services data.   

proxyscan  
---------  
* Added HELP files for ProxyScan.  (Note that SET DNSBLACTION is an OperServ command)  

## STAY UP TO DATE WITH XthemeOrg NEWS, UPDATES & SECURITY ADVISORIES  
Subscribe to the XthemeOrg News at:  
https://xtheme.org/org-tools/newsletter/   


Xtheme IRC Services 8.0.0 Development Notes  
========================================================  
chanserv  
--------  
* AntiFlood updates, making it more effective against drive-by spammers, and flooders.  Also performs a verbose notice upon trigger.  
* MASS DE-Commands are now available. _(Such as MASS DEVOICE, MASS DEOP, etc)_  
* Added SET NOOP - when set, only ChanServ or BotServ Bots will be allowed to be Operator and all commands must run through ChanServ.  

nickserv  
--------  
* Added FDROPMAIL, which allows Services Operators to FDROP accounts by email address.  
* Added FGROUP, which allows Services Operators to forcibly GROUP a nickname to their own account, or a user account.  
* Added SET BLOCKSENDPASS, which when set to ON will only allow IRC Operators to SENDPASS that account. (per account setting)

operserv  
--------  
* Cleaned up OperServ INFO command.  Displayed output will now be in sections.  


## STAY UP TO DATE WITH XthemeOrg NEWS, UPDATES & SECURITY ADVISORIES  
Subscribe to the XthemeOrg News at:  
https://xtheme.org/org-tools/newsletter/   


Xtheme IRC Services 8.0.0-dev5 Development Notes  
========================================================  
nickserv  
--------  
* Crash bug fixed that was introduced by the option to AKILL on BADMAIL matches, with use of XMLRPC interfaces.  
* Fixed bug that allowed BADMAIL registrations to proceed, even when they are denied. (via XMLRPC)


## STAY UP TO DATE WITH XthemeOrg NEWS, UPDATES & SECURITY ADVISORIES  
Subscribe to the XthemeOrg News at:  
https://xtheme.org/org-tools/newsletter/   

Xtheme IRC Services 8.0.0-dev4 Development Notes  
========================================================  
chanserv  
--------
* flags: updated to not show AKICKS in a FLAGS LIST.  _(AKICKs have their own list, and channels with PUBACL set probably do not want their bans revealed to be easily evaded.)_  
* regproc: chanserv/regproc was removed -- it was full of bugs and the requesting network _(and myself)_ have lost interest in the module.

general
-------
* Copyright years updated.  
* corrected line endings in various modules.    

nickserv  
--------  
* Fix added for BADMAIL entries.  _(They will now be saved to database instead of disappearing)_


## STAY UP TO DATE WITH XthemeOrg NEWS, UPDATES & SECURITY ADVISORIES  
Subscribe to the XthemeOrg News at:  
https://xtheme.org/org-tools/newsletter/    


Xtheme IRC Services 8.0.0-dev3 Development Notes  
==================================================  
chanserv  
--------
* regproc: Removed invalid pointer that could lead to a crash on ACTIVATE or REJECT. _(Fixed)_  

core
----
* version: added Maro to to credits for being an excellent bug-finder and reporter.  

dist  
* rotatelogs.in: Added a Log Rotation script that will _(when **modified correctly** and **crontab'd to run once a day**)_ fixes OperServ GREPLOG.

doc
---
* INSTALL document Updated  

help
----
* cservice/register: Fix to actually provide the necessary output.  
* cservice: some cosmetic cleanups  

helpserv  
* ticket: HelpServ REQUEST command can now notify network staff of help requests via MemoServ and Group memos!  

operserv
* greplog: Now works if the included log rotation script is configured properly, along with a daily crontab _(cronjob)_  


## STAY UP TO DATE WITH XthemeOrg NEWS, UPDATES & SECURITY ADVISORIES  
Subscribe to the XthemeOrg News at:  
https://xtheme.org/org-tools/newsletter/  


Xtheme IRC Services 8.0.0-dev2 Development Notes  
==================================================  
chanserv
--------
* access: LIST command now shows Last Modified information, similar to FLAGS output.  
* access: LIST headers added
* flags: LIST headers added, formatting and spacing fixes (thanks @MrBenC)  
* list: PUBACL is now added as criteria

contrib
-------
* ns_ajoin: Removed check that caused crashes (thanks @MrMaro & @mohdschizo)  

core
----
* function: random_string is now more random (A-Z,a-z,0-9) for keys generated  
* flags: save PUBACL flag so it is not lost across restarts (thanks @MrBenC)  

crypto
------
* MANY improvements to the crypto functionality. (thanks to upstream atheme/atheme and @amdj)    
* YOU SHOULD READ xtheme.conf.example FOR INFORMATION ABOUT HOW CRYPTO IS CHANGING.  
	+ Backup your services.db at **least 3 times** for safety of your database. +  
	_(**YOU SHOULD BE DOING THIS REGULARLY, ANYWAY**)_

help
----
* nickserv/badmail: Update with new functionality and SYNTAX.  

hostserv
--------
* request: Only verified accounts can utilize the REQUEST command.  

nickserv
--------
* badmail: Fixed a crash bug discovered in BADMAIL DEL command (thanks @MrMaro)  
* badmail: Fixed a bug that prevented matches for AKILL actioned entries. (thanks @MrMaro)  
* badmail: Updated log messages to be more helpful. (Show ACTION taken)  

protocol
--------
* nefarious: Patch to prevent display of un-registered users' IP addresses
	being shown during GLINE with Xtheme.  
* nefarious: Update protocol for compatibility, as the protocol changed
	in Nefarious 2.  
	(Thanks to @xnite and @GLolol for these patches)  
* unreal4: Updates to channel modes supported (thanks @MrBenC)  
* unreal/unreal4: Support channel mode +T (NONOTICE) (thanks @MrBenC)


	## STAY UP TO DATE WITH XthemeOrg NEWS, UPDATES & SECURITY ADVISORIES  
	Subscribe to the XthemeOrg News at:  
	https://xtheme.org/org-tools/newsletter/  


Xtheme IRC Services 8.0.0-dev1r2 Development Notes  
==================================================  
database  
--------  
* nickserv/badmail: Fixed database fail from update to BADMAIL so that
	networks do not need to delete all old BADMAIL database entries or
	manually edit the services.db to remove BE entries.  

nickserv  
--------  
* nickserv/badmail: Fixed database fail from update to BADMAIL so that
	networks do not need to delete all old BADMAIL database entries or
	manually edit the services.db to remove BE entries.


	## STAY UP TO DATE WITH XthemeOrg NEWS, UPDATES & SECURITY ADVISORIES  
	Subscribe to the XthemeOrg News at:  
	https://xtheme.org/org-tools/newsletter/  


Xtheme IRC Services 8.0.0-dev1 Development Notes  
==================================================  
botserv
-------
* BotServ bots are now automatically un-assigned if a channel is frozen.
	(Previously, ChanServ would additionally join the channel; causing
	there to be ChanServ as well as a BotServ bot in a frozen channel
	and duplicated output of fantasy commands.)  
* botserv/say_caller: Added two checks that were overlooked, thanks to
	@MrBenC and [ChatServices](https://bitbucket.org/chatlounge/chatservices/)  

chanserv
--------
* AKICK enforcement _(kick messages)_ have been updated to a more
	modern kickreason.  
	Was: "Banned: Reason" - Now:
	"(setter) Reason"  
* FREEZE command now un-assigns any BotServ bot that is assigned to
	the channel so that there are not two service bots in the frozen
	channel.  
* New config option in chanserv section called 'verifiedaccess', which  
	when enabled; only allows users who have completed account verification  
	(when 'auth' is set to email) to be able to be added to access lists or  
	given flags on channels.  
* chanserv/suspend: No longer refers to "lists", as there aren't any.  It's
	a flag _(+P)_.  

config
------
* References to KLINE have been changed to AKILL for better consistency.  
	This causes three (3) configuration changes:  
	* kline_time is now akill_time  
	* kline_with_ident is now akill_with_ident  
	* kline_verified_ident is now akill_verified_ident  
* A new config option 'vhost_change' has been added, allowing network staff
	to limit how often (in days) a user must wait between vHost changes
	such as use of TAKE or REQUEST command.  
	This causes one (1) additional configuration addition:  
	* vhost_change (in days)  
* A new config option 'verifiedaccess' was added to the chanserv section
	which (when enabled) only allows verified users to be given channel
	flags or access.  
	This causes one (1) additional configuration addition to the chanserv
	section:  
	* verifiedaccess;   (or #verifiedaccess; to disable)  

contrib  
-------  
* Help: Added observed missing help files for various contrib
	modules.  
* ns_ajoin: Fixed a bug in the NickServ AJOIN module that would
	allow adding erroneous channels to AJOIN. (instead of
	ensuring it is a valid channel name)  
* os_akillchan: Updated so that when a user joins an AKILLCHAN the
	user is AKILLed using @IP, making less redundant akills for
	the user@host and going with @IP instead.  (Previously would
	AKILL all user@hosts joining, which in the case of a botnet
	can result in many duplicate AKILLs that could have been solved
	with just 1 AKILL on the IP address.  
* os_akillnicklist: Updated so that when a user matches the AKILL
	criteria that the AKILL is sent using @IP to be more effective
	against bots/drones.  
* os_zlinechan: New contrib module that does the same as akillchan,
	but sets ZLINEs/DLINEs instead of AKILLs - managed via ZLINE
	management in OperServ. (LIST,ADD,DEL,etc.)   
* os_trace: updated to use the new akill_time config option.  
* ns_generatehash & ns_sendpassmail updated with new crypto
	improvements.  

core
----
* Added a VERIFICATION warning for un-verified users (when applicable)
	that reminds un-verified users that their account will expire
	within 24 hours of registration.  
* Updated cautions and warnings when users trigger Flood Protection  
	within Services, as well as the AKILL reason when the user/bot  
	ignores the first two notices.  

crypto  
------  
* Many improvements to crypto modules and security. (from upstream
	atheme group: most notably @amdj)  

database
--------
* opensex database improvements  
* Add a ratbox-services database conversion tool for Xtheme  

general
-------
* xtheme.conf.example: Various updates for added modules, missing info,  
	and maintenance.  **5 changes to xtheme.conf so far.**  

hostserv
--------
* New config option (vhost_change) allows Network Staff to limit how often a
	user may change their vHost without Staff intervention. (Helps deter
	ban evaders, etc.)  

memoserv
--------
* send: Read receipts now include the original memo text (thanks to
	[ChatServices](https://bitbucket.org/chatlounge/chatservices/) and @MrBenC )  
* send: fix format warnings during compile

nickserv  
--------  
* badmail: You now filter the LIST output by pattern.  Also, a new
	TEST subcommand was added to allow lookup of patterns that would
	prevent the use of an email address.  
* badmail: You can now REJECT _(traditional)_ or AKILL on badmail matches
	or hits.  _(Helpful to combat botnets calling NickServ REGISTER)_
* nickserv/info: Reveals a user's account _(if different than their nickname)_  


operserv
--------
* netlog: New module for logging/relaying network information (similar
	to Anope's LOGUSERS functionality) and logtype: NETLOG  
* info: Now refers to KLINEs as AKILLs for better consistency  
* info: Now shows vhost_change value as well as verifiedaccess value.  


protocol
--------
* chatircd protocol added (a modern Charybdis fork)
	https://bitbucket.org/chatlounge/chatircd/  
* UnrealIRCd 4: Advertise SASL mechanisms to UnrealIRCd servers.
	(From upstream atheme - Thanks @syzop)  


## STAY UP TO DATE WITH XthemeOrg NEWS, UPDATES & SECURITY ADVISORIES  
Subscribe to the XthemeOrg News at:  
https://xtheme.org/org-tools/newsletter/  


Xtheme Services 7.5.2 Development Notes
============================================

* This release addresses a CVE and is highly recommended to upgrade.
	!! All previous versions are affected by the CVE which is fixed
	in this release.  !!

proxyscan
---------
* dnsbl can now add a ZLINE on DNS Blacklist Hits and maintain it in
	OperServ.

operserv
--------
* Added a new ZLINE management system similar to the AKILL management
	system.  ZLINEs can now be organized and easily managed via
	Services
* ZLINEs placed through OperServ will be numbered [Z#1]
* AKILLs placed through OperServ will be numbered [AK#1]


sasl
----
* CVE FIXED sasl/main: free sasl_sourceinfo_t after use (thanks mniip)


## STAY UP TO DATE WITH XthemeOrg NEWS, UPDATES & SECURITY ADVISORIES
Subscribe to the XthemeOrg News at:
https://xtheme.org/org-tools/newsletter/


Xtheme Services 7.5.1 Development Notes
============================================
contrib
-------
* backtrace updated with Xtheme information and where to report issues
	to, etc.

core
----
* After 10 failed logins to a nickname, further attempts will result
	in WALLOPS warnings to network Staff. (No longer attempts automated
	AKILLs)  This fixes a crash in certain situations.  

crypto
------
* pbkdf2: remove obsolete compatibility function
* pbkdf2v2: remove obsolete compatibility function

general
-------
* A crash bug has been fixed as of the latest git.  Networks are advised to
	update immediately!

memoserv
--------
* Let users know (on return from AWAY or as they authenticate) that their
	MemoServ inbox is full.
* fix: unregister hooks when unloading MemoServ

protocol
--------
* inspircd: Added support for +U (inspircd-extras/m_opmoderated.cpp)
	(Similar to Charybdis/ircd-seven's +z moderation mode)



## STAY UP TO DATE WITH XthemeOrg NEWS, UPDATES & SECURITY ADVISORIES
Subscribe to the XthemeOrg News at:
https://xtheme.org/org-tools/newsletter/

Xtheme Services 7.5.0 Development Notes
============================================
chanfix
-------
* Fix from a previous commit (minor)

general
-------
* Added needoper clarification and set as default for ircop operclass.  This
	can optionally be commented out or removed from oper classes if you wish.
	(Such as if you have a Helper operclass with limited privs, etc)

nickserv
--------
* LASTLOGIN will now be displayed along with successful authentication
	notice can now be optionally disabled per user with SET HIDELASTLOGIN



## STAY UP TO DATE WITH XthemeOrg NEWS, UPDATES & SECURITY ADVISORIES
Subscribe to the XthemeOrg News at:
https://xtheme.org/org-tools/newsletter/

Xtheme Services 7.5.0-rc1 Development Notes
============================================
chanfix
-------
* Fixed a bug where certain channels were not being fixed (even without
	NOFIX set) and if attempted to be manually fixed, would run into a
	mode removal loop instead of fixing the channel.

chanserv
--------
* INFO command will now indicate (if the channel is not private) the ANTIFLOOD
	response or reaction. (if enabled)
* MASS command and functions added. (Halfop, Op, Owner, Protect, Voice)
	(Closes XthemeOrg/Xtheme#11)
* Introducing FSUSPEND (not enabled by default) which allows Services Operators
	to suspend any user's channel access (even Founders)

contrib
-------
* cs_fregister now accepts a optional Founder, otherwise will register to the
	Services Operator using the ChanServ FREGISTER command as it did before.
* dnsbl removed - it was broken and networks should be using the core proxyscan/dnsbl
	module instead.
* os_akillchan: updated to include the service bot issuing the automated AKILL instead
	of "*".
* os_akillnicklist: updated to include the service bot issuing the automated AKILL
	instead of "*".
* os_defcon: updated to include the service bot issuing the automated AKILL instead of
	"*".
* AdminServ removed from contrib, but still available on XthemeOrg GitHub for users
	that desire to use it.
* ns_forbid: Fix so forbidden nicks show up as forbidden in NickServ LIST instead of
	frozen.

core
----
* SOPER and SOPER:AF (Authentication Failure) will now report the operclass
	names in messages.  This helps increase visibility for network staff.

general
-------
* move stray memory calls to memory wrapper

helpserv
--------
* helpserv/ticket: integration of close reason

hostserv
--------
* hostserv/request: Will no longer allow duplicate vhosts to be requested.  User will
	be told to request a different vhost or seek network staff.

nickserv
--------
* Added FORBIDDEN to NickServ LIST criteria and output
* Added mention of CERT CLEAR in help file
* nickserv/multimark: Fixes for assertion failure and patricia warning

operserv
--------
* CLEARCHAN with AKILL option will now show the Services Operator performing
	the command in the AKILL list instead of "*"
* AKILL now shows the number of matching users affected by the set AKILL as
	well as any matches/hits to the operator issuing the command.

proxyscan
---------
* The DNSBL module has been fixed (so that it is working and stable) by
	ilbelkyr (shalture/shalture)  Additional reviews and fixes were made
	to the module after extensive testing.
* DNSBL exemptions are now properly saved and do not cause issues with startup
	as of latest git.
* DNSBL Hits (with KLINE option) will be set for 14 Days for more effectiveness.
	(This may be changed to the xtheme.conf default in time.)
* Example config for Proxyscan/dnsbl added to xtheme.conf.example [Requires xtheme.conf update]
* DNSBL Hits (with KLINE option) will now show the DNSBL the user was listed in as the
	AKILLer in AKILL LIST FULL.



## STAY UP TO DATE WITH XthemeOrg NEWS, UPDATES & SECURITY ADVISORIES
Subscribe to the XthemeOrg News at:
https://xtheme.org/org-tools/newsletter/

Xtheme Services 7.4.3 Development Notes
=====================================
botserv
-------
* botserv/bottalk: (Optionally) give the command caller's nickname.
* botserv/set_saycaller: Command and Helpfile added for optionally
	giving the command caller's nickname in the message. (Requested upstream
	in atheme/atheme#487)
* botserv/info: Show whether channels have SAYCALLER set or not

chanfix
-------
* chanfix/fix: Stay in channel after the fix if the channel being fixed
	is the Services log channel. (Bug reported upstream in atheme/atheme#486)

chanserv
--------
* hold: record services operator that held channel
* info: reveal services operator that held channel (*NOTE: Previous HOLDs
	will not reveal services operator that set the HOLD as it was not
	recorded until Xtheme 7.4.3)

core
----
* Libmowgli-2 updated to most recent code

memoserv
--------
* Remove an extra 'from' from email notices about memos

nickserv
--------
* hold: record services operator that held account
* info: reveal services operator that held account (*NOTE: Previous HOLDs
	will not reveal services operator that set the HOLD as it was not
	recorded until Xtheme 7.4.3)

general
-------
* Move vendor field to a compile time macro
* README was updated as SWI has moved back to XthemeOrg
* botserv/set_saycaller was added to the xtheme.conf.example module list

operserv
--------
* OUTPUT of SPECS has been clarified better

protocols
---------
* protocols/unreal*: Fix wrong source for SVSNICK


## STAY UP TO DATE WITH XthemeOrg NEWS, UPDATES & SECURITY ADVISORIES
Subscribe to the XthemeOrg News at:
https://xtheme.org/org-tools/newsletter/

## NOTE
! Atheme Development has resumed.  Xtheme development will continue up until
	Atheme has caught up as far as bug fixes and new features.  For more info:
	http://www.Xtheme.org/


Xtheme Services 7.4.2 Development Notes
=====================================
chanserv
--------
* flags: Make Anope FLAGS compatibility an option
* set_*: announce changes via verbose()
* antiflood: Change antiflood_msg_count to size_t
* ftransfer: call myentity_allow_foundership
* clone: Do not copy certain metadata such as HOLDs or
	ANTIFLOOD AKILL, but copy MARKs to avoid evasion.

memoserv
--------
* Remove an extra 'from' from email notices about memos

nickserv
--------
* enforce: Prevent REGAINing reserved nicknames
* cert: Add CLEAR command for users who want to clear all certs
	or cannot clear their own certs.

core
----
* After 10 failed logins to a nickname, further attempts will result
	in a 1 hour AKILL. (Prevents brute-force style attacks with the
	IDENTIFY command)
* Fix issue where pretty_mask would return host!*@*
* flags: update_chanacs_flags(): do not assume that a protocol module
	is loaded

general
-------
* General Bug Fixes from upstream were the primary reason for this
	release.
* xmlrpc: Do not copy more bytes than were allocated
* automake: Fix typo in compiler flag
* Move away from BSD data types

operserv
--------
* OUTPUT of SPECS has been clarified better

protocol
--------
* ts6-generic: add support for sending mechlists
* inspircd: Only set hideoper mode on oper pseudoclients


! Atheme Development has resumed.  Xtheme development will continue up until
	Atheme has caught up as far as bug fixes and new features.  For more info:
	http://www.Xtheme.org/


Xtheme Services 7.4.1 Development Notes
=====================================
chanserv
--------
* Fix for removing old suspend metadata once a suspension expires.
* Added ability for channel Founders (+F users) to delete a
	suspension affecting them.
	* NOTE: Checks are already in place to ensure Founders cannot
		be suspended.
* Fix for SUSPEND - Remove halfops status (if applicable) as well,
	and added checks to ensure another user cannot simply ask
	ChanServ to give the suspended user halfop status.

Xtheme goes sleeper
-------------------
! This will be the final release of Xtheme (excluding bug fixes)
	since the maintainers of Shalture, Zohlai and Xtheme have
	formed the new Atheme Group.  We will now be focusing our
	efforts to bring the bug fixes from our forks over to Atheme
	as well as (some) of the new features.  Feel free to request
	new Xtheme features that you enjoyed on the atheme GitHub
	Issue tracker.

More information on Atheme can be found at:
	* https://atheme.github.io/
	* https://github.com/atheme/atheme/


Xtheme Services 7.4.0 Development Notes
=====================================
general
-------
* A lot of testing, bug fixes and code cleanups.


Xtheme Services 7.4.0alpha-2 Development Notes
=====================================
chanserv
--------
* SUSPEND ADD/DEL (flag +P) added for ability to suspend channel
	users access or privileges in a channel.  Suspensions can be
	timed or permanent. (!T expiry or !P)
* SUSPEND Help file added
* SUSPEND (+P) added to HELP FLAGS output (when applicable)
* SUSPEND information added to output in ChanServ WHY and
	ChanServ ACCESS INFO.
* Fixes for WHY and ACCESS INFO output giving incorrect information
	in output for PERMANENT (or non-expiring) suspensions.
* A lot of module header updates


Xtheme Services 7.4.0alpha-1 Development Notes
=====================================
contrib
-------
* dnsbl, os_akillnicklist, os_defcon have been updated to add AKILL
	hits to DB for easier management.

nickserv
--------
* LASTLOGIN will now be displayed along with successful authentication
	notice.

general
-------
* Since this release introduces various fixes and a change to core,
	the development version has been bumped to 7.4.0.  Releases of
	v7.4.0 will begin in alpha stages. (beginning with 7.4.0-alpha1)


Xtheme Services 7.3.6 Development Notes
=====================================
chanfix
-------
* LIST -- Added MARKed channels to ChanFix LIST output. (Now ChanFix
	LIST will show channels and indicate if they are NOFIXed or MARKed)

release
-------
* Release skipped, production of v7.4.0 started instead.

Xtheme Services 7.3.5 Development Notes
=====================================
botserv
-------
* Added checks for FROZEN channels, disabling botserv commands for
	frozen channels.

chanfix
-------
* Added LIST command (List ChanFix channels matching given pattern)
* Added checks for NOFIX'd channels in LIST command and will add
	NOFIX to output for LIST.
* Added NOFIX command (Excludes channels from automated and manual
	chanfixes.)

chanserv
--------
* Fixed bug in chanserv/moderate where Services Operators were opped
	in ACTIVATE'd channels instead of the actual Founder.
* Add +e as a defualt flag for Channel Founders to match templates.
* Small fix to LIST output so that closed channels are not listed
	with/as frozen channels.

operserv
--------
* Added CLEARCHAN AKILL hits to AKILL DB for easier management.

contrib
-------
* Added contrib/os_akillchan AKILL hits to AKILL DB for easier
	management.


Xtheme Services 7.3.4 Development Notes
=====================================
chanserv
--------
* Fixed paths to HELP files for ChanServ MODERATE (ACTIVATE, REJECT,
	WAITING) so that they actually display.
* Clarify in GROUPMEMO's that a channel is waiting to be reviewed

hostserv
--------
* Clarify in GROUPMEMO's that a vhost is waiting to be reviewed
* Clarify HELP for HostServ DROP (thanks GLolol)

operserv
--------
* Added SETTIME to AKILL LIST (shows time AKILL was originally set
	(closes #27)

protocols
---------
* Added support for UnrealIRCd 4
* Added support for Charybdis channel mode +T (Block Channel Notices)
* Added support for Charybdis channel mode +M (Oper Immune)


Xtheme Services 7.3.3 Development Notes
=====================================
chanserv
--------
* Add missing HELP files for ACTIVATE and REJECT (for channels using
	module chanserv/moderate)
* Add BLOCKBADWORDS help files and reference BLOCKBADWORDSOPS, from
	upstream.
* Added REGGROUP option (turned off by default) for Channel Request
	memo's to be sent to the appropriate services team members to
	the Xtheme example config. (For networks using chanserv/moderate)
* Bug Fix for ChanServ FREEZE which makes ChanServ leave the channel
	after it is unfrozen (unless GUARD is set)

hostserv
--------
* added ability to send Group Memos for HostServ requests, allowing
	proper services staff to be notified via Memo of vHost requests.
* Added REGGROUP option (turned off by default) for vHost Request
	memo's to be sent to the appropriate services team members to
	the Xtheme example config. (For all networks allowing requests)
* added DROP command allowing users to drop their assigned vhosts
	without the need for Staff Intervention.


nickserv
--------
* removed CERT IDENTIFY, as SASL REAUTH would be a better solution.
* ensure upon REGAIN that the nick is valid

saslserv
--------
* various updates and bug fixes from upstream

config
------
* clarification added for various features admins may not have been
	previously aware of.
* add +q to default Founder flags (thanks GLolol)


Xtheme Services 7.3.2 Development Notes
=====================================
general
-------
* bug fixes (this release is generally bug fixes)

nickserv
--------
* added CERT IDENTIFY
* bug with SENDPASS fixed

chanserv
--------
* fixed bug in ChanServ MODERATE where channels ACTIVATEd were not joined by
	ChanServ if the Founder gets disconnected or logs out.
* SET FOUNDER is back (and a bug introduced by it's return is fixed)


Xtheme Services 7.3.1 Development Notes
=====================================
general
-------
* added Xtheme packages for downloading Xtheme instead of using git or GitHub
* corrected path to xtheme.motd, so /motd services.network.org works
* update to pbkdf2v2 crypto module saving CPU cycles

chanserv
--------
* added ChanServ CLEAR OPS (and Help file)
* ChanServ now shows WHO last modified a user's flags, as well as how long ago


Xtheme Services 7.3.0 Development Notes
=====================================
general
-------
* Atheme has decided to end development.  The Xtheme Group forked Atheme to
	create Xtheme IRC Services!
* added Help file for FREEZE command
* added Help file for GroupServ/SET GROUPNAME (reported missing in
	#Atheme on Freenode to Atheme group)
* changed command actions from klines to akill, however core functionality is still
	mostly the same to maintain compatibility with networks that were using either
	Atheme or a fork.

chanserv
--------
* added FREEZE command (Similar to CLOSE, but allows a channel to remain
	open, but all access to it "suspended".
* Make `NOSYNC` show in ChanServ INFO output  

operserv
--------
* Change KLINE messages to AKILL to be more appropriate with todays most
	commonly used IRCds.
* "Ignoring attempt from..." wallops from OperServ have been turned into a
	log event instead.

help files
----------
- Help files have been updated to reference Xtheme, instead of Atheme.


Atheme Services 7.2.5 Development Notes
=====================================

nickserv
--------
- Make `VHOST` set cloak assigner and timestamp the same way HostServ does
- Make `INFO` call the `user_info_noexist` hook for queries that don't match an account
- Make `REGAIN` log you in if successful.
- Allow implementing custom filters for `LIST`
- nickserv/multimark: new module which allows multiple MARK entries per nickname.

chanserv
--------
- Add a `$server:` exttarget accepting server masks
- Add `PUBACL` flag which allows the channel access to be public.
- Don't allow `DEOP` or `KICK` of a services bot.
- Don't try to expand extbans in various commands.
- Allow users with +O or +V flags to op/voice themselves, since they can regain op/voice
  by cycling the channel anyway.
- chanserv/clear_akicks: new module providing a `CLEAR AKICKS` command.

gameserv
--------
- gameserv/dice: make the maximum roll count configurable.

groupserv
---------
- Hook into `sasl_may_impersonate` to support group-membership checks
- groupserv/set_groupname: new module allowing renaming a groupserv group

helpserv
--------
- helpserv/ticket: optionally accept a close reason and send a memo to an offline user

operserv
--------
- operserv/rwatch: allow creation of RWATCH rules which k-line if 'K' is a modifier on the
  provided regexp.

saslserv
--------
- Add support for SASL authorization identities
- Add a `sasl_may_impersonate` hook
- The DH-AES and DH-BLOWFISH mechanisms were removed in their entirety.
- Add support for IRCv3.2-draft SASL mechanism list caching, implemented by InspIRCd 2.2.

alis
----
- Add a `list ... -showsecret` flag (chan:auspex) to list secret channels

perl api
--------
- Export SaslServ's `sasl_may_impersonate` hook

ircd protocol
-------------
- Add user flag for tracking external services clients
- inspircd: Hopefully fix ignored account names when linking to the network
- inspircd: Various improvements to InspIRCd 2.0 support
- inspircd: Remove InspIRCd 1.2 and 2.1beta support
- inspircd: Add support for rejoindelay property in InspIRCd 2.2
- inspircd: Change the opertype used from 'Services' to 'Service'
- ircnet: Implement oper-wallops, using individual notices
- ngircd: Enable +qaohv support
- ngircd: Ignore non-# channels for now
- ngircd: Implement oper-wallops, using individual notices
- unreal: Request MLOCK messages when linking to the network
- sporksircd: Nuke obsolete module

other
-----
- various: Fix quite a few resource leaks and possible null derefs
- crypto/pbkdf2: Detect malformed (truncated) hashes
- contrib/cap_sasl.pl: Import various fixes from freenode's v1.5
- contrib/cap_sasl.pl: Implement SASL EXTERNAL, ECDSA-NIST256P-CHALLENGE
- contrib/cap_sasl.pl: Fix crash if irssi has ICB or SILC plugins loaded
- contrib/cap_sasl.pl: Fix crash if disconnected while waiting for SASL reply
- transport/jsonrpc: new module implementing JSONRPC transport

crypto
------
- pbkdf2v2: Newer module implementing PBKDF2-HMAC digest scheme
            with backward compatibility and limited forward compatibility

Atheme Services 7.1 Release Notes
=================================
In addition to assorted bugfixes in various subsystems from 7.0, the
following changes have been introduced in 7.1.

ircd protocol
-------------
- ngircd: New protocol module.
- nefarious: Add Nefarious 2 SASL support.
- nefarious: Send account timestamp in svslogin.
- elemental-ircd: New protocol module.
- dreamforge: Remove protocol module.
- inspircd: Add support for server-side MLOCK and TOPICLOCK enforcement
- inspircd: Add support for matching extbans modifying matching logic
- inspircd: Add +H to channel modes
- inspircd: Add +X and +w to list-like mode list
- ircd-seven: Support charybdis extension cmodes on ircd-seven as well.
- ts6-generic: Add support for serverinfo::hidden
- unreal: Add support for extbans.
- unreal: Add cmode +P for permanent channel.

buildsys
--------
- MacOS 10.5 required for OS X builds.
- V=1 option to make for verbose output.
- Allow parallel building, i.e. with -j option.
- Dependencies tracked on a per-sourceunit basis
- Allow --disable-rpath to modify buildsys param LDFLAGS_RPATH
- Install default email templates
- Add --with(out)-libmowgli to force use of internal mowgli

chanserv
--------
- antiflood: New module to react to channel flooding
- quiet: Channel statuses are removed from the target user to ensure
  that the quiet takes effect.
- quiet: Allow unquieting improper masks on the quiet list.
- quiet: Notify target user when anything changes about them.
- quiet: Honor protected mode like with kick/kickban.
- quiet: Support IRCDs with quiet extbans like UnrealIRCd and InspIRCd.
- flags: New exempt flag +e, split from +r. Databases should be upgraded
  automatically.
- flags: Require FORCE argument and chan:auspex to oper override.
- flags: Allow users with +f and +o (+v) to set +-O (+-V) on self.
- access: Do not allow changing +F via ROLE command.
- Support multiple users as arguments for owner, op, halfop, voice,
  and quiet.

nickserv
--------
- sendpass: Accept grouped nicks.
- register: Allow any number of emailexempts.
- Do net send 'spam' notice if chanserv does not exist.
- Add confirmation for badmail:del
- listemail: Match on canonical addresses too
- info: Show setpass to services admins with user:auspex
- info_lastquit: New module to show last quit message in INFO
- resetpass: Allow specifying any grouped nickname.
- drop: Request confirmation when dropping an account.
- access: Allow TLDs
- Log sendpass sender and time
- Show entity ID in 'ACC' and 'INFO' commands.

groupserv
---------
- Restrict +f from +F-ing themselves
- Prevent +f-F from removing founders
- Prevent removing last founder of a group
- Make sure +F always have +f
- Notify users when they are invited to a group.

sasl
----
- Add ecdsa-nist256p-challenge mechanism
- Add dh-aes scheme, intended to replace dh-blowfish.
- Disable reload capability on all modules.

perl api
--------
- Add function to return entity ID
- Allow sending wallops
- Allow setting vhosts
- Allow transferring and dropping channels
- Change myuser_find to myuser_find_ext to allow lookups by UID.
- Add config.xs to retrieve config values from the Perl API
- Add functions to channel.xs to register a channel and to retrieve a
  limit, key, and ts.
- Allow channelregistration.xs to get/set flags and get used time
- Add registration and last seen time in account.xs

email
-----
- Put the network name in the subject field of outgoing emails.
- Add a module canonicalizing gmail addresses.
- Use canonical email addresses when checking for registration limits.

libathemecore
-------------
- Allow different send and receive passwords for uplinks
- Respect founder_flags config setting during channel succession
- Denote default crypt provider in version output.
- Include reason with kline expiration messages.
- Allow customization of the address for email from services.
- Add option to kline user@host instead of *@host
- Add qrcode API

botserv
-------
- Blacklist '/' from various fields.
- Monkeypatch notice() to rewrite source from chanserv to botserv.

crypto
------
- Rename 'fallback' crypt provider to 'plaintext'
- Allow crypto modules to be loaded and the database to be updated to
  the preferred crypto scheme on the fly.
- pbkdf2: New module implementing PBKDF2-HMAC digest scheme.

misc
----
- xmlrpc: Add metadata accessor
- security/cmdperm: New module which dynamically infers virtual
  permissions, such as command:chanserv:register
- alis: Strip mIRC color/control codes from topics.
- operserv/clones: Add option to give a few warning kills before applying
  a k-line
- Codebase is stringref clean (GitHub issue #60)
- memoserv/delete: Only accept numeric indexes.
- chanfix: Allow admins with chan:admin to register regardless of
  chanfix score.
- memoserv: Make inbox size customizable.
- Add dragon, a new, modular, ircd link performance benchmarking toolkit.
- Flood k-lines use IP address where available instead of hostname.
- Add !snotices and !wallops logging targets.
- Record vHost assigner and timestamp, and display in NS INFO output.
- Contrib modules have their own git repo.
- Add a git .mailmap
- gameserv/dice: Ensure loop paramaters are integers limited to 1000

atheme.conf
-----------
Be sure to check atheme.conf.example for more information on what each
of these settings does.
- Add 'registeremail' setting to serverinfo{}, specifying address that
  services emails should originate from.
- Add 'hidden' setting to serverinfo{}, specifying that the services server
  should be hidden in /links output (limited to some ircds).
- Split 'password' setting in uplink{} into 'send_password' and
  'receive_password' (optional).
- Move 'maxnicks' setting from serverinfo{} to nickserv{}
- Move 'maxchans' setting from serverinfo{} to chanserv{}
- Add 'antiflood_enforce_method' to chanserv{} for chanserv/antiflood
- Add 'maxmemos' setting to memoserv{}
- Add !snotices and !wallops logfiles
- Add 'permissive_mode' setting to general{}, specifying manner of
  command denials.
- Add 'kline_with_ident' and 'kline_verified_ident' to general{}
- Add 'binddn' and 'bindauth' conf items to ldap{}
- Document "user" operclass.

Atheme Services 7.0 Release Notes
=================================
All bugfixes from the 6.0 branch of Atheme are also in 7.0.

dbverify
--------
- New utility.  Performs extensive and complicated consistency checks
  on your OpenSEX object store.  It can find things like:
  - corrupt AKICK entries (AKICKs with other flags/metadata that shouldn't be there);
  - duplicate channel ACL entries;
  - entity ID collisions
  It can find other stuff too, and will be expanded upon in the future.
  Think of it like a `fsck(1)` for your object store.

ircd protocol
-------------
- bahamut: add experimental support for bahamut-2.0 NICKIPSTR
  capability.
- charybdis: Add support for locking of modes provided by
  extensions modules.
- unreal: Add support for changets.
- inspircd: Add support for locking the +H channel mode.
- ithildin, bircd, plexus and ptlink protocol modules removed.
- inspircd: Users are now warned when they attempt to link on a client
  port instead of a server port.
- unreal: Add SASL support.
- unreal: Implement full support for mlocking +f.

chanfix
-------
- New service. Similar to EFNet's chanfix service.

chanserv
--------
- sync: New module based on cs_sync from contrib. Adds autosync on
  ACL change (and the ability to turn it off).
- channel entrymsgs are now displayed in INFO.
- akick: Support added for timed AKICKs.
- ban, quiet and akick: Atheme now fills in the parts of a hostmask
  that are missing with these commands.
- access: Various cleanups.
- cs_access_alias: New contrib module. Allows level-style pseudo
  access lists.
- clone: New module allowing you to clone a channel's access list,
  flags and metadata to a new channel.
- cs_badwords: New contrib module. Allows channel staff to specify a
  badwords list for a channel and what action to take when a user
  says one of the words in the channel.
- moderate: New module allowing operators with PRIV_CHAN_ADMIN to moderate
  channel registrations.  This is especially useful in combination with
  chanfix.  It is also useful in maintaining a standard of content correctness
  for specialized chat systems.

exttarget
---------
- exttarget/main: a new framework has been added which extends the
  entity subsystem further, allowing for entities to be dynamically
  constructed with the purpose of matching against any kind of user
  or account attribute in channel access lists.  these targets can
  take optional parameters.
- exttarget/oper: $oper extended target added.  this target allows you
  to match against all opers on the network in channel access lists.
- exttarget/registered: $registered extended target added.  this target
  matches anyone who is logged into services.
- exttarget/channel: $channel extended target added.  this target allows
  you to match anyone who is on a channel.

groupserv
---------
- all groupserv commands are now modules. Your atheme.conf will need
  to be updated for this change if you use groupserv.
- add join_flags config option and SET JOINFLAGS command. These allow
  changing the group flags a new user will get upon JOINing the group.
- add the +b (ban) flag. This prevents accounts matching it from JOINing
  the group.
- fflags: New command. Allows services operators to force a flags change
  on a group they they do not have access to.
- list: Allow refining the list with a pattern.
- listchans: New command. Allows group members with the +c flag to see all
  channels that group has access in.
- honor user:regnolimit permission in relation to the maximum number of groups
  a user may register. (SRV-125)

gameserv
--------
- many refactorings
- calc: new command. Allows doing basic math with GameServ.
- gs_roulette: New contrib module. A game of Russian Roulette.
- lottery: New module that randomly chooses one user out of the channel
  members.
- happyfarm: New (skeleton) module that's a game like FarmVille! But on IRC!

hostserv
--------
- added a new host_request hook to catch and do other things with host requests.
- reject: Add a optional reason parameter that will be memoed to the user with the
  rejection notice.

memoserv
--------
- ms_fsend: new contrib module. Allows sopers to override a target user being
  set NOMEMO or having the source user on ignore.

nickserv
--------
- restrict: New module that allows services opers to stop users from using
  commands that can be abused (hostserv/request, hostserv/take,
  groupserv/register, etc)
- emailexempts: New config option. Lets you specify email addresses that have
  no limit to the number of accounts they can have registered.
- when logging into a new account, users are informed that they will be logged
  out of their old account.
- when doing RELEASE or REGAIN against a user logged into an account, log
  them out of the account.
- old Atheme-1.x-style external logout implemented. Allows logging another user
  logged into your account out remotely.
- listgroups: New module that shows you which groups you have access in.
- nevergroup: New module that prevents anyone giving you access to a group.
- badmail: New module which allows setting email addresses (or glob patterns)
  which are not allowed to register accounts on-the-fly.
- nickserv now allows passwords longer than 32 characters if the database is
  being hashed.
- subscribe: Removed as it had many flaws and no one used it.
- ns_cleannick: new contrib module. Forces a nick change on a user if their nick is
  'lame' using case normalisation.

operserv
--------
- emailexempts and autokline exempts are now shown in INFO.
- modreload now rehashes the config if the module requires it and reloads modules
  that depend on the specified module.
- clones: Many cleanups.
- clones: Added an option to variable increase the clone limit if a users' clones
  are identified.
- soper: Allow adding a new SOPER with a password (optional, of course).
- set: Adds the ability to temporarily modify some config options on-the-fly.
- info: Add a new hooks so modules that add config options can also add lines to
  the operserv/info output.
- os_modeall: New contrib module. Allows setting a given mode on all channels.
- os_joinmon: New contrib module. Facilitates monitoring certain users and when
  a monitored user joins a channel, that information will be sent to the services
  log channel.
- os_resolve: New contrib module for testing the asynchronous DNS resolver.
- the RWATCH database is now serialized as opensex entities.
- specs: add support for groupserv-related permissions and clarify meanings of
  the various 'auspex' privileges. (SRV-125)

proxyscan
---------
- New service. Currently implements only a DNSBL scanning module.

rpgserv
-------
- New service. For finding and joining RP games on an IRC network.

scripting
---------
- Support for scripting Atheme in Perl added. Perl scripts are loaded with
  OperServ MODLOAD just like modules. Still in alpha. Add the --with-perl configure
  switch to enable it. POD-style documentation for the perl API is in doc/perl/.

statserv
--------
- New service. For querying for statistics about the network.

xmlrpc
------
- moved to transport/xmlrpc . Your atheme.conf will need to be updated for this
  change if you use xmlrpc.
- bad_password() is now called on invalid XMLRPC logins.

code
----
- libmowgli-2 is now required instead of libmowgli.
- a bit of the signal code and linker code was converted to use the mowgli
  implementations.
- charybdis' asynchronous DNS resolver added.
- mowgli.global_storage can now be used to make a module's data persistent on
  module reload. It is currently only used in GroupServ.
- many assertions added in various places throughout the code.
- added a new AC_AUTHENTICATED pseudo-priv to replace many identical checks if
  a user is logged in throughout the code.
- irc parse/uplink state has been made modular.
- atheme core has been changed to build as a library.
- all the old SNOOP channel code has been removed. SNOOP has been deprecated since
  5.1 and gone since 5.2.
- MODULE_USE_SYMBOL() was removed in favour of MODULE_TRY_REQUEST_SYMBOL().
- most service-specific (config file) code split out from the core.
- configuration-defined usernames are now truncated at USERLEN (10 characters).
- UID generation split out from the core.
- module_load can now be hooked into. This is particularly useful for scripting
  modules.
- entities now have unique IDs.  unique IDs may be referenced in all XMLRPC and
  IRC commands.
- strlcpy()/strlcat() have been replaced with mowgli implementations.
- atheme.string has been replaced with mowgli.string.
- add new hook_channel_acl_req_t structure for channel_acl_change hook, which is
  intended to describe ACL changes more effectively.
- call shutdown(2) on sockets being closed to help some TCP stacks be more aggressive
  when closing sockets.
- use mowgli_eventloop_pollable instead of old eventloop code.
- Windows is now supported.

other
-----
- ensure buffers passed to strftime() are large enough to fit the entire string.
  strftime() is not really required to behave in any specific way in the event of
  buffer overflow.
- ircd_announceserv: New contrib service. This allows users to request network
  announcements (which sopers must approve before they're sent).
- an access {} config block was added allowing rewriting of command privs.
  If specified, the user must match the original priv and the rewritten priv.
- allow Atheme datadir to be specified on the command-line when starting.
- many improvements to the LDAP authentication module.
- general::immune_level config option added. This allows customising the operlevel
  that gets kick immunity privileges.
- DNS Blacklist scanning module added. This module will scan connecting users
  against a list of DNS blacklists and take action if the users' IP is in one
  of the blacklists. This module is mainly managed through operserv.
- allow SASL authentication for any nick linked to the account, not just the
  accountname.

Atheme Services 6.0 Release Notes
=================================
All bugfixes from the 5.2 branch of Atheme are also in 6.0.

ircd protocol
-------------
- inspircd: Support for owner, halfops and admin are now dynamically
  enabled by what modes exist instead of being enabled by what modules
  you have loaded in inspircd.
- support for InspIRCd 1.1, OfficeIRC and UltimateIRCd 3 has been removed.

opensex
-------
- opensex is now the required database format. All flatfile will do is
  convert your flatfile database to opensex and exit.
- converted many modules that use external databases to using opensex.

chanserv
--------
- new module: chanserv/access.  this adds role-based channel acl via the
  ACCESS and ROLE commands.
- new module: chanserv/successor_acl.  this adds a +S channel acl flag which
  will weight a user as a successor.
- modules may now override the succession process using the new
  channel_pick_successor hook.
- chanserv/list: Enhance by adding many possible criteria to match channels
  against.
- new set_prefix module. This module allows channels to define a channel-specific
  fantasy prefix. The channel-specific prefix is displayed in the INFO for the
  channel. This is particularly useful if the channel uses an external bot that
  conflicts with the services default fantasy prefix.
- new clear_flags module. This allows founders to remove all entries from the
  channel access list except other founders.

groupserv
---------
- new service that allows users to form groups of accounts and apply the
  same ACL entries to them, send memos to them and other features.

helpserv
--------
- new service that allows users to request oper help in different ways.
  Currently either via a ticket system or by "pinging" the opers with a
  request for help.

hostserv
--------
- allow activating or rejecting all waiting vhosts by using '*' instead of
  a nick.
infoserv
--------
- oper-only message support. You can now give messages an importance where
  they will only be sent to opers upon oper-up.
- in infoserv message subjects, underscores will now be replaced with spaces
  so you can have multi-word subjects.
- allow customizing the number of infoserv messages shown to users on connect.

nickserv
--------
- new contrib module, ns_waitreg that allows you to specify how long a user must
  be connected before they can register a nick.
- new regnolimit module. Allows opers to set users as able to be exempt from channel
  registration limits. (how many channels may be registered to one account)
- nickserv/list: Enhance by adding many possible criteria to match users against.

operserv
--------
- new readonly module. This allows changing the readonly state at runtime.

xmlrpc
------
- the legacy xmlrpc/account, xmlrpc/channel and xmlrpc/memo modules have been
  removed. These have been deprecated for over 4 years and you should be
  using xmlrpc/main and atheme.command for all your xmlrpc uses.
- the xmlrpc core has been rewritten a little bit to use mowgli's patricia tree
  code.  this should bring a performance improvement over the hashtable code it
  was using.
- xmlrpc has been completely moved out of core
- a new command, atheme.privset has been added to get the soper privs of a user.

code
----
- default values in config options are now supported. This is particularly
  useful in modules and cleans up the config code a bit.
- many bugfixes and compile warning fixes.
- the flags code has been cleaned up to assume that there is only one flags
  table.
- the flags code is now extendable by modules.
- mychan_pick_candidate() is now in the public API.
- the core now lives in an ipv6 world.  it's 2010 - if your operating system
  doesn't support ipv6 - you suck.
- ctcp handling has been rewritten.
- new easter egg.
- the shrike hash function (shash()) has been removed as there was no longer
  anything using it.
- the "symbolmatrix" code was removed because we went with a different solution
  instead long ago.
- myuser_t is now a child of myentity_t which describes an entity that can have
  channel membership.
- list_t/node_t have been removed in preference of mowgli.list.
- balloc has been removed in preference of mowgli.heap.

other
-----
- added an anope 1.9.2 flatfile DB to OpenSEX DB conversion script.
- mail sending has been changed, likely causing serverinfo::mta scripts to
  break. The command is now passed "-t" rather than the email address and
  the shell is no longer used.
- the SDK hg revision of modules in now shown in MODINSPECT.

Atheme Services 5.2 Release Notes
=================================
Note: We are looking for additional developers to help with maintenance of
Services.  After almost 7 years of development, many of the programmers have
moved on.

ircd protocol
-------------
- inspircd: track channelmodes +D (delayjoin) and +d (delaymsg).

chanserv
--------
- split out SET into seperate modules for each SET command. chanserv/set
  is now a "meta-module" that depends on all the set_* modules.

hostserv
--------
- added OFFER module that allows opers to offer vhosts to users.
- made the request system (specifically the ACTIVATE command) not send
  a memo to the user.

infoserv
--------
- new service. infoserv allows opers to send notices to users when they
  connect or at the time of running the command (like Global).

nickserv
--------
- split out SET into seperate modules for each SET command. nickserv/set
  is now a "meta-module" that depends on all the set_* modules.
- added cracklib module that checks users' passwords on REGISTER and lets
  them know if the password is secure or not. You can have it just warn
  the user or disallow them from registering with a configuration option.
- added ns_generatehash contrib module to generate a password hash for
  a soper if you have crypto enabled.
- removed ns_ratelimitreg contrib modules as its functionality is now in
  core.

operserv
--------
- added expiry time to clone exempt

code
----
- replace the atheme-services build system with the ACBS used by many other
  Atheme projects.
- rework the colour and special character stripping for xmlrpc.
- remove snoop(). any modules still using snoop() will fail to compile on
  atheme 5.2. please replace it in your code with logcommand() or slog().

other
-----
- ircservtoatheme: generally make a bit more robust.
- added ratelimiting support to hostserv/request, chanserv/register and
  nickserv/register.
- add a new database format called opensex.  This is available in 5.2 as a
  "technology preview" and will be mandatory in 6.0.

Atheme Services 5.1.1 Release Notes
===================================

ircd protocol
-------------
- TS6: Rework MLOCK a bit to make it more robust and support more modes.

operserv
--------
- add os_helpme contrib module. Thist module marks a user as a network helper.
  This will only work on ircd's with the helpop (usually +h) user mode.

other
-----
- add extends directive to operclasses so one operclass can inherit privledges
  from another. See the example config for details. Note, you can have two
  operclasses with the same privledges, so extending is not forced.

Atheme Services 5.1 Release Notes
=================================
### [MERGED] indicates items merged to the 5.0 branch

ircd protocol
-------------
- inspircd: common code has been merged into inspircd-aux, this will continue
  in the next version with inspircd 1.1 support.
- inspircd: several unsupported module configurations are now programatically
  marked as such.
- inspircd: permanent channels are now tracked in 1.2 and later. (SRV-29)
- inspircd: add support for receiving SVSNICK (nick collisions)
- inspircd: add support for m_ojoin
- TS6: add support for MLOCK
- shadowircd: updated module to shadowircd6
- hyperion: removed
- Added support for ithildin1. This is still a bit experimental.
- ircnet: support added for server hostmasking.

botserv
-------
- add missing helpfiles
- botserv bots now quit instead of splitting when terminating/restarting
  services (SRV-12)

chanserv
--------
- FLAGS: allow +F* as well as +*F

hostserv
--------
- add missing helpfiles

nickserv
--------
- add support for CERTFP (CERT command)

operserv
--------
- reject jupes with names containing wildcards.
- add os_trace contrib module. This module looks up users by various criteria
  and lets you perform actions on them.
- add os_akillnicklist contrib module. Automatically AKILLs a list of clients,
  given their operating parameters.
- change CLEARCHAN GLINE action to AKILL to be more consistent with the rest
  of Atheme. GLINE still exists as an alias to AKILL.

saslserv
--------
- add AUTHCOOKIE SASL method which allows for integration with Iris

code
----
- add taint subsystem which allows developers to programatically define
  unsupportable conditions.
- constify *line_sts() protocol module functions.
- track deaf umode and set it on services clients if fantasy is disabled.
- allow #else in helpfiles
- startup flag -r (read-only) added.
- enable large file support.
- Add 'force_language' to sourceinfo_t, which forces the locale
  to be reset to the language specified by the sourceinfo structure.
  Useful for forcing XMLRPC responses to be in English.
- force dependency calculation before most targets to fix -j problems; there
  is a new target build-nodeps to skip this for subsequent builds (like the
  old behaviour of build).

other
-----
- logging system entirely reworked.  snoop() is deprecated and will be
  removed in the next version.
- add general::exempts config block, for masks that will never be
  automatically klined.
- add configurable command aliases to the services blocks
- helpfiles added for all contrib modules.
- make the wumpus contrib module compile and work again.
- anope_convert: support newer 1.8.x Anope versions and made anope_convert
  a bit more robust in handling encrypted passwords.

Atheme Services 5.0.1 Release Notes
===================================

botserv
-------
- When kicking users from an otherwise empty channel, set INHABIT, so that
  the bot leaves the channel after a short delay.

code
----
- Remove legacy .disp field from core services structures.

Atheme Services 5.0 Release Notes
=================================
### [MERGED] indicates items merged to the 4.0 branch

ircd protocol
-------------
- inspircd12: fix UID parsing and rejoining services after kicks. **[MERGED]**
- TS6: allow nicer topic setting using charybdis 3.2's ETB.
- hyperion: fix a bug that could cause the hostnames of services clients
  to be overwritten. **[MERGED]**
- plexus: port to ts6-generic, add UF_IMMUNE for +N, add support for
  permanent channels.
- hybrid: fix a crash.
- unreal: use SVSKILL for kills from NickServ. This reduces excessive server
  notices.

nickserv
--------
- Matching a nickname access list entry no longer resets last used time.
- Allow authentication via an LDAP server.
- Add some missing help files.
- Start the enforce timer on /ns set enforce on.
- Add a per-account language setting. This currently does not work very well.
- Prepend "(restored) " to marks restored from previously deleted accounts.
- Change VHOST syntax, adding an ON/OFF keyword and requiring a FORCE keyword
  to set a vhost on a marked account. The old syntax still works for vhosts
  containing a dot, colon or slash.
- Add ns_listlogins contrib module. This allows logged in users to see real
  hosts of their other logins.

chanserv
--------
- Set owner/protect on the founder of a new channel, if appropriate.
- Do not set protect status if the user already has owner.
- Rework successor selection for channels to respect flags more. **[MERGED]**
- Allow users with +V to voice themselves.

botserv
-------
- New service. This allows users to have a "bot" join their channel instead
  of ChanServ.

hostserv
--------
- New service. This adds per-nick vhosts and a request system to what
  /ns vhost provides. As long as per-nick vhosts are not used it interoperates
  with /ns vhost.

alis
----
- Add -maxmatches option which xmlrpc and chan:auspex may set to higher than
  the default.

oper
----
- Fix a possible crash with /os greplog. **[MERGED]**
- Add SGLINE system for bans by realname (TS6 xline).
- Services ignores no longer apply to users with general:admin privilege.
- Add /os listklinechan to the os_klinechan contrib module.
- Add os_kill contrib module. This allows opers to kill users while hiding
  their identity. (This was added earlier, but not linked to the build.)
- Add SQLINE system to disallow nick and channel names (TS6 resv).
- Fix possible crash with /os noop.
- RWATCH now also watches nick changes.

xmlrpc
------
- Remove 4K limitation on length of xmlrpc command output. **[MERGED]**

code
----
- Remove select() support and code to allow multiple "socket engines".
  poll() is sufficient.
- Rework the network connection code to be cleaner and more flexible.
- Close all connection_t fds in child processes.
- Allow using sourceinfo_t.v with IRC sources.
- Some tweaks to the build system.
- Add type checking to the hook system. See src/hooktypes.in. It may be
  necessary to specify --enable-warnings to configure to enable the checks.

other
-----
- Try to detect MacOS X crypt(3) breakage in crypto/posix and generate a DES
  based hash.
- Allow the user_add hook to remove the user from the network safely.
- Add user_nickchange hook for nick changes, which is also allowed to remove
  the user from the network.

Atheme Services 4.0 Release Notes
=================================
[MERGED] indicates items merged to the 3.1 branch

ircd protocol
-------------
- Add support for ShadowIRCd 5 **[MERGED]**. This replaces the support for
  older versions of ShadowIRCd.
- hyperion: improve detection of overwritten I:line spoofs.
- hyperion: Add support for UF_IMMUNE.
- general: Do not enforce AKICKs against users marked UF_IMMUNE; it is
  impossible to ban them effectively.
- inspircd12: various fixes and updates.
- ratbox: make akills work with ircd-ratbox 3.x.
- ratbox: add support for ratbox services shortcuts (ENCAP RSMSG,
  m_rsshortcut.so)
- nefarious: allow /ns vhost (FAKEHOST).
- nefarious: let services joining channels op themselves, avoiding HACK(4)
  notices
- ircd-seven: new protocol module
- Limit the send queue to the ircd to a configurable value, default 1MB.
  Large networks may need to increase this.
- Limit IRC command output to 2000 lines. XMLRPC is unaffected.
- Add tracking for the "server admin" umode in some ircds.
- ptlink: add support for forced nick changes (SVSNICK), fix nickTS.
- Remove some obsolete protocol modules: aurora, sorcery, shadowircd.
  If you do still use one of these, please contact us.
- Add support for P10 account creation times.
- Add support for P10 user IPv6 addresses.

nickserv
--------
- Snoop on freeze on/off.
- Add nickserv/vacation module, allowing to temporarily extend expiry times.
- Make register help text depend on whether email verification is used.
- Refer users to their email if they try to identify again while unverified.
  **[MERGED]**
- In FUNGROUP, allow dropping account names, by specifying a new account name.
- Add optional nickserv/listownmail to allow users to see accounts with
  their email.
- When refusing a login due to maxlogins, tell the user what the logged in
  nicks are.
- Show FREEZE status (but not setter, time or reason) to normal users.
- Show taxonomy (property, metadata) in INFO.
- Show recognized (access list) a bit better in INFO.
- Ignore access lists for frozen accounts. **[MERGED]**
- Add ns_fenforce contrib module. This allows admins to toggle enforce on any
  nickname.
- When a user is recognized but not identified, still tell them to identify,
  but with a shorter message.
- Make nickserv/enforce timings more accurate.

chanserv
--------
- Allow multiple spaces before fantasy commands when ChanServ is addressed
  by nickname.
- QUIET/UNQUIET now notify the target user or channel.
- Show the current successor in /cs info (for +A users and opers).
- FFLAGS now overrides the NEVEROP setting on the target account.
- Set owner/protect if appropriate after xOP ADD.
- Show taxonomy (property, metadata) in INFO.
- Add chanserv/set_limitflags, allows limiting +f's power.

alis
----
- Fix handling of key and limit options.
- Allow alis list on a single +s channel the user is on.

memoserv
--------
- Add DELETE OLD to delete all read memos.

gameserv
--------
- Do not add chanserv commands if fantasy is disabled.
- Allow ROLL, WOD and DF with a channel name to send the results to that
  channel without requiring fantasy commands. This must be specifically
  enabled on a per-channel basis using the new ChanServ command SET GAMESERV
  (module chanserv/set_gameserv).

oper
----
- Allow searching for AKILLs matching a given mask or id in AKILL LIST.
- Allow running an operserv-only services instance, which picks up login
  names from the main instance (currently only for hyperion, TS6 and P10
  ircds).
- Add optional PCRE support. Configure --with-pcre to enable it and add
  the p flag to use it (e.g. /os rmatch /\d\d\d/p). The regex wrapper has
  been changed slightly to make this possible.
- Report other nicks of deleted accounts to snoop and log file.
- Add operserv/greplog module (from freenode modules) to allow searching
  through recent logs from IRC.
- Automatically rehash after loading modules that need a rehash.

xmlrpc
------
- Fix atheme.memo.ignore.list and atheme.memo.ignore.clear to require only
  two arguments (third wasn't ever in use). **[MERGED]**

code
----
- Change kline_delete() to take a kline_t pointer instead of a user and host.
- Allow modules to influence the expiry process.
- Fix a minor memory leak on /os REHASH.
- Fix null pointer dereference with some invalid config files.
- Move the metadata entries to object_t.
- Change some protocol module functions to take object pointers instead of
  names and add some const keywords.
- Modules can now request other modules be loaded. This has been used to
  move some generic TS6 and P10 stuff into common modules.
- Rename CMODE_OP and the like to CSTATUS_*, emphasizing that they are
  separate from simple modes.
- Use C99 booleans (<stdbool.h>, bool, true, false).

other
-----
- Allow arbitrary line lengths in flatfile database loader.
- Synchronized with libmowgli 0.7 framework.
- Remove automatic module loading for modules/ directory; this behaviour
  has been deprecated since version 0.3.
- Fix ircservices conversion for ircservices 5.1.
- Improve flood detection.
- Fix a bug with /os identify introducing enforcers.
- Fix a bug that could cause normal users to be seen as enforcers.
- Allow any service's nick/user/host/realname to be set in the configuration
  file, and update them on a rehash. The service creation code works quite a
  bit differently to make this possible.
- Fix running on MacOS X 10.5.
- Fix compilation sometimes using system include files in place of our own.
- Change the default for gettext (NLS) to disabled in the setup script.
- Add a check against loading incompatible modules. Formerly, trying to load
  incompatible modules often caused a crash.
- Rework the configuration file parser to detect more errors and make it
  easier to add configuration options.
- Add +a to the example configuration's SOP to fit expectations better.
- Update anope_convert for anope 1.8 enc_md5/enc_sha1 passwords.
- Wake up the process less often if it is idling.
- Install an example services MOTD automatically.

Atheme Services 3.1 Release Notes
=================================
### [MERGED] indicates items merged to the 3.0 branch

ircd protocol
-------------
- Fix a crash that could happen with ircd bugs or nick collisions with
  services. **[MERGED]**
- Fix host changes in hyperion. **[MERGED]**
- Do not check the server's password in the hyperion protocol module.
- Do not allow spoofs ending in a slash in the hyperion protocol module.
- Allow nickname enforcers which are clients.
- Fix ping replies in P10.
- Add support for InspIRCd 1.2.
- Some ircds dislike colons in kline reasons, so don't use them for flood
  klines.
- When restoring an akill, send it to all servers on all protocols.
  Formerly, on some protocols it was only sent to the server the banned
  user was on.
- Add ircd-aurora protocol module. ircd-aurora is a patched version of
  charybdis with +qah channel statuses.
- For ircds that do not indicate host change to clients, send a 396 numeric
  instead of a notice from the service. This is easier to parse for clients.

nickserv
--------
- Comment out nickserv/subscribe from the example configuration, because
  it is experimental at this time. **[MERGED]**
- Fix a possible crash in nickserv release (nickserv/enforce module). **[MERGED]**
- Fix RETURN only accepting relatively short email addresses.
- Allow disabling the possibly slow maxusers (accounts/email) check by
  putting 0.
- Show /ns vhost in /ns info. Appears to user self and user:auspex opers.
- Adjust times so nicks cannot appear created before their account or used
  after their account was last seen.
- Add user_verify_register hook, called when a registration is verified.
  This is after a successful VERIFY if email verification is enabled, after
  a successful REGISTER if not.
- Make gen_vhostonreg contrib module only grant vhost once it's verified,
  and also set vhosts on users without vhost as they identify.
- Add clearer log messages for duplicate accounts/nicks/channels in atheme.db.
- Make INFO default to the user's nick (owned nicks) or current account (no
  owned nicks).
- Also introduce an enforcer when FNCing a user via the RELEASE command.
- Allow ignoring enforce on nicks unused for too long (nickserv::enforce_expire
  config option). This does not affect held accounts.
- Add nick_can_register hook and use it to block GROUP on guest nicks also.
  This hook is called on both REGISTER and GROUP (if nickname ownership is
  enabled).
- In SENDPASS, require the new keyword FORCE to override marks and the new
  keyword CLEAR to clear keys that were previously sent but not yet used.
  If these keywords are needed, the oper will be warned.
- Do not allow SENDPASS on unverified accounts.
- Make the enforce delay settable in the config file.
- Make holdnick enforcer time variable, 30s the first time then 1h.
- Add ns_ajoin contrib module to allow services-side autojoin.
- Show a pending email address change in INFO, to user self and user:auspex
  opers.
- Add ns_forbid contrib module. This registers, enforces, holds and freezes
  a nickname.
- Split DROP into DROP (users) and FDROP (admins).
- Send all failed password attempts for SOPER accounts to the snoop channel.
- Make the text in INFO for unverified accounts more conspicuous.

chanserv
--------
- Fix removing non-applicable flags (e.g. +hH) from host channel access.
  **[MERGED]**
- Fix ChanServ not deopping in some cases with guard on and changets off.
  **[MERGED]**
- Fix some ugly output in chanserv/unban_self. **[MERGED]**
- Respect NOOP flag in cs_sync contrib module.
- Allow calling RECOVER via xmlrpc.
- Add channel_can_register hook to allow modules to block channel registrations.
- Add SET QUIETCHG (nickserv setting) which suppresses notices from OP,
  VOICE, and the like by other users.
- Add cs_updown contrib module. This provides UP and DOWN commands that add
  and remove all modes a user is entitled to.
- Change SET STAFFONLY to SET RESTRICTED. This kicks all users except those
  with chan:joinstaffonly priv or any access (except +b) on the channel.
  Also make it handle +i channels more effectively.
- Allow admins to change oper only modes in mlocks even without +s flag.
- Snoop changes to oper only modes in mlocks.
- Split DROP into DROP (users) and FDROP (admins).
- Add a confirmation step against accidental drops to DROP. This only
  applies to commands via IRC.

alis
----
- Move ALIS from contrib to modules. The new atheme.conf line is
  loadmodule "modules/alis/main";

memoserv
--------

gameserv
--------

oper
----
- Fix a possible crash in /stats B. **[MERGED]**
- Fix slight damage to news items when reloading in contrib/os_logonnews.
  **[MERGED]**
- Allow GLOBAL to be used from non-IRC.
- Add CLONES DURATION to allow changing the duration of the network bans
  set by the clones module.
- Add os_klinechan contrib module. This allows setting channels to kline
  any users joining them.

xmlrpc
------
- Some improvements to buffer and character set handling.

code
----
- Disable object_t refcount.
- Fix various format string types, add many const keywords, hide a few
  structs that should be private.

other
-----
- Improve performance with large databases by changing the mowgli_heap
  memory allocator. **[partially MERGED]**
- Improve performance by changing the dictionary to a patricia algorithm.
- Decrease memory usage for large networks.
- Add LOCALEDIR to Makefile.in files, necessary for gettext. **[MERGED]**
- Some improvements to the hybserv/theia conversion tool.
- Some improvements to the ircservices conversion tool.
- Change maximum nick length from 30 to 31.
- Remove redundant expire_check and db_save in several places. This makes
  restart, shutdown and rehash faster without threatening data integrity.
- Add Russian help files from Kein/darkwire. Using these currently requires
  manual copy/rename operations.
- Add Russian translation from Kein/darkwire and fix the build system so it
  is automatically installed if gettext is enabled.
- Allow for crypt() in libc as well as libcrypt (MacOS X).
- Fix nested includes in the configuration file.
- Add child process tracking.
- Make some help files depend on what modules are loaded.
- Fix a bug that caused certain timed events to be executed too late.

Atheme Services 3.0.3 Release Notes
===================================
- Fix use of chanserv/{owner, protect} modules with InspIRCd, AGAIN.

Atheme Services 3.0.2 Release Notes
===================================
- Fix use of chanserv/{owner, protect} modules with InspIRCd.

Atheme Services 3.0.1 Release Notes
===================================
- Fix a bug causing problems when a user changes the case of their nick.

Atheme Services 3.0 Release Notes
=================================

ircd protocol
-------------
- In hyperion, use COLLIDE instead of KILL (less noisy).
- In hyperion, fix a bug in host/realhost processing.
- Allow channel ban and host channel access matching to be customized by
  protocol modules. In particular, make charybdis extbans acorsx and hyperion
  +d and forwarding bans work.
- Track if the ircd supports CIDR bans.
- Some fixes for TS 0 channels in TS5/TS6.
- Drop support for InspIRCd 1.0, you should use InspIRCd 1.1 as it is more
  reliable. (Recommended by InspIRCd upstream.)
- Add more explicit support for invite exceptions.
- Fix SVSMODE in ultimate3.
- Make /version reply (351 numeric) rfc1459 compliant.
- Add support for mlocking various new modes in inspircd.
- Add experimental support for inspircd 1.2.
- Add TBURST support to hybrid protocol module, allows better topic setting.
- Improve support for unrealircd.
- Improve handling of kills and nick collisions.
- Improve support for +q/+a (owner/protect) channel statuses.

nickserv
--------
- Add nickserv/subscribe, allows users to be notified when metadata is
  changed.
- Disallow DROP of a frozen account except by admins.
- Always show configured vhost to users, not any other host.
- Allow ENFORCE and NOMEMO to be set on new accounts via uflags in the
  configuration file.
- Load nickserv/group by default in the example configuration file.
- Add nickserv/set_privmsg, enables use of private messages on a per-account
  basis.
- INFO now shows user:auspex or chan:auspex opers a count of channel access.
- Add nickserv/set_private, allows users to hide some information about
  their account.
- Check nickserv/vhost for validity for hyperion/charybdis/hybrid (Bugzilla #7).
- When displaying nick(account), add a space in between.
- Rework INFO so it is easier to understand and more readable.
- Add nickserv/set_accountname, allows changing the account name to
  another grouped nick (Bugzilla #102).
- Save marks when nicks/accounts are dropped and restore them if they are
  recreated.

saslserv
--------

chanserv
--------
- Change channel_register hook from mychan_t * to hook_channel_req_t *. This
  makes it possible to send text to the person registering the channel in a
  clean way.
- Improve non-halfops behaviour somewhat.
- Add chanserv::maxchanacs option, limits how many entries can be put in
  channel access lists.
- Add chanserv/unban_self, which only allows unbanning the source user.
  This is intended to be loaded instead of chanserv/ban for the paranoid.
- Kick out unauthorized users who may be recreating channels mlocked +i.
- Disable xOP in the example configuration, it does not work well.
- HOLD on an account no longer prevents channels from expiring.
- Allow multiple founders on a channel, signified by a new flag +F.
  All founders have the exact same privileges, including the privilege to
  add/remove founders and drop the channel.
- When processing a kick command, put the kicking user before the user-given
  reason.
- Add chanserv/set_private, allows users to hide some information about
  their channel.
- Hide SET FANTASY from help if fantasy commands are disabled in atheme.conf.
- Add chanserv::deftemplates config option to start channels off with some
  templates.
- For the "addressing chanserv" fantasy command, require a non-letter
  after the nick.
- Add chanserv/owner and chanserv/protect, to set/unset +q/+a modes.

memoserv
--------
- Still notify online user if EMAILMEMOS is set.
- Consider MemoServ ignores matched if any grouped nick of the sender matches.

gameserv
--------

oper
----
- Add LG_REGISTER level for all registration related messages. This can be
  used to create a log file containing all registrations and drops only.
- Allow any akill without wildcards in the user part, like in charybdis 2.2.
- Allow CIDR masks in the CLONES exempt list.
- Add privilege operserv:akill-anymask, allows akilling masks with
  insufficient non-wildcard characters.
- Allow requiring an additional password for services operators.

xmlrpc
------
- Make xmlrpc faster.
- Create a nick in addition to an account in the obsolete xmlrpc/account
  module.
- Fix crashes when unloading xmlrpc modules.

code
----
- Switch from Subversion to Mercurial. As a result, the $Id$ fields in many
  modules are no longer updated. We are examining other solutions for identifying
  modules' revisions using Mercurial at this time.
- Remove redundant user_t pointer from hook_channel_req_t.
- Switch to non-copyleft BSD licensing.
- Integrate libmowgli framework into the tree. Only mowgli_dictionary is
  used for now.
- Make --disable-balloc work and no longer change ABI.
- Rework channel message handling.
- Remove some unnecessary cruft from configure.
- Change the way fantasy commands work.
- Enable GCC format string parameter checking.
- Move nickname ownership nagging to the nickserv module.

documentation
-------------
- Move developer documentation to doc/technical/.
- Add conditions to the help files, used to hide halfops/owner/protect/oper
  information when it is not applicable.

other
-----
- Split expiry setting into seperate nickserv::expiry and chanserv::expiry.
- Make expiry settings of 0 work more consistently as "do not expire
  anything", and still update last used times.
- Add graphtastical contrib module, creates files to be processed with
  graphviz.
- Add mlocktweaker contrib module, adds or removes additional modes from
  the default mlock.
- Tell users on services ignore once per session that they are ignored.
- Remove database{} configuration block.
- Rename example files.
- Put multiple line help texts in one gettext string.
- Improve hybserv/theia conversion tool.
- Add an ircservices conversion tool.
- Tweak various strings for translation purposes.

Atheme 2.2 Release Notes
========================
### [MERGED] indicates items merged to the 2.1 branch

ircd protocol
-------------
- Don't send "Setting your host to \2%s\2." notice if the ircd already
  notifies users of host changes, by sending this notice from the protocol
  module.
- Fix some potential crashes and desyncs with inspircd and channel
  bans. **[MERGED]**
- Allow for SAQUIT in inspircd11. **[MERGED]**
- Allow for RSQUIT in inspircd11 to allow /squit on jupes. **[MERGED]**
- Increase maximum parameter count for protocol commands from 19 to
  35. **[MERGED]**
- In charybdis/ratbox/hybrid, hide jupes from a flattened /links.
- Add basic support for IRCXPRO/OfficeIRC.
- Improve topic changing support (prevent our topic changes to be ignored
  in some cases and allow topicsetter/topicTS to be set more often). This
  is done by adding a prevts parameter to the topic_sts() protocol module
  function.
- Track /away status (but not away messages).
- Support join throttling (+j) in bahamut and solidircd.
- Make /os jupe work on existing servers for ircds using unconnect semantics
  on SQUIT (bahamut, ircnet, ultimate3, solidircd, inspircd11). This marks the
  server as "pending jupe" and introduces the jupe when it really goes away.
- Make /os jupe work on P10.
- Use TMODE in TS6 and use FMODE in inspircd11 (reduces desyncs).
- Support channel mode +J in inspircd11.
- Add chanserv::changets support for ircu 2.10.12.06 or newer (undernet).
- Ensure kicks are accepted if the service is not on the channel in
  asuka, bircd and undernet.
- Allow services to create channels.
- For ircds that use a umode for registered nicks, also set and recognize (for
  some) that umode for grouped nicks other than the account name.

nickserv
--------
- The LIST command now allows searching by last host/vhost, and shows
  hold/waitauth status also.
- If ENFORCE is loaded, disallow registering accounts starting with
  Guest<digit>.
- ACCESS ADD now allows omitting the mask and generates one matching the
  user if so.
- GHOST now omit the source's user@vhost from the kill message if it is the
  same as the target's user@vhost.
- Add FVERIFY command to allow admins with user:admin privilege to verify
  any account without needing the verification email.
- Add FREGISTER command to contrib, allows creating accounts with little
  checking and pre-crypted passwords.
- In INFO, show to everyone if the account has not completed registration
  verification (MU_WAITAUTH).
- Add ns_ratelimitreg contrib module, rate limits account registrations.
  The amount/time is hardcoded.
- Don't allow implicit logout by LOGIN/IDENTIFY as another account if the
  LOGOUT command is not loaded.
- Make it less likely that a user will be recognized as a recreated account.
- Make FREEZE and RETURN log out all sessions.
- Split out SENDPASS from user:admin to its own privilege user:sendpass.
- Add SETPASS, an alternative method of password retrieval. When setpass is
  loaded, SENDPASS will send a code that can be used to set a new password
  with setpass, only affecting the old password when that happens.
- Deny certain spoofs that would break the protocol in VHOST.
- Make ENFORCE more efficient and accurate.
- Do not allow dropping an account with the HOLD flag set.

saslserv
--------
- Improve logging.
- Increase the grace time.

chanserv
--------
- Fix a crash in /cs set mlock. **[MERGED]**
- Allow multiple trigger characters for fantasy commands.
- In a flags listing, show the template name, if any, in parentheses after
  the flags.
- Only set owner/protect on identify if they also have +O flag. Setting on
  join already behaved this way.
- Log at LG_INFO level: channel succession, channel deletion due to no
  eligible channel successors and account/nick/channel expiry.
- In WHY, also show founder access explicitly.
- The LIST command now shows hold status also.
- Get rid of some unnecessary mode lock checks.
- Allow disabling HOP by setting it to the same value as VOP.
- Disable and remove +hH access flags if the ircd does not support halfops.
- Add cs_userinfo contrib module, allows setting an info text on users with
  access to a channel, which is shown to the channel on join.
- Add TOPICPREPEND command.
- Track when channel access entries were last modified.
- Add a reason field to AKICK. The part of the reason after a '|' character
  is not shown publicly.
- Add QUIET and UNQUIET commands for +q modes in charybdis and hyperion.
- Make RECOVER also clear all owner/protect statuses on ircds that support
  those.
- Add a new type of fantasy command: addressing ChanServ by name, for example
  "ChanServ: kick somenick reason here".
- Do not allow dropping a channel with the HOLD flag set.
- Allow per-channel redefinition of xOP templates.

memoserv
--------
- If a user unsets away and is logged in or recognized, notify them of new
  memos.
- Add SENDOPS command for channel memos (sent to all channel ops). If this
  is loaded, SEND also accepts channel names.
- Add READ NEW which reads upto 5 unread memos.

gameserv
--------
- Add game services, works as fantasy commands and in /msg.

oper
----
- CLONES ADDEXEMPT/DELEXEMPT now have better accountability.
- CLONES now allows editing an exemption without removing it first.
- Add some more checking on AKILL masks.
- The os_logonnews contrib module now uses DATADIR to find its file. **[MERGED]**
- Make CLONES not send more than one warning per IP per 5 minutes unless the
  number of clones is increasing.
- Make the log subsystem more flexible.
- Make jupes record who placed them.
- Use configured --bindir for restart.

code
----
- Remove the incomplete atheme testsuite.
- Rework handle_ctcp_common() to use sourceinfo instead of passing
  redundant references around.
- Move all code under libatheme/ to src/.
- Add an object framework and overhaul account management to use it.
- Add some new assertion macros that send wallops if the assertion fails.
- Remove unused me.uplink and serverinfo::uplink.
- Move some of the m_server() logic to a new function handle_server() in
  src/ptasks.c.
- Add vim options to the end of each C file.
- Remove generated files from SVN, you now need autoconf and automake (only
  for aclocal) to compile the SVN version.
- Add a 'dist' target to the Makefile to build tarballs.
- Move the modestacker from channel names to channel_t pointers.
- Move part() to src/services.c. The protocol module function is now called
  part_sts() and takes channel_t and user_t pointers.
- Move mode_sts() protocol module function from channel names to channel_t
  pointers.
- Stop using <stdint.h> types.
- Remove remnants of Windows support.
- Make channel_delete() take a channel_t pointer instead of a name.
- Various optimizations and cleanups.

xmlrpc
------
- Added perl modules abstracting atheme's XML-RPC interface.

documentation
-------------
- Update documentation a little. **[MERGED]**

other
-----
- Fix a potential division by zero. **[MERGED]**
- Improve random number generation. **[partly MERGED]**
- Move crypto, backend and protocol directories to modules/. This requires
  changes to atheme.conf.
- Remove the PostgreSQL backend. If you were using this, first start the old
  version with backend/postgresql, then load a supported backend like
  backend/flatfile and /msg operserv update, to convert your data. See
  doc/SQL for more information.
- Add various checks to server names which should give clearer warnings
  about several misconfigurations.
- Various corrections to log levels.
- Exit atheme if atheme.db cannot be opened for reading for any reason other
  than it not existing, to avoid overwriting it with an empty database later.
- Add a hybserv/theia to atheme conversion tool to contrib.
- Tone down debug logging a little so it is a bit more usable on large
  networks.
- Add gettext support (see doc/TRANSLATION).
- Move removal of old servers/users/channels from just before reconnecting
  to just after disconnecting.
- Rename the services binary to atheme-services.
- Add alis (advanced list service) to contrib; this is a port of
  ratbox-services's alis.
- Update anope_convert for anope 1.7.18.

Atheme 2.1 Release Notes
========================

ircd protocol
-------------
- The inspircd11 protocol module now sends wallops as server notices
  and no longer needs m_globops.so (but still uses it if loaded).
- When sending a notice to a channel as a server notice (because the
  service is not on channel and the ircd would not allow the notice),
  prepend it with "[<service>:<channel>] " instead of "<service>: "
- Clean up handling of services deopped on channels.

nickserv
--------
- Add nickname grouping, this allows users to register multiple nicks
  to one account. The GROUP and UNGROUP commands add and remove additional
  nicks. Additional nicks expire separately from the main account. Admins
  can use FUNGROUP to remove additional nicks from any account.
- The ACCESS ADD command now converts masks like 1.2.3.* to CIDR masks
  instead of rejecting them.
- Add LISTVHOST command.
- Make the gen_vhostonreg contrib module also generate a valid (although
  ugly) vhost for account names which contain characters not valid in
  hostnames.
- Add user_info hook, allows modules to add things to /ns info.
- Show ENFORCE status in /ns info (to all users).
- Snoop notices for registrations now show the source name explicitly
  if appropriate.
- Add user_can_register hook, allows modules to abort account
  registrations before they go through.

memoserv
--------
- Allow sending a memo to oneself.
- Remove rate limiting for users with general:flood privilege.

chanserv
--------
- The TEMPLATE command now allows updating all channel access entries
  matching the template with a template change, by prefixing a flag
  change with an '!'.
- Add SYNC command to contrib, makes channel status agree with flags.
- Add FFLAGS command, allows opers to force a flags change on any channel.
- On newly registered channels, only mlock -l if no limit is set
  and only mlock -k if no key is set.
- Show channel email address in INFO.
- Add chanserv::trigger option, allows changing the '!' in fantasy commands
  to something else. The special case for '.flags' has been removed.
- Add SET GUARD, allows toggling whether ChanServ joins the channel on a
  per-channel basis. The general::join_chans option determines whether
  SET GUARD can be used.
- Make the WHY command show all flags.
- Allow using the xOP commands via non-IRC.
- Allow using SET as a fantasy command.
- Move help for /cs halfop to a separate help file.
- Make channel access entry metadata work. This allows modules to associate
  name/value pairs with channel access entries. There is currently no
  facility to read/write channel access entry metadata directly like there
  is for user and channel metadata.

oper
----
- Add /os soper, allows adding services operators at run time.
- Add operclass::needoper option, denies giving any privilege to IRC users
  matching this operclass who are not IRCops.
- Add some more log levels and generally clean up logging, for example
  allow disabling logging of all commands. Note that loglevel error logs
  much less now, use loglevel info for moderate logging.
- Add os_logonnews contrib module to send text to all connecting users.

code
----
- Remove irccmp() and ircncmp(). These functions were equivalent to strcmp()
  and strncmp(). IRC-style case-insensitive comparisons are irccasecmp() and
  ircncasecmp().
- Make clog unavailable outside libatheme/ and rename it. Other code should
  use slog(), logcommand(), etc. to log things.

documentation
-------------
- Add some missing help files.
- Clarify some things in the installation documentation.
- Add documentation for the SASL client protocol.

other
-----
- chanserv::changets is now shown as 't' in version replies
- Make --enable-fhs-paths also affect the location of the log file, data
  files and pid file. Note that for data files to work with
  --enable-fhs-paths, they need to use DATADIR instead of "etc".
- Add motd replacement &mynicks&.
- Fix various memory leaks.
- Add support for converting services operators, nickname access lists
  and nickname grouping in anope_convert.c.
- Using the postgresql backend is now officially discouraged; it will be
  removed in 2.2.x and newer.
- Remove socklen_t check from configure.

Atheme 2.0 Release Notes
========================

ircd protocol
-------------
- The ptlink module now assumes +a (channel admin) and +h (halfops) exist.
- Make protocol parsing stricter, rejecting various illegal messages.
- Don't leave empty channels if all users in a channel are killed or invalid.
- Add support for EUID (real host behind dynamic spoofs and login name in same
  message as user introduction) to the charybdis module.
- Split inspircd support in inspircd10 for 1.0 and inspircd11 for 1.1.
- Add chanserv::changets support to the inspircd11 module.
- Add STATS, ADMIN and MOTD support to the inspircd11 module.
- Add SVSHOLD support (nickname enforcers that are not clients) to the
  protocol abstraction.

services
--------
- Make CTCP replies generic to all services.
- Make services commands more generic, allowing them to be called in other
  ways than from IRC.
- Show more details when describing users issuing commands, for example
  the account name if it is different from the nick.
- Send no such nick reply if we get a message to a nonexistent service.
- Support CIDR channel bans/exceptions and akills.
- Shut down if too many services are killed in one second.
- Handle SASL logins more cleanly.

nickserv
--------
- Move nickname enforcement module from contrib to modules. This forces
  nickname changes on users who do not identify to their nick.
- Add gen_vhostonreg module to contrib from atheme-modules, sets a vhost on
  new nick/account registrations.
- Make it possible for nickserv to act as userserv with the
  nickserv::no_nick_ownership config option and the nickserv/login module.
  Remove the old userserv which mostly consisted of a duplicate of nickserv.
- Add nickname access lists; users matching an u@h mask on the access list for
  a nick will not be warned to identify and will not be affected by nickname
  enforcement, and the last login time will be updated. Channel access will
  not be granted.
- Make /ns acc a bit more usable.

chanserv
--------
- Add TOPICLOCK flag, reverts topic changes by users who do not have +t flag.
- Store the channelTS of registered channels in the private:channelts metadata
  entry. This is used to make chanserv::changets nicer and to avoid KEEPTOPIC
  restores if the channel was not recreated.
- Also give VERBOSE notices on foundership changes.
- Fantasy and regular commands now use the same code. Fantasy commands are
  executed with VERBOSE set. This allows executing all chanserv commands
  affecting a single registered channel, except DROP and SET, as a fantasy
  command but removes some aliases.
- Allow /cs status when not logged in.
- Add /cs count from contrib, shows how many entries are in access lists.

oper
----
- Add a privilege operserv:massakill to control new commands that may
  do mass kills.
- Add /os clearchan, kicks, kills or klines all non-IRCops in a channel.
- Add /os compare, shows common channels of two users or common users in two
  channels.
- Add /os rmatch, shows all users matching a regular expression.
- Add /os rakill, bans all users matching a regular expression from the
  network.
- Add /os rnc, shows the most common realnames on the network.
- Add /os clones, watches the network for IPs with too many connections.
- Add operserv rwatch, a list of regular expressions all connecting clients
  are matched against. Matching clients are described in the snoop channel
  and/or klined.
- Add /stats B, shows hash statistics, requires general:auspex privilege.
- Add messages to the snoop channels about various things that used to
  generate wallops only; move some other things from wallops to snoops.
- Add /stats f, shows some information about open connections.

xmlrpc
------
- Clean up the HTTP protocol handling considerably.
- Allow pipelining multiple calls over one connection.
- Allow calling most services commands via XMLRPC.
- Allow passing a parameter (typically true source IP address) to various
  XMLRPC methods, which will be logged.

documentation
-------------
- Add a script to convert the help files to HTML.
- Make various help files clearer.

code
----
- Replace the old hashtables with a cleaner and more powerful dictionary tree
  implementation.
- Make 'make depend' work, this allows only recompiling what is necessary.
- Move around many things to different source files, hopefully making it
  easier to find things.
- Restrict visibility of some uplink-related declarations (uplink.h and
  pmodule.h no longer included by atheme.h).
- Add ability to run/build Atheme without the block allocator (for debugging).
- Change parameter type for the channel_join and channel_part hooks to
  hook_channel_joinpart_t *. This avoids ugliness when the first hook kicks
  out the user and the next uses freed memory.
- Rewrite much of the network I/O code.
- Pass less nicks and UIDs around in string form.
- Remove the win32 port, it was hopelessly broken and nobody cares about it.
- Add more comments about what various functions do.

other
-----
- Get rid of signal-related race conditions.
- Add DESTDIR support to the build system (allows installing to a different
  path than where we expect to run from).
- Add sorservices-compatible password encryption support.
- Various bugfixes and cleanups.

Atheme 1.2 Release Notes
========================
### [MERGED] indicates items merged to the 1.1 branch

ircd protocol
-------------
- Allow using TS changes to reliably deop people recreating channels
  with the new chanserv::changets config option (for charybdis, ratbox,
  hybrid, bahamut, solidircd).
- Put netwide end of burst detection in the core. This is used for suppressing
  various notices to users coming back from a netsplit or services restart.
- Add modules which disable use of halfops, channel protection and founder
  statuses to contrib.
- Only set owner/protect modes if the user has the +O (autoop) flag.
- Remove the Chunky Monkey protocol module.
- Update Plexus protocol module to Plexus 3. **[MERGED]**
- Fix global notices in the P10 protocol modules. **[MERGED]**
- Fix login handling in the nefarious protocol module. **[MERGED]**
- Support more modes in the inspircd protocol module. **[MERGED]**
- Use m_services_account.so in inspircd to track services logins better.
  Using this module is required. **[MERGED]**
- Make the PTlink protocol module work. **[MERGED]**
- Make the DreamForge and sorcery protocol modules work better. **[MERGED]**

channels
--------
- Make the last used time for channels more accurate and show it in /cs info.
- Get rid of many redundant mode lock checks.
- Add full support for non-standard simple modes with parameters, like join
  throttling. These can now be mode locked, etc.
- Add GETKEY command to chanserv, returns the current key (+k) to users who
  have +i flag.
- Rewrite a lot of code relating to channel modes. Among other things, this
  removes the Cygnus mode stacker.
- Don't allow users with an unverified email address to register channels.
- Add cs_kickdots contrib module, kicks users for saying "...". **[MERGED]**

oper
----
- Remove alias KLINE for AKILL.
- Make modrestart work again.
- Improve rehash error handling.
- Global notices now include the oper's nick in the first line.
- Show more detailed version information in atheme -v. **[MERGED]**

other
-----
- Fix crash when a module tries to use something in another module which is
  not loaded. This protection requires changes in modules using other modules
  to be effective, see include/module.h.
- Add command line option -l to change the log file.
- Rewrite nickserv/userserv set command handling, allowing modules to
  provide extra set options.
- Merge modules/saslserv/sasl module into modules/saslserv/main. You should
  remove modules/saslserv/sasl from your config file, it will be deleted
  automatically by make install.
- Clean up stale SASL sessions periodically.
- Mention FANTASY in /cs help set.
- Remove the MySQL backend. If you were using this, first start the old version
  with backend/mysql, then load a supported backend like backend/flatfile and
  /msg operserv update, to convert your data. See doc/SQL for more information.
- Improve flatfile error handling. **[MERGED]**
- Change maximum length of memos from 129 to 300. **[MERGED]**
- Improve handling for network errors. **[MERGED]**
- Many bugfixes and documentation improvements. Some **[MERGED]**

Atheme 1.1 Release Notes
========================
- Shorten /msg chanserv/nickserv/userserv help, the full command list
  is on /msg *serv help commands.
- Add XMLRPC method atheme.channel.access.get.
- Disallow vhosts which are too long or contain @!*?
- Don't introduce a service with a UID if the ircd does not support UIDs
  when loading it at runtime.
- Change regex_match API to be more efficient.
- Disable modrestart, it cannot possibly work. (It used to crash.)
- Add wumpus, a service providing a game, to contrib.
- Add source of the message to hook_cmessage_data_t.
- Add optional new syntax for operclass{} blocks.
- Add SASL support, this allows users on charybdis networks to log in
  before registration to the network; among other things this ensures
  the real host behind a services vhost is not shown to nonopers.
- Add support for /motd <atheme>, uses PREFIX/etc/atheme.motd.
- Allow users to GHOST other nicks which are logged into their account.
- Allow other services than chanserv to process fantasy commands.
- Allow specification of akill duration in hours (h), days (d) and weeks (w).
- Add /os akill sync, sends all akills to all servers.
- Ultimate3 improvements.
- Add support for Nefarious IRCu 0.4.x or later.
- Several UserServ bugfixes.
- InspIRCd improvements.
- Ensure kline exempt fully exempts from akills; do not kill akilled users
  at any time.
- Avoid sending out klines with negative expiry time.
- Add IP glob akill matching, for ircds that send user IP addresses.
- Move akill checking to operserv/akill module.
- Don't allocate me.name/me.numeric on rehash. This fixes a known P10
  issue.
- Add support for ircu2.10.12's +D mode, which was already implemented
  in Asuka.
- Always send both the channel TS and the topic TS when setting a topic
  under P10 ircd's.
- Several other minor tweaks and bugfixes.


Atheme 1.0 Release Notes
========================
- Add ability to use =<nick> where accounts are used. (it aliases to the
  User's account).
- Deop users recreating registered channels if they do not have op flags.
  This attempts not to affect channels created longer ago.
- Add separate protocol module for ircd-hybrid, with CHGHOST support.
- Add ircd name to /version output.
- Add atheme.account.vhost XMLRPC for setting and removing vhosts. Note
  that this, unlike the other XMLRPCs, allows things that the given
  credentials would not normally (over IRC) be allowed to.
- Remove the incomplete and cluttering nickname linking system (this was
  actually done in 1.0rc1, but we forgot to mention it).
- Don't allocate me.name/me.numeric on rehash. This fixes a known P10
  issue.
- Add support for ircu2.10.12's +D mode, which was already implemented
  in Asuka.
- Always send both the channel TS and the topic TS when setting a topic
  under P10 ircd's.

Atheme 1.0rc1 Release Notes
===========================
- Add capabilities to protocol modules:
  - forced nick change (aka SVSNICK)
  - invite
  - channel op notice (aka WALLCHOPS)
- Don't unkline temp akills if they have already expired.
- AKILL wildcard checking changed to ratbox algorithm.
- Further simplification of access checking.
- Hostmask access entries now check vhost, not host. This means services
  cloaks now work in access lists.
- InspIRCd module updated to beta6 and above (tree linking instead of mesh).
- For akicks, use a matching +b'ed hostmask if possible.
- For ircds with dalnet-like +r umodes, don't set it for userserv.
- Add /cs forcexop to reset access levels to xOP values. Useful if CA_?OP
  are changed.
- Make CA_?OP configurable in atheme.conf.
- Add channel_info hook on /cs info.
- Add per-channel flags templates. Templates can be specified in the flags
  command, avoiding the need to memorize complicated flags strings.
  Templates are manipulated with the TEMPLATE command which is similar to the
  FLAGS command.  The xOP levels can also be used as templates.
- Add per-channel fantasy command disable.
- Fix NEVEROP and NOOP options.
- If fantasy commands are globally disabled, mark services clients as "deaf"
  for some ircds.
- Add /cs set verbose ops, works like /cs set verbose on but only shows to
  channel ops.
- Allow users to remove their own access from channels (except akicks of
  course).
- Some improvements to network I/O.
- Add support for permanent (+P) channels in charybdis/hyperion/shadowircd.
- Add support for more FHS-like paths (not really complete but should still
  be helpful for packagers).
- Add fine grained services operator privileges. IRCops get certain
  privileges, and registered accounts can be granted privileges. This
  works via atheme.conf. You will need to redo the operator part of your
  configuration. See the new example.conf and doc/PRIVILEGES for more
  information.
- Add message translation support.
- /cs voice/halfop/op now notifies the target user who did it.
- /cs flags mentions who gave the command in the verbose notice.
- Remove many redundant confirming notices.
- Regardless of join_chans/leave_chans, join chanserv temporarily to
  channels which would otherwise be empty to enforce akick or staffonly.
- Add nickserv enforcement (FNC users who do not identify in time) to
  contrib.
- Drastically improve P10 support.
- Add solidircd support.
- Add support for ban exceptions and other ban-like modes.
- Remove possibilities to log in to accounts without password by changing
  nick during a netsplit on an ircd that does not clear +r (or similar) on
  nick changes.
- Various minor tweaks and bugfixes.

Atheme 0.3 Release Notes
========================
- All services have been entirely modularized. You will need to
  redo your configuration. An authentication service has been
  added (choose from either NickServ or UserServ,) for
  UserServ you should use example.userserv.conf.
- The way modules work has been changed almost entirely. Please try
  loading bad modules especially on macintosh and AIX systems, thanks!
  (Report any negative findings to the tracker.)
- The way sockets work has also changed entirely. We'd love to know
  how well the new poll and kqueue code is working, and if there
  are any issues with it.
- Support for poll() and kqueue() have been added, please comment on
  performance differentials.
- Most static object structures are now described as metadata, please
  comment on how well this transition is working for you.
- PostgreSQL support is starting to freeze. Please note schema changes
  in SQL/atheme_init.sql.
- The build system has been reworked. You may need to use gmake on
  BSD systems. Please comment on any trouble you have had with building
  Atheme (on the tracker, of course.)
- protocol/hyperion supports login session tracking now, please comment
  on how well it is working for you.
- protocol/ratbox support for login sessions has also vastly improved.
- dbtool is most likely broken right now. This is not likely to change
  any time soon.
- The configuration parser has been overhauled. Please report any crashes
  when parsing configurations.
- Channel passwords have been removed.
- Most access checking has been simplified, please make sure it is working
  properly on your network.
- w00t has gone and entirely tokenized the Unreal protocol support. Please
  report any issues with this that you find.
- STATS is now abstracted instead of living in 15 different protocol modules.
- Probably much more that we have missed :)

Atheme 0.2 Release Notes
========================
- Way too many things to note.

Atheme 0.2rc1 Release Notes
========================
- Protocol support has been modularized.
- The configuration format has drastically changed. Please redo your config.
- The core has been modularized. Please provide feedback if something is not
  working as it should.
- Module support has been added. To compile third party modules, add them to
  the modules directory and rerun setup/configure. Any modules in your
  installation's modules/ directory will be automatically loaded at startup.
- Metadata support has been added. To experiment with it, see our wiki
  information on the topic:

  http://wiki.oscnet.org/index.php/Atheme:Experimenting_with_Metadata
- Several NickServ-related bugs have been fixed.
- IRCNet support is marked as experimental. If you run this ircd,
  please give us feedback on how well it functions.

Atheme 0.1 Release Notes
========================

- Dancer/Hyperion IRCd support is marked as experimental. If you run this ircd,
  please give us feedback on how well it functions.
- Chunky Monkey IRCd is also marked as experimental. If you run this ircd,
  please give us feedback on how well it functions.
- NickServ support is experimental. We would like suggestions on how to improve it.
  If you would like to use it, you should use the example-traditional.conf file,
  instead of the example.conf. This will set up a logical environment for NickServ to
  operate in.
- InspIRCd support passes our regression tests, however, we would like feedback
  concerning how well it works. Features added by optional modules are not supported
  at this time. You will need the m_services.so module loaded to make things work
  100% properly.
- UnrealIRCd support is implemented fairly well, but not all features are supported.
- Bahamut 1.8 support has not been tested against 1.8.4, though it is expected that
  1.8.4 should work fine.
- TSora IRCd support (ratbox) does not support TS6, and probably never will.

### If you need help, drop by AthemeNet and ask: irc.atheme.org #atheme

Thanks!
