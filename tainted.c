/* $Id: tainted.c,v 1.19 2025/03/18 10:16:55 ralph Exp $
 * vim:set fileencoding=utf8 fileformat=unix filetype=c tabstop=2 noexpandtab:
 *
 * Tainted: Tool to get the current taint value and print each set bit in
 * 	        human readable format
 *
 * (C) 2014 - Nikolay Aleksandrov <nikolay@redhat.com> - https://github.com/NikAleksandrov/tainted
 * (c) 2022, 2025 - Ralph Roth - enhancements and fixes for SLES 12, 15, openSUSE - https://github.com/roseswe/tainted
 *
 * Compile with:
 * 	gcc -O2 -o tainted tainted.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define PROC_TAINTED "/proc/sys/kernel/tainted"
#define BIT(x) (1UL << x)

/* Extracted from linux/include/linux/kernel.h */
enum
{
	TAINT_PROPRIETARY_MODULE,
	TAINT_FORCED_MODULE,
	TAINT_CPU_OUT_OF_SPEC,
	TAINT_FORCED_RMMOD,
	TAINT_MACHINE_CHECK,
	TAINT_BAD_PAGE,
	TAINT_USER,
	TAINT_DIE,
	TAINT_OVERRIDEN_ACPI_TABLE,
	TAINT_WARN,
	TAINT_CRAP,
	TAINT_FIRMWARE_WORKAROUND,
	TAINT_OOT_MODULE,			 /* 12 enic, drbd etc. */
	TAINT_UNSIGNED_MODULE, /* 13 */
	TAINT_SOFTLOCKUP,
	TAINT_LIVEPATCH, /* 15 new stuff follows */
	TAINT_AUX,
	TAINT_STRUCT_RANDOM, /* 17 */
	TAINT_FUTURE_18,
	TAINT_FUTURE_19,
	/* 20 */

	/* #  define TAINT_NO_SUPPORT             30? 31?    ("N", SUSE) found 08.11.2022 with an Azure Kernel */
	TAINT_NO_SUPPORT = 30,
	TAINT_FUTURE_31 = 31,		/* Bit 31 */
};

/* Extracted from linux/Documentation/sysctl/kernel.txt */
static const char *szKernelTaintDescription[] = {
		[TAINT_PROPRIETARY_MODULE] = "(G/P) A module with a non-GPL license has been loaded,\n                    this includes modules with no license.\n                    Set by modutils >= 2.4.9 and module-init-tools",
		[TAINT_FORCED_MODULE] = "A module was force loaded by insmod -f",
		[TAINT_CPU_OUT_OF_SPEC] = "Unsafe SMP processors: SMP with CPUs not designed for SMP",
		[TAINT_FORCED_RMMOD] = "A module was forcibly unloaded from the system by rmmod -f",
		[TAINT_MACHINE_CHECK] = "A hardware machine check error (MCE) occurred on the system",
		[TAINT_BAD_PAGE] = "A bad page was discovered on the system",
		[TAINT_USER] = "(N/U) The user has asked that the system be marked \"tainted\".\n                    This could be because they are running software that \n                    directly modifies the hardware, or for other reasons",
		[TAINT_DIE] = "The system has died",
		[TAINT_OVERRIDEN_ACPI_TABLE] = "The ACPI DSDT has been overridden with one supplied by the\n                    user instead of using the one provided by the hardware",
		[TAINT_WARN] = "A kernel warning has occurred (kernel issued warning)",
		[TAINT_CRAP] = "A module from drivers/staging was loaded",
		[TAINT_FIRMWARE_WORKAROUND] = "The system is working around a severe firmware bug",
		[TAINT_OOT_MODULE] = "An out-of-tree module has been loaded (not shipped with Kernel)",
		[TAINT_UNSIGNED_MODULE] = "An unsigned module has been loaded in a kernel supporting\n                    module signature",
		[TAINT_SOFTLOCKUP] = "A soft lockup has previously occurred on the system",
		[TAINT_LIVEPATCH] = "The kernel has been live patched", /* 0x8000 - 15 new stuff follows */
		[TAINT_AUX] = "Auxiliary taint, defined for and used by distros",  /* 0x10000 */
		[TAINT_STRUCT_RANDOM] = "Kernel was built with the struct randomization plugin", /* 0x20000 */

		NULL, NULL, NULL,  /* 20 */
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, /* 29 */
		[TAINT_NO_SUPPORT] = "Unsupported modules loaded, maybe Public Cloud (Azure/AWS) specific?\n                    Maybe not YES! certified?", /* 30 0x40000000 */
		[TAINT_FUTURE_31 ] = "SUSE special???? ",  /* 31, 0x80000000 */
		NULL};

static const char *szFlags = "PFSRMBUDAWCIOELKXT____________N?";

static unsigned long read_flags(void)
{
	unsigned long ulFlags;
	char readbuf[16]; /* this is not safe/fool proof */
	int fd;

	fd = open(PROC_TAINTED, O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		return -errno;
	}
	if (read(fd, readbuf, sizeof(readbuf)) == -1)
	{
		perror("read");
		return -errno;
	}
	readbuf[sizeof(readbuf) - 1] = '\0';
	ulFlags = strtoul(readbuf, NULL, 10);
	if (errno == ERANGE || errno == EINVAL)
	{
		perror("strtoul readbuf");
		return -errno;
	}

	return ulFlags;
}

int main(int argc, char **argv)
{
	unsigned long ulFlags = 0;
	int opt, i;

	while ((opt = getopt(argc, argv, "?hilx:")) != -1)
	{
		switch (opt)
		{
		case 'i':
		case 'l':
			ulFlags = ~0; /* not zero -> -1 */
			break;
		case 'h':
		case '?':
		  /* If format strings can be influenced by an attacker, they can be exploited (CWE-134)
			printf(HELP_FMT, argv[0], __DATE__, PROC_TAINTED); */
			printf("%s [-?hix value] Version 2.2.1 (%s)\nWithout command-line options this tool will print the\n"
				"current taint value (from proc FS) with information about each set bit.\n"
				"-h -?    - this help\n"
				"-i -l    - print information about the different taint bits (list)\n"
				"-x value - print taint information using value instead\n\n"
				"Hint: Taints are stored in %s\n"
				"Sourcecode available at: https://github.com/roseswe/tainted\n",
				argv[0], __DATE__, PROC_TAINTED);
			return 0;
		case 'x':
		  /* identifier "optarg" is undefined */
			ulFlags = (unsigned long)strtoull(optarg, NULL, 0); /* we accept also hex/oct */
			if (errno == ERANGE || errno == EINVAL)
			{
				perror("strtoul command line");
				return -errno;
			}
			break;
		default:
			// printf("[!] Unknown option!\n");
			break;
		}
	}
	if (!ulFlags)
	{
		ulFlags = read_flags();
		if (errno)
			return -errno;
	}

	printf("Taint value (original): %lu, Hex: 0x%08lx\nBinary: [", ulFlags, ulFlags);
	// see 'N'-Flag: https://raw.githubusercontent.com/openSUSE/kernel/SLE15-SP4/Documentation/admin-guide/sysctl/kernel.rst
	for (i = strlen(szFlags); i >= 0; i--)
	{
		if (ulFlags & BIT(i))
			printf("1");
		else
			printf("0");
	} /* for i */
	printf("] Hex cleared: 0x%08lx\n", ulFlags & 0x1ffff);

	printf("%-5s %-11s %-64s\n", "[F/bit]", "[bit val]", "[description]");
	// was: for (i = 0; szKernelTaintDescription[i] && i <  sizeof(long int) * 8; i++)
	for (i = 0; i<32; i++)
	{
		if (ulFlags & BIT(i))
			printf(" %c %3d  %-10lu  %-64s\n",
						 szFlags[i], i, BIT(i), szKernelTaintDescription[i]);
	} /* for i */


	return 0;
}
