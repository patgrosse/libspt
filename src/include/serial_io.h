/*
 *  libspt - Serial Packet Transfer Library
 *
 *  Copyright (C) 2017 Patrick Grosse <patrick.grosse@uni-muenster.de>
 */

/**
 * @brief   input/output management for serial data streams
 * @file    serial_io.h
 * @author  Patrick Grosse <patrick.grosse@uni-muenster.de>
 */

#ifndef LIBSPT_SERIAL_IO_H
#define LIBSPT_SERIAL_IO_H

#include <pthread.h>

#include <data_fifo.h>

/**
 * The default cache size for the internal cache
 */
#define DEFAULT_CACHE_BUFFER_SIZE 2000
/**
 * The default buffer size for the data output
 */
#define DEFAULT_PRINT_BUFFER_SIZE 200
/**
 * The default cache size for a single read
 */
#define DEFAULT_READ_BUFFER_SIZE 200

/**
 * Callback for data being received from the serial file descriptor
 */
typedef void (*serial_io_receive_cb)(const size_t, const char *, void *arg);

/**
 * @brief Context of a serial input/output
 */
struct serial_io_context {
    /* user variables */
    /** @brief Input file descriptor */
    int fd_in;
    /** @brief Output file descriptor */
    int fd_out;
    /** @brief Size of the cache buffer */
    size_t cache_buffer_size;
    /** @brief Size of the print buffer */
    size_t print_buffer_size;
    /** @brief Size of the read buffer */
    size_t read_buffer_size;

    /* user callback */
    /** @brief Callback function */
    serial_io_receive_cb data_received;
    /** @brief Parameter to pass to the function */
    void *data_received_arg;

    /* internal variables */
    /** @brief Buffer for outputs */
    char *print_buffer;
    /** @brief Buffer for inputs */
    char *read_buffer;
    /** @brief Thread for reads */
    pthread_t event_thread;
    /** @brief Thread for outputs */
    pthread_t print_thread;
    /** @brief FIFO for the internal cache */
    fifo_t *read_fifo;
#ifndef NO_LIBEVENT
    /** @brief The libevent event base for reads */
    struct event_base *ebase;
    /** @brief The libevent event for the input fd */
    struct event *readev;
#endif
};

/**
 * Initialize a serial_io_context, no resources will be allocated yet
 * @param sictx The context to initialize
 * @param fd_in The file descriptor of the input stream
 * @param fd_out The file descriptor of the output stream
 */
void serial_io_context_init(struct serial_io_context *sictx, int fd_in, int fd_out);

/**
 * Start the background dispatcher threads after allocating the required resources
 * @param sictx The context for what the threads should be started
 * @return 0 on success
 */
int8_t serial_io_dispatch_start(struct serial_io_context *sictx);

/**
 * Output to the serial output stream
 * @param sictx The context with the output file descriptor
 * @param len The length of the data to output
 * @param data The data buffer
 * @return 0 on success
 */
int8_t serial_io_write(struct serial_io_context *sictx, size_t len, const void *data);

/**
 * Cancel the threads of a serial context
 * @param sictx The context with the running background threads
 * @return 0 on success
 */
int8_t serial_io_dispatch_stop(struct serial_io_context *sictx);

#endif //LIBSPT_SERIAL_IO_H
