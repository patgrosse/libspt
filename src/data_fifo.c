/*
 *  libspt - Serial Packet Transfer Library
 *
 *  Copyright (C) 2017 Patrick Grosse <patrick.grosse@uni-muenster.de>
 */
/*
 * based on https://stratifylabs.co/embedded%20design%20tips/2013/10/02/Tips-A-FIFO-Buffer-Implementation/
 */

#include <pthread.h>
#include <malloc.h>
#include <errno.h>
#include <stdio.h>

#include <data_fifo.h>

int8_t fifo_init(fifo_t *f, const size_t size) {
    if (size == 0) {
        errno = EINVAL;
        perror("Illegal FIFO size");
        return -1;
    }
    f->head = 0;
    f->tail = 0;
    f->full = false;
    f->size = size;
    f->buf = malloc(size);
    if (pthread_mutex_init(&f->mutex, NULL)) {
        perror("Error while initializing FIFO mutex");
        return -1;
    }
    if (pthread_cond_init(&f->data_available, NULL)) {
        perror("Error while initializing FIFO condition");
        return -1;
    }
    return 0;
}

ssize_t fifo_read(fifo_t *f, void *buf, const size_t bytes) {
    if (pthread_mutex_lock(&f->mutex)) {
        return -1;
    }
    ssize_t ret;
    while (fifo_empty(f)) {
        if (pthread_cond_wait(&f->data_available, &f->mutex)) {
            ret = -1;
            goto out;
        }
    }
    size_t i;
    char *p;
    p = buf;
    for (i = 0; i < bytes; i++) {
        if (f->tail != f->head || f->full) { //see if any data is available
            *p++ = f->buf[f->tail];  //grab a byte from the buffer
            f->tail++;  //increment the tail
            if (f->tail == f->size) {  //check for wrap-around
                f->tail = 0;
            }
            f->full = false;
        } else {
            ret = i; //number of bytes read
            goto out;
        }
    }
    ret = bytes;

    out:
    if (pthread_mutex_unlock(&f->mutex)) {
        return -1;
    }
    return ret;
}

ssize_t fifo_write(fifo_t *f, const void *buf, const size_t bytes) {
    if (pthread_mutex_lock(&f->mutex)) {
        return -1;
    }
    ssize_t written_bytes;
    size_t i;
    const char *p;
    p = buf;
    for (i = 0; i < bytes; i++) {
        //first check to see if there is space in the buffer
        if (f->full) {
            written_bytes = i; //no more room
            goto out;
        } else {
            f->buf[f->head] = *p++;
            f->head++;  //increment the head
            if (f->head == f->size) {  //check for wrap-around
                f->head = 0;
            }
            if (f->head == f->tail) {
                f->full = true;
            }
        }
    }
    written_bytes = bytes;
    if (written_bytes > 0) {
        if (pthread_cond_signal(&f->data_available)) {
            written_bytes = -1;
        }
    }

    out:
    if (pthread_mutex_unlock(&f->mutex)) {
        written_bytes = -1;
    }
    return written_bytes;
}

void fifo_destroy(fifo_t *f) {
    pthread_mutex_destroy(&f->mutex);
    pthread_cond_destroy(&f->data_available);
    free(f->buf);
}

bool fifo_empty(fifo_t *f) {
    return f->head == f->tail && !f->full;
}