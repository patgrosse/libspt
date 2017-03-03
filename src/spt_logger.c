/*
 *  libspt - Serial Packet Transfer Library
 *
 *  Copyright (C) 2017 Patrick Grosse <patrick.grosse@uni-muenster.de>
 */

#include <spt_logger.h>

#include <time.h>
#include <stdio.h>
#include <stdarg.h>

void spt_log_msg(const char *category, const char *msg, ...) {
    time_t rawtime;
    struct tm *timeinfo;
    char timebuf[21];
    char buf[2000];

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    va_list vl;
    va_start(vl, msg);
    vsprintf(buf, msg, vl);
    strftime(timebuf, 21, "%Y-%m-%dT%H:%M:%SZ", timeinfo);
    printf("%s [INFO] %s - %s", timebuf, category, buf);
    fflush(stdout);
}