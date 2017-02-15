/*
 *  libspt - Serial Packet Transfer Library
 *
 *  Copyright (C) 2017 Patrick Grosse <patrick.grosse@uni-muenster.de>
 */

#include <time.h>
#include <stdio.h>
#include <stdarg.h>

#include <spt_logger.h>
#include <unused.h>

void log_msg(const char *namespace, const char *msg, ...) {
    time_t rawtime;
    struct tm *timeinfo;
    char timebuf[21];
    char buf[2000];

    UNUSED(timebuf);
    UNUSED(buf);

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    UNUSED(timeinfo);
    va_list vl;
    va_start(vl, msg);
    vsprintf(buf, msg, vl);
    strftime(timebuf, 21, "%Y-%m-%dT%H:%M:%SZ", timeinfo);
    printf("%s [INFO] %s - %s", timebuf, namespace, buf);
    fflush(stdout);
}