CC ?= gcc
CFLAGS ?= -g -Werror -Wall -Wextra -Wfloat-equal -Wundef -Wshadow -Wcast-align -Wstrict-prototypes -Wstrict-overflow=5 \
	-Wno-write-strings -Waggregate-return -Wcast-qual -Wswitch-default -Wswitch-enum -Wunreachable-code
LD ?= gcc
RM ?= rm
ECHO ?= echo
SHELL ?= /bin/sh