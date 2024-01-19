#include "getopt_impl.h"
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

int opterr = -1;

int optopt;

int optind = 1;

char *optarg = NULL;

int getopt_impl(int argc, char *const *argv, const char *optstring)
{
    static int sp = 1;
    int c;
    const char *cp;
    if (sp == 1)
    {
        if (optind >= argc || !argv[optind] || argv[optind][0] != '-') return -1;

        if (argv[optind][1] == '\0')
        {
            return -1;
        }
        else if (argv[optind][1] == '-')
        {
            optind++;
            return -1;
        }
        else
        {
        }
    }

    c = argv[optind][sp];
    optopt = argv[optind][sp];

    if (c == ':' ||                      //
        !(cp = strchr(optstring, c)) ||  //
        !isalnum(c) ||                   //
        0)
    {
        if (opterr && optstring[0] != ':')
        {
            fprintf(stderr, "%s: illegal option -- %c\n", argv[0], c);
        }
        if (argv[optind][++sp] == '\0')
        {
            optind++;
            sp = 1;
        }
        return '?';
    }

    if (*++cp == ':')
    {
        if (argv[optind][sp + 1] != '\0')
            optarg = &argv[optind++][sp + 1];
        else if (++optind >= argc)
        {
            if (opterr && optstring[0] != ':')
            {
                fprintf(stderr, "%s: option requires an argument -- %c\n", argv[0], c);
            }
            sp = 1;
            return optstring[0] == ':' ? ':' : '?';
        }
        else
        {
            optarg = argv[optind++];
        }
        sp = 1;
    }
    else
    {
        if (argv[optind][++sp] == '\0')
        {
            sp = 1;
            optind++;
        }
        optarg = NULL;
    }

    return c;
}
