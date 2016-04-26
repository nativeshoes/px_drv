# how to install

1. clone this repository

```
$ cd /usr/local/src
$ git clone https://github.com/project-pp/px_drv.git
```

2. load driver and copy config file for persistent module loading

```
$ cd px_drv/driver
$ sudo insmod pxq3pe_dtv.ko
$ sudo cp pxq3pe_dtv.modules /etc/sysconfig/modules/
```

3. build recpt1

pls install libarib25 library and build tools before this section.

```
$ cd ../recpt1
$ bash autogen.sh
$ ./configure --enable-b25
$ make
$ sudo make install
```

4. test

pls setup your card reader. check LNB supplyment.
this board can't seems to supply LNB when use linux driver.

```
# GR
$ recpt1 --b25 --strip 27 - test27.ts

# BS
$ recpt1 --b25 --strip 101 - test101.ts

# http broadcast
$ recpt1 --http 8888
```

5. reboot

pls reboot. then check driver and recpt1 works fine :)

:):):):):)

6. contact

if you have a question, pls open new issue.
