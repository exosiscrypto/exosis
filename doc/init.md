Sample init scripts and service configuration for exosisd
==========================================================

Sample scripts and configuration files for systemd, Upstart and OpenRC
can be found in the contrib/init folder.

    contrib/init/exosisd.service:    systemd service unit configuration
    contrib/init/exosisd.openrc:     OpenRC compatible SysV style init script
    contrib/init/exosisd.openrcconf: OpenRC conf.d file
    contrib/init/exosisd.conf:       Upstart service configuration file
    contrib/init/exosisd.init:       CentOS compatible SysV style init script

Service User
---------------------------------

All three Linux startup configurations assume the existence of a "exosis" user
and group.  They must be created before attempting to use these scripts.
The macOS configuration assumes exosisd will be set up for the current user.

Configuration
---------------------------------

At a bare minimum, exosisd requires that the rpcpassword setting be set
when running as a daemon.  If the configuration file does not exist or this
setting is not set, exosisd will shutdown promptly after startup.

This password does not have to be remembered or typed as it is mostly used
as a fixed token that exosisd and client programs read from the configuration
file, however it is recommended that a strong and secure password be used
as this password is security critical to securing the wallet should the
wallet be enabled.

If exosisd is run with the "-server" flag (set by default), and no rpcpassword is set,
it will use a special cookie file for authentication. The cookie is generated with random
content when the daemon starts, and deleted when it exits. Read access to this file
controls who can access it through RPC.

By default the cookie is stored in the data directory, but it's location can be overridden
with the option '-rpccookiefile'.

This allows for running exosisd without having to do any manual configuration.

`conf`, `pid`, and `wallet` accept relative paths which are interpreted as
relative to the data directory. `wallet` *only* supports relative paths.

For an example configuration file that describes the configuration settings,
see `share/examples/exosis.conf`.

Paths
---------------------------------

### Linux

All three configurations assume several paths that might need to be adjusted.

Binary:              `/usr/bin/exosisd`  
Configuration file:  `/etc/exosis/exosis.conf`  
Data directory:      `/var/lib/exosisd`  
PID file:            `/var/run/exosisd/exosisd.pid` (OpenRC and Upstart) or `/var/lib/exosisd/exosisd.pid` (systemd)  
Lock file:           `/var/lock/subsys/exosisd` (CentOS)  

The configuration file, PID directory (if applicable) and data directory
should all be owned by the exosis user and group.  It is advised for security
reasons to make the configuration file and data directory only readable by the
exosis user and group.  Access to exosis-cli and other exosisd rpc clients
can then be controlled by group membership.

### macOS

Binary:              `/usr/local/bin/exosisd`  
Configuration file:  `~/Library/Application Support/Exosis/exosis.conf`  
Data directory:      `~/Library/Application Support/Exosis`  
Lock file:           `~/Library/Application Support/Exosis/.lock`  

Installing Service Configuration
-----------------------------------

### systemd

Installing this .service file consists of just copying it to
/usr/lib/systemd/system directory, followed by the command
`systemctl daemon-reload` in order to update running systemd configuration.

To test, run `systemctl start exosisd` and to enable for system startup run
`systemctl enable exosisd`

NOTE: When installing for systemd in Debian/Ubuntu the .service file needs to be copied to the /lib/systemd/system directory instead.

### OpenRC

Rename exosisd.openrc to exosisd and drop it in /etc/init.d.  Double
check ownership and permissions and make it executable.  Test it with
`/etc/init.d/exosisd start` and configure it to run on startup with
`rc-update add exosisd`

### Upstart (for Debian/Ubuntu based distributions)

Upstart is the default init system for Debian/Ubuntu versions older than 15.04. If you are using version 15.04 or newer and haven't manually configured upstart you should follow the systemd instructions instead.

Drop exosisd.conf in /etc/init.  Test by running `service exosisd start`
it will automatically start on reboot.

NOTE: This script is incompatible with CentOS 5 and Amazon Linux 2014 as they
use old versions of Upstart and do not supply the start-stop-daemon utility.

### CentOS

Copy exosisd.init to /etc/init.d/exosisd. Test by running `service exosisd start`.

Using this script, you can adjust the path and flags to the exosisd program by
setting the EXOSISD and FLAGS environment variables in the file
/etc/sysconfig/exosisd. You can also use the DAEMONOPTS environment variable here.

### macOS

Copy org.exosis.exosisd.plist into ~/Library/LaunchAgents. Load the launch agent by
running `launchctl load ~/Library/LaunchAgents/org.exosis.exosisd.plist`.

This Launch Agent will cause exosisd to start whenever the user logs in.

NOTE: This approach is intended for those wanting to run exosisd as the current user.
You will need to modify org.exosis.exosisd.plist if you intend to use it as a
Launch Daemon with a dedicated exosis user.

Auto-respawn
-----------------------------------

Auto respawning is currently only configured for Upstart and systemd.
Reasonable defaults have been chosen but YMMV.
