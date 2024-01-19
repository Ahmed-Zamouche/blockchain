#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined(__APPLE__)

#ifdef __cplusplus
extern "C"
{
#endif

    //@see: https://pubs.opengroup.org/onlinepubs/9699919799/functions/getopt.html
    extern int opterr;

    extern int optopt;

    extern int optind;

    extern char *optarg;

    int getopt_impl(int argc, char *const *argv, const char *options);

    static inline int getopt(int argc, char *const *argv, const char *options)
    {
        return getopt_impl(argc, argv, options);
    }

#ifdef __cplusplus
}
#endif
#else
#include <getopt.h>
#endif
