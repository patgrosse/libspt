/*
 *  libspt - Serial Packet Transfer Library
 *
 *  Copyright (C) 2017 Patrick Grosse <patrick.grosse@uni-muenster.de>
 */
/*
 * based on http://stackoverflow.com/a/6947758
 */

#ifndef NO_TERMIOS

#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include <tty_utils.h>

int8_t set_interface_attribs(int fd, speed_t speed) {
    struct termios tty;
    if (tcgetattr(fd, &tty) != 0) {
        perror("error on tcgetattr in set_interface_attribs");
        return -1;
    }

    if (cfsetospeed(&tty, speed) != 0) {
        perror("error on cfsetospeed in set_interface_attribs");
        return -1;
    }
    if (cfsetispeed(&tty, speed) != 0) {
        perror("error on cfsetospeed in set_interface_attribs");
        return -1;
    }

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
    // disable IGNBRK for mismatched speed examples; otherwise receive break
    // as \000 chars
    tty.c_iflag &= ~IGNBRK;         // disable break processing
    tty.c_lflag = 0;                // no signaling chars, no echo,
    // no canonical processing
    tty.c_oflag = 0;                // no remapping, no delays
#ifndef NO_LIBEVENT
    tty.c_cc[VMIN] = 0;            // read doesn't block
#else
    tty.c_cc[VMIN] = 1;            // read does block
#endif
    tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

    tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
    // enable reading
    tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        perror("error on tcsetattr in set_interface_attribs");
        return -1;
    }
    return 0;
}

int8_t init_serial_connection(const int fd) {
    if (set_interface_attribs(fd, B115200) != 0) {
        return -1;
    }
    int DTR_flag = TIOCM_DTR;
    int RTS_flag = TIOCM_RTS;
    if (ioctl(fd, TIOCMBIC, &DTR_flag) != 0) {
        perror("error setting dtr flag");
        return -1;
    }
    if (ioctl(fd, TIOCMBIC, &RTS_flag) != 0) {
        perror("error setting rts flag");
        return -1;
    }
    return 0;
}

int connect_serial(char *portname) {
    int fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        perror("error opening serial connection");
        return -1;
    }
    if (init_serial_connection(fd) != 0) {
        return -1;
    }
    return fd;
}

#endif
