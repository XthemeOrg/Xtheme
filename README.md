## Xtheme

Xtheme is a set of services for IRC networks designed for large IRC networks with high
scalability requirements.  It is relatively mature software, with some code and design
derived from another package called Atheme and Shrike.

Xtheme's behavior is tunable using modules and a highly detailed configuration file.
Almost all behavior can be changed at deployment time just by editing the configuration.

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
