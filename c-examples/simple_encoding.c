// Encode the same tiny blank frame 30 times
//
// Usage:
// $ cargo build --release
// $ cc -Wall -I ../include -L../target/release -lrav1e simple_encoding.c -o simple_encoding

#include <rav1e.h>
#include <stdio.h>
#include <inttypes.h>

int main(int argc, char **argv)
{
    RaConfig *rac = rav1e_config_default();
    RaFrame *f = NULL;
    RaContext *rax = NULL;
    int ret = -1;

    if (!rac) {
        printf("Unable to initialize\n");
        goto clean;
    }

    ret = rav1e_config_parse_int(rac, "width", 64);
    if (ret < 0) {
        printf("Unable to configure width\n");
        goto clean;
    }

    ret = rav1e_config_parse_int(rac, "height", 96);
    if (ret < 0) {
        printf("Unable to configure height\n");
        goto clean;
    }

    ret = rav1e_config_parse_int(rac, "speed", 9);
    if (ret < 0) {
        printf("Unable to configure speed\n");
        goto clean;
    }

    rax = rav1e_context_new(rac);
    if (!rax) {
        printf("Unable to allocate a new context\n");
        goto clean;
    }

    f = rav1e_frame_new(rax);
    if (!f) {
        printf("Unable to allocate a new frame\n");
        goto clean;
    }

    for (int i = 0; i < 30; i++) {
        printf("Sending frame\n");
        ret = rav1e_send_frame(rax, f);
        if (ret < 0) {
            printf("Unable to send frame %d\n", i);
            goto clean;
        } else if (ret > 0) {
            printf("Unable to append frame %d to the internal queue\n", i);
        }
    }

    for (int i = 0; i < 30; i++) {
        RaPacket *p;
        printf("Encoding frame\n");
        ret = rav1e_receive_packet(rax, &p);
        if (ret < 0) {
            printf("Unable to receive packet %d\n", i);
            goto clean;
        } else if (ret == 0) {
            printf("Packet %"PRIu64"\n", p->number);
            rav1e_packet_unref(p);
        }
    }

    ret = 0;

clean:
    rav1e_frame_unref(f);
    rav1e_context_unref(rax);
    rav1e_config_unref(rac);

    return -ret;
}
