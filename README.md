# About #

The libnss_tld_mapper library allows you to force domain names ending in .dev to resolve
to the the loopback address without requiring root access. This is accomplished by using
LD_PRELOAD to install this library as an additional NSS lookup database.

# Supported Platforms #

Presently only known to work on GNU/Linux, although any platform using GLibc's NSS implementation
should be compatible.

# Installation #

Download the latest release tarball, extract it, and do a typical autotools compile:

    $ ./configure
    $ make
    $ sudo make install

For all the gory details, please see the INSTALL file.

# Usage #

Run the program whose name resolution you would like to modify with libnss_tld_mapper in it's
LD_PRELOAD, for example:

    $ LD_PRELOAD="libnss_tld_mapper.so.2 ${LD_PRELOAD}" curl futilepleasures.dev

Several environment variables may be used to influence the behavior of libnss_tld_mapper:

* TLD_MAPPER_TLD: The TLD that the mapper should use, defaults to '.dev'.
* TLD_MAPPER_SERVICES: The hosts service line that libnss_tld_mapper should use when inserting itself
  as a DNS resolver service. As the NSS API provides no mechanism for reading the current resolver service
  configuration, this is set to 'files dns' by default. If this environment variable is set, its contents
  will be appended to rather then the default. For example, running

    $ export TLD_MAPPER_SERVICES `sed -n 's/^hosts: *\(.*\)$/\1/p' /etc/nsswitch.conf`

  would cause subsequent usages of the library to use the systems resolver configuration.