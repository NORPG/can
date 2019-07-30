// reference https://blog.csdn.net/jirryzhang/article/details/77969825

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/unistd.h>
#include <sys/fcntl.h>

int receive_can(int socket, struct can_frame *cf);

int main(int argc, char *argv[])
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
    strcpy(ifr.ifr_name, "can0");
    //get device index (ifr_ifindex) by device name (ifr_name)
    ioctl(s, SIOCGIFINDEX, &ifr);

    //bind device to socket 
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(s, (struct sockaddr *) &addr, sizeof(addr)) == -1) {
	printf("error: fail to bind device\n\r");
	return -1;
    }
    //can bus frame for receive and send
    while (1) {
	struct can_frame r_frame, s_frame;
	size_t nbytes;
	size_t index = 0;

	if (receive_can(s, &r_frame) == 0) {
	    printf("revice a can frame, can id: %x,", r_frame.can_id);
	    while (index < r_frame.can_dlc)
		printf(" %x", r_frame.data[index]);
	    printf("\n\r");
	}
	//can frame id
	s_frame.can_id = 0x127;
	//can frame data length
	s_frame.can_dlc = 4;
	//can frame data
	s_frame.data[0] = 0xDE;
	s_frame.data[1] = 0xAD;
	s_frame.data[2] = 0xBE;
	s_frame.data[3] = 0xEF;

	//send can frame
	nbytes = write(s, &s_frame, sizeof(s_frame));

	if (nbytes != sizeof(s_frame))
	    printf("error: can write fail\n\r");
    }
}

int receive_can(int socket, struct can_frame *cf)
{
    size_t nbytes;

    if (socket == 0) {
	printf("socket file desc error.\n\r");
	return -2;
    }

    nbytes = read(socket, cf, sizeof(*cf));

    if (nbytes < 0) {
	printf("no can frame.\n\r");
	return -1;
    }

    if (nbytes < sizeof(*cf)) {
	printf("can frame read fail.\n\r");
	return -1;
    }

    return 0;
}
