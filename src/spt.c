/*
 *  libspt - Serial Packet Transfer Library
 *
 *  Copyright (C) 2017 Patrick Grosse <patrick.grosse@uni-muenster.de>
 */

#include <stdio.h>
#include <memory.h>
#include <malloc.h>
#include <errno.h>
#include <stdlib.h>

#include <spt.h>
#include <spt_logger.h>
#include <base64.h>
#include <netinet/in.h>

ssize_t index_of_char(const char *haystack, const size_t size, const char needle) {
    for (size_t i = 0; i < size; i++) {
        if (*(haystack + i) == needle) {
            return i;
        }
    }
    return -1;
}

int8_t process_data(struct spt_context *sptctx, char *workbuf, size_t len);

int8_t process_packet(struct spt_context *sptctx, char *workbuf, size_t len) {
    if (!sptctx->processing_packet) {
        sptctx->processing_packet = true;
        sptctx->recv_base64_len_bytes_amount = 0;
        sptctx->recv_base64_len_bytes = malloc(base64_required_size_in_base64(sizeof(base64_len_t)));
        if (sptctx->recv_base64_len_bytes == NULL) {
            return ENOMEM;
        }
        sptctx->expected_base64_data_bytes_amount = 0;
        sptctx->received_base64_data_bytes_amount = 0;
        sptctx->received_base64_data_bytes = NULL;
    }
    if (len == 0) {
        return 0;
    }
    if (sptctx->recv_base64_len_bytes_amount == base64_required_size_in_base64(sizeof(base64_len_t))) {
        for (size_t read_index = 0; read_index < len; read_index++) {
            sptctx->received_base64_data_bytes[sptctx->received_base64_data_bytes_amount] = (unsigned char) workbuf[read_index];
            sptctx->received_base64_data_bytes_amount++;
            if (sptctx->received_base64_data_bytes_amount == sptctx->expected_base64_data_bytes_amount) {
                struct serial_data_packet *pkt = malloc(sizeof(struct serial_data_packet));
                if (pkt == NULL) {
                    return ENOMEM;
                }
                size_t real_data_buffer_size = base64_required_size_from_base64(
                        sptctx->received_base64_data_bytes_amount);
                pkt->data = malloc(real_data_buffer_size);
                if (pkt->data == NULL) {
                    return ENOMEM;
                }
                int ret;
                if ((ret = base64_decode((unsigned char *) sptctx->received_base64_data_bytes,
                                         sptctx->received_base64_data_bytes_amount,
                                         (unsigned char *) pkt->data,
                                         &real_data_buffer_size)) != BASE64_SUCCESS) {
                    free(pkt->data);
                    free(pkt);
                    free(sptctx->received_base64_data_bytes);
                    return (int8_t) ret;
                }
                pkt->len = (uint16_t) real_data_buffer_size;
                sptctx->callback(sptctx, pkt);
                free(pkt->data);
                free(pkt);
                free(sptctx->received_base64_data_bytes);
                sptctx->processing_packet = false;
                size_t offset = read_index + 1;
                return process_data(sptctx, workbuf + offset, len - offset);
            }
        }
        return 0;
    } else {
        for (size_t read_bytes = 0; read_bytes < len; read_bytes++) {
            sptctx->recv_base64_len_bytes[sptctx->recv_base64_len_bytes_amount] = workbuf[read_bytes];
            sptctx->recv_base64_len_bytes_amount++;
            if (sptctx->recv_base64_len_bytes_amount == base64_required_size_in_base64(sizeof(base64_len_t))) {
                // base64 padding: 2 bytes -> 4 chars -> 3 bytes
                size_t len_base64_len = sizeof(base64_len_t) + 1;
                int ret;
                if ((ret = base64_decode((unsigned char *) sptctx->recv_base64_len_bytes,
                                         sptctx->recv_base64_len_bytes_amount,
                                         (unsigned char *) &sptctx->expected_base64_data_bytes_amount,
                                         &len_base64_len)) !=
                    BASE64_SUCCESS) {
                    return (int8_t) ret;
                }
                sptctx->expected_base64_data_bytes_amount = ntohs(sptctx->expected_base64_data_bytes_amount);
                free(sptctx->recv_base64_len_bytes);
                sptctx->received_base64_data_bytes = malloc(sptctx->expected_base64_data_bytes_amount);
                if (sptctx->received_base64_data_bytes == NULL) {
                    return ENOMEM;
                }
                size_t offset = read_bytes + 1;
                return process_packet(sptctx, workbuf + offset, len - offset);
            }
        }
        return 0;
    }
}

int8_t process_message(struct spt_context *sptctx, char *workbuf, size_t len) {
    if (len == 0) {
        return 0;
    }
    ssize_t index_lf = index_of_char(workbuf, len, '\n');
    if (index_lf == -1) {
        char buf[len + 1];
        memcpy(buf, workbuf, len);
        buf[len] = '\0';
        if (sptctx->log_in_line) {
            printf("%s", buf);
            fflush(stdout);
        } else {
            spt_log_msg("data", "%s", buf);
            sptctx->log_in_line = true;
        }
        return 0;
    } else {
        workbuf[index_lf] = '\0';
        if (sptctx->log_in_line) {
            printf("%s\n", workbuf);
            fflush(stdout);
        } else {
            spt_log_msg("data", "%s\n", workbuf);
        }
        sptctx->log_in_line = false;
        size_t offset = (size_t) index_lf + 1;
        return process_message(sptctx, workbuf + offset, len - offset);
    }
}

int8_t process_data(struct spt_context *sptctx, char *workbuf, size_t len) {
    if (len == 0) {
        return 0;
    }
    if (sptctx->processing_packet) {
        return process_packet(sptctx, workbuf, len);
    }
    ssize_t index_0 = index_of_char(workbuf, len, SPT_PACKET_CHAR);
    if (index_0 != -1) {
        int8_t ret;
        if ((ret = process_message(sptctx, workbuf, (size_t) index_0)) != 0) {
            return ret;
        }
        size_t offset = (size_t) index_0 + 1;
        return process_packet(sptctx, workbuf + offset, len - offset);
    } else {
        return process_message(sptctx, workbuf, len);
    }
}

void data_received(const size_t len, const char *buf, void *arg) {
    struct spt_context *sptctx = arg;
    char workbuf[len];
    if (workbuf != NULL) {
        memcpy(workbuf, buf, len);
        int8_t ret;
        if ((ret = process_data(sptctx, workbuf, len)) != 0) {
            printf("Error while processing SPT data from input stream! Error code: %d\n", ret);
            exit(EXIT_FAILURE);
        }
    }
}

void spt_init_context(struct spt_context *sptctx, struct serial_io_context *sictx, received_packet_cb callback) {
    sptctx->callback = callback;
    sptctx->sictx = sictx;

    sptctx->log_in_line = false;
    sptctx->processing_packet = false;
    sptctx->recv_base64_len_bytes_amount = 0;
    sptctx->recv_base64_len_bytes = NULL;
    sptctx->expected_base64_data_bytes_amount = 0;
    sptctx->received_base64_data_bytes_amount = 0;
    sptctx->received_base64_data_bytes = NULL;
}

void spt_start(struct spt_context *sptctx) {
    sptctx->sictx->data_received_arg = sptctx;
    sptctx->sictx->data_received = data_received;
    serial_io_dispatch_start(sptctx->sictx);
}

int8_t spt_send_packet(const struct spt_context *sptctx, const struct serial_data_packet *packet) {
    int ret;
    unsigned char lenbytes[sizeof(base64_len_t)];
    size_t base64_size_len = base64_required_size_in_base64(sizeof(base64_len_t));
    size_t base64_data_len = base64_required_size_in_base64(packet->len);
    unsigned char base64_size[base64_size_len];
    unsigned char base64_data[base64_data_len];
    uint16_t packetsize;

    if ((ret = base64_encode((unsigned char *) packet->data, packet->len, base64_data, &base64_data_len)) !=
        BASE64_SUCCESS) {
        return ret;
    }
    base64_len_t truncatedlen = (base64_len_t) base64_data_len;
    truncatedlen = htons(truncatedlen);
    memcpy(lenbytes, &truncatedlen, sizeof(base64_len_t));
    if ((ret = base64_encode(lenbytes, sizeof(packet->len), base64_size, &base64_size_len)) != BASE64_SUCCESS) {
        return ret;
    }
    packetsize = sizeof(char) + base64_size_len + base64_data_len;
    char *pkt_ptr = malloc(packetsize);
    if (pkt_ptr == NULL) {
        return ENOMEM;
    }
    *pkt_ptr = SPT_PACKET_CHAR;
    char *len_ptr = pkt_ptr + sizeof(char);
    memcpy(len_ptr, base64_size, base64_size_len);
    char *data_ptr = len_ptr + base64_size_len;
    memcpy(data_ptr, base64_data, base64_data_len);
    ret = serial_io_write(sptctx->sictx, packetsize, pkt_ptr);
    free(pkt_ptr);
    return ret;
}

void spt_stop(struct spt_context *sptctx) {
    serial_io_dispatch_stop(sptctx->sictx);
}