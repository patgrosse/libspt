/*
 *  libspt - Serial Packet Transfer Library
 *
 *  Copyright (C) 2017 Patrick Grosse <patrick.grosse@uni-muenster.de>
 */
/*
 * based on http://stackoverflow.com/a/6947758
 */

/**
 * @brief   utilities for ttys
 * @file    tty_utils.h
 * @author  Patrick Grosse <patrick.grosse@uni-muenster.de>
 */

#ifndef LIBSPT_TTY_UTILS_H
#define LIBSPT_TTY_UTILS_H

#ifndef NO_TERMIOS

#include <termios.h>
#include <inttypes.h>

/**
 * Set the interface attributes for a tty file descriptor
 * @param fd The file descriptor
 * @param speed The serial speed
 * @return 0 on success
 */
int8_t set_interface_attribs(int fd, speed_t speed);

/**
 * Initialize a serial connection and set the interface attributes
 * @param fd The file descriptor of the serial connection
 * @return 0 on success
 */
int8_t init_serial_connection(const int fd);

/**
 * Open a serial tty file descriptor
 * @param portname The file name of the serial tty
 * @return A file descriptor on success, -1 otherwise
 */
int connect_serial(const char *portname);

#endif

#endif //LIBSPT_TTY_UTILS_H