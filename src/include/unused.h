/*
 *  libspt - Serial Packet Transfer Library
 *
 *  Copyright (C) 2017 Patrick Grosse <patrick.grosse@uni-muenster.de>
 */

/**
 * @brief   macros for unused symbols
 * @file    unused.h
 * @author  Patrick Grosse <patrick.grosse@uni-muenster.de>
 */

#ifndef LIBSPT_UNUSED_H
#define LIBSPT_UNUSED_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Prevents an unused variable from generating compiler warnings
 */
#define UNUSED(arg) (void) arg

#ifdef __cplusplus
}
#endif

#endif //LIBSPT_UNUSED_H
