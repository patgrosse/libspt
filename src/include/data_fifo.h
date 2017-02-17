/*
 *  libspt - Serial Packet Transfer Library
 *
 *  Copyright (C) 2017 Patrick Grosse <patrick.grosse@uni-muenster.de>
 */
/*
 * based on https://stratifylabs.co/embedded%20design%20tips/2013/10/02/Tips-A-FIFO-Buffer-Implementation/
 */

/**
 * @brief   data FIFO queue
 * @file    data_fifo.h
 * @author  Patrick Grosse <patrick.grosse@uni-muenster.de>
 */

#ifndef LIBSPT_CHAR_FIFO_H
#define LIBSPT_CHAR_FIFO_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Represents a circular FIFO queue
 */
typedef struct {
    /** @brief The internal buffer */
    char *buf;
    /** @brief Head index */
    size_t head;
    /** @brief Tail index */
    size_t tail;
    /** @brief Size of the queue */
    size_t size;
    /** @brief Indicator if the FIFO is full */
    bool full;
    /** @brief Mutex for mutual exclusion */
    pthread_mutex_t mutex;
    /** @brief Condition that is signaled when data becomes available */
    pthread_cond_t data_available;
} fifo_t;

/**
 * This initializes the FIFO structure with the given buffer and size
 * @param f The fifo structure to initialize
 * @param size The size of the FIFO queue
 * @return 0 on success
 */
int8_t fifo_init(fifo_t *f, size_t size);

/**
 * This reads bytes bytes from the FIFO
 * @param f The FIFO structure to read from
 * @param buf The buffer where to read the bytes into
 * @param bytes The maximum number of bytes to read
 * @return The number of bytes read, -1 on error
 */
ssize_t fifo_read(fifo_t *f, void *buf, const size_t bytes);

/**
 * Write data to the FIFO
 * If the head runs in to the tail, not all bytes are written
 * @param f The FIFO queue to append the data to
 * @param buf The buffer to read the data from
 * @param bytes The amount of bytes to be written
 * @return The amount of bytes actually written, -1 on error
 */
ssize_t fifo_write(fifo_t *f, const void *buf, const size_t bytes);

/**
 * Free the resources reserved by a FIFO structure
 * @param f The structure to free resources from
 */
void fifo_destroy(fifo_t *f);

/**
 * Check if a FIFO queue is empty
 * @param f The FIFO structure to check
 * @return True if the queue is empty
 */
bool fifo_empty(fifo_t *f);

#endif //LIBSPT_CHAR_FIFO_H
