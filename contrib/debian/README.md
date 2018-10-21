
Debian
====================
This directory contains files used to package exosisd/exosis-qt
for Debian-based Linux systems. If you compile exosisd/exosis-qt yourself, there are some useful files here.

## exosis: URI support ##


exosis-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install exosis-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your exosis-qt binary to `/usr/bin`
and the `../../share/pixmaps/exosis.png` to `/usr/share/pixmaps`

exosis-qt.protocol (KDE)

