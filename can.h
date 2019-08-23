#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/can.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/unistd.h>
#include <sys/fcntl.h>
#include <sys/socket.h>

int recv_can(int , struct can_frame *);
int send_can(int , struct can_frame *);
void dump_can(struct can_frame *);
int create_socket(const char *);
