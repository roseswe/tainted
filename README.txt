
tainted - CLI tool to get the current Linux Kernel taint value and print each set bit in human readable format

(C) 2014 - Nikolay Aleksandrov <nikolay@redhat.com> (Version 1.0)
(c) 2022 - Ralph Roth - emhancements and fixes for SLES 12, 15, openSUSE (Version 2.0)

Homepage of version 2.x = https://github.com/roseswe/tainted

Completely free, no license whatsoever.

The current taint value is extracted from /proc/sys/kernel/tainted

-----------------------------------------------------------------------------

How to build

$ make
cc -c -Wall tainted.c -o tainted.o
cc  tainted.o -o tainted

-----------------------------------------------------------------------------

$ ./tainted -h
./tainted [-hix value] Version 2.0 (Feb 16 2022)
Without command-line options this tool will print the
current taint value (from proc FS) with information about each set bit.
-h - this help
-i - print information about the different taint bits
-x value - print taint information using value instead

$ ./tainted -x 2147488256
Taint value: 4608 0x1200 (0000000001001000000000)
[F/bit] [bit val] [description]
 W   9  512       A kernel warning has occurred
 O  12  4096      An out-of-tree module has been loaded

$ ./tainted -x2147495936
Taint value: 12288 0x3000 (0000000011000000000000)
[F/bit] [bit val] [description]
O  12  4096      An out-of-tree module has been loaded
E  13  8192      An unsigned module has been loaded in a kernel supporting
                 module signature

$ ./tainted
Taint value: 77824 0x13000 (0000010011000000000000)
[F/bit] [bit val] [description]
O  12  4096      An out-of-tree module has been loaded
E  13  8192      An unsigned module has been loaded in a kernel supporting
                 module signature
X  16  65536     Auxiliary taint, defined for and used by distros

-----------------------------------------------------------------------------

$ ./tainted -i
Taint value: 2147483647 0x7fffffff (1111111111111111111111)
[F/bit] [bit val] [description]
 P   0  1         A module with a non-GPL license has been loaded, this
                  includes modules with no license.
                  Set by modutils >= 2.4.9 and module-init-tools
 F   1  2         A module was force loaded by insmod -f
 S   2  4         Unsafe SMP processors: SMP with CPUs not designed for SMP
 R   3  8         A module was forcibly unloaded from the system by rmmod -f
 M   4  16        A hardware machine check error occurred on the system
 B   5  32        A bad page was discovered on the system
 U   6  64        The user has asked that the system be marked "tainted". This
                  could be because they are running software that directly
                  modifies the hardware, or for other reasons
 D   7  128       The system has died
 A   8  256       The ACPI DSDT has been overridden with one supplied by the
                  user instead of using the one provided by the hardware
 W   9  512       A kernel warning has occurred
 C  10  1024      A module from drivers/staging was loaded
 I  11  2048      The system is working around a severe firmware bug
 O  12  4096      An out-of-tree module has been loaded
 E  13  8192      An unsigned module has been loaded in a kernel supporting
                  module signature
 L  14  16384     A soft lockup has previously occurred on the system
 K  15  32768     The kernel has been live patched
 X  16  65536     Auxiliary taint, defined for and used by distros
 T  17  131072    Kernel was built with the struct randomization plugin
 *  18  262144    Reserved for the future :-)
 *  19  524288    Reserved for the future :-)

// $Id: README.txt,v 1.2 2022/02/16 20:20:14 ralph Exp $
// vim:set fileencoding=utf8 fileformat=unix filetype=text tabstop=2 expandtab:
