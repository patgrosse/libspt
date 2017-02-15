/*
 *  libspt - Serial Packet Transfer Library
 *
 *  Copyright (C) 2017 Patrick Grosse <patrick.grosse@uni-muenster.de>
 */
/*
 * based on http://stackoverflow.com/a/6947758
 */

#ifndef LIBSPT_TTY_UTILS_H
#define LIBSPT_TTY_UTILS_H

#ifndef NO_TERMIOS

#include <termios.h>

int set_interface_attribs(int fd, speed_t speed);

int init_serial_connection(const int fd);

int connect_serial(char *portname);

#endif

#endif //LIBSPT_TTY_UTILS_H