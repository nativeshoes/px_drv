```
$ md5sum /lib/modules/$(uname -r)/kernel/drivers/video/pxq3pe_dtv.ko
d99ee6147c36721683d229908abcd7a7

$ modinfo /lib/modules/$(uname -r)/kernel/drivers/video/pxq3pe_dtv.ko
filename:       /lib/modules/2.6.32-573.el6.x86_64/kernel/drivers/video/pxq3pe_dtv.ko
license:        GPL
author:
description:    PX-Q3PE driver module
srcversion:     A6E2A26B94BF482246AB236
alias:          pci:v0000188Bd00005220sv00000B06sd00000002bc*sc*i*
depends:
vermagic:       2.6.32-279.el6.x86_64 SMP mod_unload modversions
parm:           irq_debug:enable debug messages [IRQ handler] (int)
parm:           vid_limit:capture memory limit in megabytes (int)
```

## 1. Clone repository

```
$ git clone https://github.com/project-pp/px_drv.git
```

## 2. Configure persistent module loading

refer: https://access.redhat.com/documentation/en-US/Red_Hat_Enterprise_Linux/6/html/Deployment_Guide/sec-Persistent_Module_Loading.html

```
$ sudo ln -s $(pwd)/px_drv/driver/pxq3pe_dtv.ko /lib/modules/$(uname -r)/kernel/drivers/video/pxq3pe_dtv.ko
$ sudo vi /etc/sysconfig/modules/pxq3pe_dtv.modules
#!/bin/sh

if [ ! -c /dev/input/pxq3pe0 ] ; then
    exec /sbin/modprobe pxq3pe_dtv >/dev/null 2>&1
fi

$ sudo chmod +x /etc/sysconfig/modules/pxq3pe_dtv.modules

# load module
$ /etc/sysconfig/modules/pxq3pe_dtv.modules
```

## 3. Build recpt1

Install [libarib25](https://github.com/stz2012/libarib25) before build recpt1.

```
$ cd px_drv/recpt1
$ bash autogen.sh
$ ./configure --enable-b25
$ make && sudo make install
```

## 4. Try it now!

Setup your card reader to enable b25 decode.
This board can't seems to supply LNB on Linux.
Enable LNB from other tuner.

```
$ recpt1 --help

Usage:
recpt1 [--b25 [--round N] [--strip] [--EMM]] [--udp [--addr hostname --port portnumber]] [--http portnumber] [--device devicefile] [--lnb voltage] [--sid SID1,SID2] channel rectime destfile

Remarks:
if rectime  is '-', records indefinitely.
if destfile is '-', stdout is used for output.

Options:
--b25:               Decrypt using BCAS card
  --round N:         Specify round number
  --strip:           Strip null stream
  --EMM:             Instruct EMM operation
--udp:               Turn on udp broadcasting
  --addr hostname:   Hostname or address to connect
  --port portnumber: Port number to connect
--http portnumber:   Turn on http broadcasting (run as a daemon)
--device devicefile: Specify devicefile to use
--lnb voltage:       Specify LNB voltage (0, 11, 15)
--sid SID1,SID2,...: Specify SID number in CSV format (101,102,...)
--help:              Show this help
--version:           Show version
--list:              Show channel list
```

## 5. Contact

via github issue.
