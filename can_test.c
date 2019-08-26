#include <stdio.h>
#include <stdlib.h>
#include "can.h"

int main(int argc, char *argv[])
{
    int s;

    s = create_socket("can0");
    if (s < 0) {
	printf("error: socket create fail");
	return -1;
    }

    //can bus frame for receive and send
    while (1) {
	struct can_frame *r_frame, *s_frame;

	r_frame = calloc(1, sizeof(*r_frame));
	s_frame = calloc(1, sizeof(*s_frame));

	if (recv_can(s, r_frame) == 0) {
	    dump_can(r_frame);
	}

	free(r_frame);
	//can frame id
	s_frame->can_id = 0x127;
	//can frame data length
	s_frame->can_dlc = 4;
	//can frame data
	s_frame->data[0] = 0xDE;
	s_frame->data[1] = 0xAD;
	s_frame->data[2] = 0xBE;
	s_frame->data[3] = 0xEF;

	if (send_can(s, s_frame) == 0)
	    dump_can(s_frame);

	free(s_frame);
	sleep(1);
    }
}
