/*
 *  libspt - Serial Packet Transfer Library
 *
 *  Copyright (C) 2017 Patrick Grosse <patrick.grosse@uni-muenster.de>
 */

#include <serial_io.h>

#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <memory.h>

#include <unused.h>

#ifndef NO_LIBEVENT

#include <event.h>

#endif

bool runningbit = true;

void *run_output_data(void *arg) {
    struct serial_io_context *sictx = arg;
    while (runningbit) {
        ssize_t readbytes = fifo_read(sictx->read_fifo, sictx->print_buffer, sictx->print_buffer_size);
        if (readbytes == -1) {
            printf("Error while reading from FIFO!\n");
            return NULL;
        }
        if (sictx->data_received != NULL) {
            sictx->print_buffer[readbytes] = '\0';
            sictx->data_received((size_t) readbytes, sictx->print_buffer, sictx->data_received_arg);
        }
    }
    return NULL;
}

void read_to_fifo(int fd, short events, void *arg) {
    UNUSED(events);
    struct serial_io_context *sictx = arg;
    ssize_t ret = read(fd, sictx->read_buffer, sictx->read_buffer_size);
    if (ret == 0) {
        printf("\nSerial data stream closed\n");
    } else if (ret == -1) {
        if (errno != EINTR) {
            perror("\nError received while reading from serial");
        }
    } else {
        if (fifo_write(sictx->read_fifo, sictx->read_buffer, (unsigned int) ret) == -1) {
            printf("\nError while writing to FIFO\n");
        } else {
            return;
        }
    }
    runningbit = false;
#ifndef NO_LIBEVENT
    event_base_loopbreak(sictx->ebase);
#endif
}

#ifndef NO_LIBEVENT

void event_interrupted(int sig) {
    UNUSED(sig);
}

#endif

void *run_event_dispatcher(void *arg) {
    struct serial_io_context *sictx = arg;
#ifdef NO_LIBEVENT
    while (runningbit) {
        read_to_fifo(sictx->fd_in, 0, sictx);
    }
#else
    struct sigaction sa;
    bzero(&sa, sizeof(sa));
    sa.sa_handler = event_interrupted;
    sigaction(SIGUSR1, &sa, NULL);
    sictx->ebase = event_base_new();
    sictx->readev = event_new(sictx->ebase, sictx->fd_in, EV_READ | EV_PERSIST, read_to_fifo, sictx);
    event_add(sictx->readev, NULL);
    event_base_dispatch(sictx->ebase);
    event_free(sictx->readev);
    event_base_free(sictx->ebase);
#endif
    return NULL;
}

void serial_io_context_init(struct serial_io_context *sictx, int fd_in, int fd_out) {
    sictx->fd_in = fd_in;
    sictx->fd_out = fd_out;
    sictx->cache_buffer_size = DEFAULT_CACHE_BUFFER_SIZE;
    sictx->print_buffer_size = DEFAULT_PRINT_BUFFER_SIZE;
    sictx->read_buffer_size = DEFAULT_READ_BUFFER_SIZE;
    sictx->data_received = NULL;
    sictx->data_received_arg = NULL;
}

int8_t serial_io_dispatch_start(struct serial_io_context *sictx) {
    if (sictx->fd_in < 0) {
        errno = EBADF;
        perror("Incorrect in file descriptor given");
        return -1;
    }
    if (sictx->fd_out < 0) {
        errno = EBADF;
        perror("Incorrect out file descriptor given");
        return -1;
    }
    sictx->read_fifo = malloc(sizeof(fifo_t));
    if (sictx->read_fifo == NULL) {
        errno = ENOMEM;
        perror("Could not allocate size for FIFO");
        return -1;
    }
    if (fifo_init(sictx->read_fifo, sictx->cache_buffer_size)) {
        return -1;
    }

    sictx->read_buffer = malloc(sictx->read_buffer_size);
    sictx->print_buffer = malloc(sictx->print_buffer_size + 1);

    pthread_create(&sictx->print_thread, NULL, run_output_data, sictx);
    pthread_create(&sictx->event_thread, NULL, run_event_dispatcher, sictx);
    return 0;
}

int8_t serial_io_write(struct serial_io_context *sictx, size_t len, const void *data) {
    size_t totalwritten = 0;
    ssize_t ret;
    while (totalwritten < len) {
        ret = write(sictx->fd_out, data + totalwritten, len - totalwritten);
        if (ret == -1) {
            return -1;
        }
        totalwritten += ret;
    }
    return 0;
}

int8_t serial_io_dispatch_stop(struct serial_io_context *sictx) {
#ifndef NO_LIBEVENT
    event_base_loopbreak(sictx->ebase);
    pthread_kill(sictx->event_thread, SIGUSR1);
#else
    pthread_cancel(sictx->event_thread);
#endif
    pthread_join(sictx->event_thread, NULL);
    pthread_cancel(sictx->print_thread);
    pthread_join(sictx->print_thread, NULL);
    free(sictx->read_buffer);
    free(sictx->print_buffer);
    close(sictx->fd_out);
    if (sictx->fd_out != sictx->fd_in) {
        close(sictx->fd_in);
    }
    fifo_destroy(sictx->read_fifo);
    free(sictx->read_fifo);
    return 0;
}