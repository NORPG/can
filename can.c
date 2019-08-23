// reference https://blog.csdn.net/jirryzhang/article/details/77969825

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <linux/can.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/unistd.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include "can.h"

int recv_can(int socket, struct can_frame *cf)
{
    size_t nbytes;

    if (socket == 0) {
	printf("error: socket file desc error.\n\r");
	return -2;
    }

    nbytes = read(socket, cf, sizeof(*cf));

    if (nbytes != sizeof(*cf)) {
	printf("error: can frame read fail.\n\r");
	return -1;
    }

    printf("info: recv a can frame.\n\r");
    return 0;
}

int send_can(int socket, struct can_frame *cf)
{
    size_t nbytes;

    if (socket == 0) {
	printf("error: socket file desc error.\n\r");
	return -2;
    }

    nbytes = write(socket, cf, sizeof(*cf));

    if (nbytes != sizeof(*cf)) {
	printf("error: can frame write fail.\n\r");
	return -1;
    }

    printf("info: send a can frame.\n\r");
    return 0;
}

void dump_can(struct can_frame *cf)
{
    size_t index = 0;

    printf("info: can frame, can id: %x,", cf->can_id);
    while (index < cf->can_dlc) {
	printf(" %x", cf->data[index]);
	index++;
    }
    printf("\n\r");
}

int create_socket(const char *dev_name)
{

    // socket handler
    int s;
    // high-level socket setting for can bus
    struct sockaddr_can addr;
    // low-level device setting
    struct ifreq ifr;

    /* create a socket by function
     * int socket(int domain, int type, int protocol)
     * domain : PF_CAN (CAN bus)
     * type : SOCK_RAW (RAW Socket)
     * protocol : CAN_RAW (CAN bus RAW data)
     * return value : socket number(fd)
     */
    s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    fcntl(s, F_SETFL, O_NONBLOCK);

    //assign can0 as device name
    strcpy(ifr.ifr_name, dev_name);
    //get device index (ifr_ifindex) by device name (ifr_name)
    ioctl(s, SIOCGIFINDEX, &ifr);

    //bind device to socket 
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(s, (struct sockaddr *) &addr, sizeof(addr)) == -1) {
	printf("error: fail to bind device\n\r");
	return -1;
    }
    return s;
}
