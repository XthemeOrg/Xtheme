## Xtheme

Xtheme is a set of services for IRC networks designed for large IRC networks with high
scalability requirements.  It is relatively mature software, with some code and design
derived from another package called Atheme and Shrike.

Xtheme's behavior is tunable using modules and a highly detailed configuration file.
Almost all behavior can be changed at deployment time just by editing the configuration.

## development hibernation/sleeper notice
Due to the Atheme Development Group being brought back to life by the maintainers of
Xtheme, Shalture and Zohlai, Xtheme v7.4.0 will be the final release of Xtheme 
(except for bug fixes) and Xtheme will go into "sleeper" or "hibernation" as we
work together to bring you the future of Atheme. 

To this end, you may find the following table useful:    
| Milestone                      | Date             |  
| ------------------------------ | ---------------- |  
| End of new Features/Requests   | 28 February 2016 |  
| End of non-security bugfixes   | 1 June 2016      |  
| End of ALL bugfixes            | 28 February 2017 |  
| End of #Xtheme                 | 1 March 2017     |    
 
 * More information about Atheme can be found at: 
	* http://atheme.github.io/
	* https://github.com/atheme/atheme/

If you are running this code from Git, you should read GIT-Access for instructions on
how to fully check out the Xtheme tree, as it is spread across many repositories.

## basic build instructions for the impatient

Whatever you do, make sure you do *not* install Xtheme into the same location as the source.
Xtheme will default to installing in `$HOME/xtheme`, so make sure you plan accordingly for this.

    $ git submodule update --init
    $ ./configure
    $ make
    $ make install

If you're still lost, read [INSTALL](INSTALL) or [GIT-Access](GIT-Access) for hints.
Help is also available on the [Xtheme IRC Services Wiki](https://github.com/XthemeOrg/Xtheme/wiki)

## SWI (services web interface) Web Panel for Xtheme

If you are interested in having a Services Web Interface (or Services Web Panel), SWI
*IS* compatible with Xtheme and other Atheme forks, and is available at:
http://atheme.github.io/swi.html or https://github.com/atheme/SWI/

## links / contact

 * [GitHub](http://www.github.com/XthemeOrg/Xtheme)
 * [Website](http://www.Xtheme.org/Xtheme/)
 * [Xtheme Group Website] (http://www.Xtheme.org/)
 * [IRC](irc://irc.IRC4Fun.net/#Xtheme) irc.IRC4Fun.net #Xtheme
 * [IRC](irc://chat.freenode.net/#Xtheme) chat.Freenode.net #Xtheme
