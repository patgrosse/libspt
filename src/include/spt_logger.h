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
 * @param namespace A category for the message (eg. the thread name)
 * @param msg The format of the message
 * @param ... The format arguments
 */
void log_msg(const char *namespace, const char *msg, ...);

#endif //LIBSPT_LOGGER_H
