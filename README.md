# legacy-sputils

spUtils REXX Library for OS/2 &amp; eComStation

## Disclaimer

This code was found on one of my harddisk. Since I haven't touch it for about 10 years,
I don't know in which state it is in. Please let me know if it compiles and works as
expected. Then I would delete this disclaimer.

## Prerequisites 

I don't remember :-(

My guess:

- OS/2 WarpServer for eBusiness or eComStation
- OS/2 4.5 Developer Toolkit
- OpenWatcom 1.1
- Netlabs XWorkplace Helpers
- GNU make

Netlabs ODIN32 should not be required any more.

## Building

- Edit config.mif and fix the paths
- Set up environment variables:
	1. Call setvars.cmd in the openwatcom directory
	2. Call setenv.cmd in the os2 toolkit directory
	3. Set xwphelpers to the path where the netlabs xwphelpers are installed
- Run `make clean all`

## Binary Package

There is a package on [hobbes](http://hobbes.nmsu.edu) which should roughly match this 
version: [sputils01.zip](http://hobbes.nmsu.edu/pub/os2/dev/rexx/sputils01.zip).

