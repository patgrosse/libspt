/*
 *  libspt - Serial Packet Transfer Library
 *
 *  Copyright (C) 2017 Patrick Grosse <patrick.grosse@uni-muenster.de>
 */

#ifndef LIBSPT_SPT_H
#define LIBSPT_SPT_H

#include <inttypes.h>

#include <serial_io.h>

/**
 * A character determining the start of a packet
 */
#define SPT_PACKET_CHAR 0x1C

/**
 * The data type of the base64 data character amount in a packet
 */
typedef uint16_t base64_len_t;

struct spt_context;

/**
 * A data packet with a fixed length
 */
struct serial_data_packet {
    uint16_t len;
    char *data;
};

/**
 * Function signature for the callback that is called when a packet is received and successfully parsed
 */
typedef void(*received_packet_cb)(struct spt_context *, struct serial_data_packet *);

/**
 * A struct to store all required data for spt transfers
 */
struct spt_context {
    /* user variables */
    struct serial_io_context *sictx;
    received_packet_cb callback;

    /* internal variables */
    bool log_in_line;
    bool processing_packet;
    size_t recv_base64_len_bytes_amount;
    char *recv_base64_len_bytes;
    base64_len_t expected_base64_data_bytes_amount;
    size_t received_base64_data_bytes_amount;
    char *received_base64_data_bytes;
};

/**
 * Initialize a spt context with a given serial_io_context and a callback for received packets
 * @param sptctx The context to initialize
 * @param sictx The serial_io_context to receive data from and to send data to
 * @param callback A function that should be called when a packet is received
 */
void spt_init_context(struct spt_context *sptctx, struct serial_io_context *sictx, received_packet_cb callback);

/**
 * Start the underlying serial io data dispatcher
 * @param sptctx The context to start the dispatcher for
 */
void spt_start(struct spt_context *sptctx);

/**
 * Send a packet
 * @param sptctx The context for this packet transfer
 * @param packet The packet to transfer
 * @return On success the positive amount of transferred bytes, a negative value on error
 */
int8_t spt_send_packet(const struct spt_context *sptctx, const struct serial_data_packet *packet);

/**
 * Stop the underlying serial io data dispatcher
 * @param sptctx The context to stop the dispatcher for
 */
void spt_stop(struct spt_context *sptctx);

#endif //LIBSPT_SPT_H
