## Xtheme

Xtheme is a *legacy* set of services for IRC networks designed for large IRC networks with high
scalability requirements.  It is relatively mature software, with some code and design
derived from another package called Atheme and Shrike.

Xtheme's behavior is tunable using modules and a highly detailed configuration file.
Almost all behavior can be changed at deployment time just by editing the configuration.

If you are running this code from Git, you should read GIT-Access for instructions on
how to fully check out the xtheme tree, as it is spread across many repositories.

## basic build instructions for the impatient

Whatever you do, make sure you do *not* install Xtheme into the same location as the source.
Xtheme will default to installing in `$HOME/xtheme`, so make sure you plan accordingly for this.

    $ git submodule update --init
    $ ./configure
    $ make
    $ make install

If you're still lost, read [INSTALL](INSTALL) or [GIT-Access](GIT-Access) for hints.

## SWI (services web interface)

If you are interested in having a Services Web Interface (or Services Web Panel), SWI
*IS* compatible with Xtheme and other Atheme forks, and is available at:
http://www.irc4fun.net/xtheme/ or http://github.com/IRC4Fun/Xtheme/

## links / contact

 * [GitHub](http://www.github.com/IRC4Fun/xtheme)
 * [Website](http://www.IRC4Fun.net/xtheme/)
 * [IRC](irc://irc.IRC4Fun.net/#Xtheme)
