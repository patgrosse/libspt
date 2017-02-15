/*
 *  libspt - Serial Packet Transfer Library
 *
 *  Copyright (C) 2017 Patrick Grosse <patrick.grosse@uni-muenster.de>
 */

#ifndef LIBSPT_SERIAL_IO_H
#define LIBSPT_SERIAL_IO_H

#include <pthread.h>

#include <data_fifo.h>

#define DEFAULT_CACHE_BUFFER_SIZE 2000
#define DEFAULT_PRINT_BUFFER_SIZE 200
#define DEFAULT_READ_BUFFER_SIZE 200

/**
 * Callback for data being received from the serial file descriptor
 */
typedef void (*serial_io_receive_cb)(const size_t, const char *, void *arg);

/**
 * Context of a serial input/output
 */
struct serial_io_context {
    /* user variables */
    int fd_in;
    int fd_out;
    size_t cache_buffer_size;
    size_t print_buffer_size;
    size_t read_buffer_size;

    /* user callback */
    serial_io_receive_cb data_received;
    void *data_received_arg;

    /* internal variables */
    char *print_buffer;
    char *read_buffer;
    pthread_t event_thread;
    pthread_t print_thread;
    fifo_t *read_fifo;
#ifndef NO_LIBEVENT
    struct event_base *ebase;
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
