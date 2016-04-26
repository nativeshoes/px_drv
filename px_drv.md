# how to install

## 1. clone this repository

```
$ git clone https://github.com/project-pp/px_drv.git
```

## 2. load driver and place config file for persistent module loading

```
$ cd px_drv/driver
$ sudo insmod pxq3pe_dtv.ko

$ vi pxq3pe_dtv.modules

# line.3; fix .ko path
/sbin/insmod /path/to/pxq3pe_dtv.ko

$ sudo cp pxq3pe_dtv.modules /etc/sysconfig/modules/
```

## 3. build recpt1

pls install [libarib25](https://github.com/stz2012/libarib25) and build tools before this section.

```
$ cd ../recpt1
$ bash autogen.sh
$ ./configure --enable-b25
$ make
$ sudo make install
```

## 4. test

pls setup your card reader.
this board can't seems to supply LNB when use linux driver.
pls check LNB supply status from other board/device/booster/tv/tuner.

```
# GR
$ recpt1 --b25 --strip 27 - test27.ts

# BS
$ recpt1 --b25 --strip 101 - test101.ts

# http broadcast
$ recpt1 --http 8888
```

## 5. reboot

pls reboot. then check driver and recpt1 works fine :)

:):):):):)

## 6. contact

if you have a question, pls open new issue.
