# tainted - About

tainted is a command-line (CLI) tool to get the current Linux Kernel taint value and print each set bit in human readable format.

(C) 2014 - Nikolay Aleksandrov <nikolay@redhat.com> (Version 1.0)

(c) 2022, 2024 - Ralph Roth (Version 2.x.x) - enhancements and fixes for SLES 12, 15, openSUSE

Homepage of version 2.x.x = <https://github.com/roseswe/tainted>

The Linux kernel maintains a "tainted state" that is included in kernel error messages. The tainted state provides information about whether something has happened to the running kernel that affects whether a kernel error or hang can be effectively fixed by analyzing the kernel source code. Some of the information in the taint relates to whether the information provided by the kernel in the error message can be trusted.

The current taint value is extracted by `tainted` from /proc/sys/kernel/tainted or can be passed on the command line with the option -x

NOTE: SLES 12/SLES 15/openSUSE 15 sets the bit 30? (N) of the tainted value. The program has been adapted to scope with this special behavior (tainted version 2.0.6++). See also the SUSE Technical Information Document (TID) #000016321 (3582750) -- Tainted kernel - <https://www.suse.com/support/kb/doc/?id=000016321> and /usr/src/linux/Documentation/sysctl/kernel.txt | <https://docs.kernel.org/admin-guide/tainted-kernels.html> | <https://www.kernel.org/doc/Documentation/admin-guide/tainted-kernels.rst>

## How to build

A Makefile is provided for gcc

    $ make
    cc -c -Wall tainted.c -o tainted.o
    cc  tainted.o -o tainted

## Usage

    $ ./tainted -?
    tainted [-?hix value] Version 2.0.9 (May 17 2024)
    Without command-line options this tool will print the
    current taint value (from proc FS) with information about each set bit.
    -h -?    - this help
    -i -l    - print information about the different taint bits (list)
    -x value - print taint information using value instead


    $ ./tainted -x 2147488256
    Taint value (original): 2147488256 0x80001200  (cleared): 4608 0x00001200 (0000000001001000000000)
    [F/bit] [bit val] [description]
    W   9  512       A kernel warning has occurred
    O  12  4096      An out-of-tree module has been loaded

    $ ./tainted -x 3221237761
    Taint value (original): 3221237761, Hex: 0xc0003001
    Binary: [011000000000000000011000000000001] Hex cleared: 0x00003001
    [F/bit] [bit val]   [description]
    P   0  1           (G/P) A module with a non-GPL license has been loaded,
                        this includes modules with no license.
                        Set by modutils >= 2.4.9 and module-init-tools
    O  12  4096        An out-of-tree module has been loaded (not shipped with Kernel)
    E  13  8192        An unsigned module has been loaded in a kernel supporting
                        module signature
    N  30  1073741824  Unsupported modules loaded, maybe Public Cloud (Azure/AWS) specific?
                        Maybe not YES! certified?
    ?  31  2147483648  SUSE special????

    $ cat /proc/sys/kernel/tainted
    2147561472

    $ ./tainted
    Taint value (original): 2147561472, Hex: 0x80013000
    Binary: [010000000000000010011000000000000] Hex cleared: 0x00013000
    [F/bit] [bit val]   [description]
    O  12  4096        An out-of-tree module has been loaded
    E  13  8192        An unsigned module has been loaded in a kernel supporting
                        module signature
    X  16  65536       Auxiliary taint, defined for and used by distros
    N  31  2147483648  SUSE: An unsupported kernel module was loaded

-----------------------------------------------------------------------------

    Taint value (original): 18446744073709551615, Hex: 0xffffffffffffffff
    Binary: [111111111111111111111111111111111] Hex cleared: 0x0001ffff
    [F/bit] [bit val]   [description]
    P   0  1           (G/P) A module with a non-GPL license has been loaded,
                        this includes modules with no license.
                        Set by modutils >= 2.4.9 and module-init-tools
    F   1  2           A module was force loaded by insmod -f
    S   2  4           Unsafe SMP processors: SMP with CPUs not designed for SMP
    R   3  8           A module was forcibly unloaded from the system by rmmod -f
    M   4  16          A hardware machine check error (MCE) occurred on the system
    B   5  32          A bad page was discovered on the system
    U   6  64          (N/U) The user has asked that the system be marked "tainted".
                        This could be because they are running software that
                        directly modifies the hardware, or for other reasons
    D   7  128         The system has died
    A   8  256         The ACPI DSDT has been overridden with one supplied by the
                        user instead of using the one provided by the hardware
    W   9  512         A kernel warning has occurred (kernel issued warning)
    C  10  1024        A module from drivers/staging was loaded
    I  11  2048        The system is working around a severe firmware bug
    O  12  4096        An out-of-tree module has been loaded
    E  13  8192        An unsigned module has been loaded in a kernel supporting
                        module signature
    L  14  16384       A soft lockup has previously occurred on the system
    K  15  32768       The kernel has been live patched
    X  16  65536       Auxiliary taint, defined for and used by distros
    T  17  131072      Kernel was built with the struct randomization plugin
    _  18  262144      (null)
    _  19  524288      (null)
    _  20  1048576     (null)
    _  21  2097152     (null)
    _  22  4194304     (null)
    _  23  8388608     (null)
    _  24  16777216    (null)
    _  25  33554432    (null)
    _  26  67108864    (null)
    _  27  134217728   (null)
    _  28  268435456   (null)
    _  29  536870912   (null)
    N  30  1073741824  Unsupported modules loaded, maybe Public Cloud (Azure/AWS) specific?
                        Maybe not YES! certified?
    ?  31  2147483648  SUSE special????

-----------------------------------------------------------------------------

### Note for Kernel taints coming from DRBD (4096, O) under SLES

    2024-09-18T12 38 41.392033+02 00 host kernel  [ 5053.593052][T328362] drbd  loading out-of-tree module taints kernel.

Workaround:

    zypper remove drbd-kmp-default

<!--
$Id: README.md,v 1.7 2024/09/18 14:12:06 ralph Exp $
vim:set fileencoding=utf8 fileformat=unix filetype=text tabstop=2 expandtab:
 -->