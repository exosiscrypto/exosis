Exosis Core version 0.17.x is now available from:

  <https://github.com/exosiscrypto/exosis/releases/>

This is a new minor version release, including new features, various bugfixes
and performance improvements, as well as updated translations.

Please report bugs using the issue tracker at GitHub:

  <https://github.com/exosiscrypto/exosis/issues>


How to Upgrade
==============

If you are running an older version, shut it down. Wait until it has completely
shut down (which might take a few minutes for older versions), then run the
installer (on Windows) or just copy over `/Applications/Exosis-Qt` (on Mac)
or `exosisd`/`exosis-qt` (on Linux).

If your node has a txindex, the txindex db will be migrated the first time you
run 0.17.0 or newer, which may take up to a few hours. Your node will not be
functional until this migration completes.

The first time you run version 0.15.0 or newer, your chainstate database will be converted to a
new format, which will take anywhere from a few minutes to half an hour,
depending on the speed of your machine.

Note that the block database format also changed in version 0.8.0 and there is no
automatic upgrade code from before version 0.8 to version 0.15.0 or higher. Upgrading
directly from 0.7.x and earlier without re-downloading the blockchain is not supported.
However, as usual, old wallet versions are still supported.

Downgrading warning
-------------------

Wallets created in 0.16 and later are not compatible with versions prior to 0.16
and will not work if you try to use newly created wallets in older versions. Existing
wallets that were created with older versions are not affected by this.

Compatibility
==============

Exosis Core is extensively tested on multiple operating systems using
the Linux kernel, macOS 10.10+, and Windows 7 and newer (Windows XP is not supported).

Exosis Core should also work on most other Unix-like systems but is not
frequently tested on them.

From 0.17.0 onwards macOS <10.10 is no longer supported. 0.17.0 is built using Qt 5.9.x, which doesn't
support versions of macOS older than 10.10.

Notable changes
===============

Documentation
-------------

- A new document introduces Bitcoin Core's BIP174
  [Partially-Signed Bitcoin Transactions (PSBT)](https://github.com/bitcoin/bitcoin/blob/0.17/doc/psbt.md)
  interface, which is used to allow multiple programs to collaboratively
  work to create, sign, and broadcast new transactions.  This is useful
  for offline (cold storage) wallets, multisig wallets, coinjoin
  implementations, and many other cases where two or more programs need
  to interact to generate a complete transaction.

0.17.x change log
=================

(todo)

Credits
=======

Thanks to everyone who directly contributed to this release:

(todo)

