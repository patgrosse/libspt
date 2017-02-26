/*
 *  libspt - Serial Packet Transfer Library
 *
 *  Copyright (C) 2017 Patrick Grosse <patrick.grosse@uni-muenster.de>
 */

/**
 * @brief   logging functions
 * @file    spt_logger.h
 * @author  Patrick Grosse <patrick.grosse@uni-muenster.de>
 */

#ifndef LIBSPT_LOGGER_H
#define LIBSPT_LOGGER_H

/**
 * Log a message printf style
 * @param category A category for the message (eg. the thread name)
 * @param msg The format of the message
 * @param ... The format arguments
 */
void spt_log_msg(const char *category, const char *msg, ...);

#endif //LIBSPT_LOGGER_H
