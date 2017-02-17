/*
 *  libspt - Serial Packet Transfer Library
 *
 *  Copyright (C) 2017 Patrick Grosse <patrick.grosse@uni-muenster.de>
 */

/**
 * @brief   transfer of packets side by side with plain text over serial data connections
 * @file    spt.h
 * @author  Patrick Grosse <patrick.grosse@uni-muenster.de>
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
 * @brief A data packet with a fixed length
 */
struct serial_data_packet {
    /** @brief Length of the data in this packet */
    uint16_t len;
    /** @brief Data in this packet */
    char *data;
};

/**
 * Function signature for the callback that is called when a packet is received and successfully parsed
 */
typedef void(*received_packet_cb)(struct spt_context *, struct serial_data_packet *);

/**
 * @brief A struct to store all required data for spt transfers
 */
struct spt_context {
    /* user variables */
    /** @brief Serial IO context */
    struct serial_io_context *sictx;
    /** @brief Callback function when a packet is received */
    received_packet_cb callback;

    /* internal variables */
    /** @brief If the log is currently in an existing line (no LF) */
    bool log_in_line;
    /** @brief If we are currently processing a packet from the input fd */
    bool processing_packet;
    /** @brief Amount of bytes received for the packet length */
    size_t recv_base64_len_bytes_amount;
    /** @brief Received bytes of the packet length */
    char *recv_base64_len_bytes;
    /** @brief Expected length for the currently receiving packet */
    base64_len_t expected_base64_data_bytes_amount;
    /** @brief Amount of bytes received for the current packet */
    size_t received_base64_data_bytes_amount;
    /** @brief Bytes that have been received of the packet data */
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
