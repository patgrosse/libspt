#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>

#include <serial_io.h>
#include <spt.h>
#include <unused.h>
#include <spt_logger.h>
#include <tty_utils.h>

void handle_received_packet(struct spt_context *sptctx, struct serial_data_packet *packet) {
    if (sptctx->log_in_line) {
        putchar('\n');
    }
    log_msg("packet", "Received packet with length %d\n", packet->len);
    char packetdatabuf[packet->len + 1];
    memcpy(packetdatabuf, packet->data, packet->len);
    packetdatabuf[packet->len] = '\0';
    log_msg("packet", "Data of packet is %s\n", packetdatabuf);
    if (sptctx->log_in_line) {
        log_msg("data", "");
    }
}

int main(int argc, char *argv[]) {
    UNUSED(argc);
    UNUSED(argv);
    int serialfd = connect_serial("/dev/ttyUSB0");
    if (serialfd < 0) {
        return EXIT_FAILURE;
    }
    if (init_serial_connection(serialfd) != 0) {
        return -1;
    }
    struct serial_io_context sictx;
    serial_io_context_init(&sictx, serialfd, serialfd);
    struct spt_context sptctx;
    spt_init_context(&sptctx, &sictx, handle_received_packet);
    log_msg("main", "Starting SPT...\n");
    spt_start(&sptctx);
    sleep(1);
    log_msg("main", "Sending packet...\n");
    struct serial_data_packet pkt;
    pkt.data = "heydevice";
    pkt.len = 9;
    spt_send_packet(&sptctx, &pkt);
    write(serialfd, "general message\n", 16);
    sleep(2);
    spt_stop(&sptctx);
    exit(0);
    return EXIT_SUCCESS;
}