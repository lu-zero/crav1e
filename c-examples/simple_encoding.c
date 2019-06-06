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
/*
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
*/
    ret = rav1e_config_parse_int(rac, "speed", 9);
    if (ret < 0) {
        printf("Unable to configure speed\n");
        goto clean;
    }
/*
    ret = rav1e_config_set_color_description(rac, 2, 2, 2);
    if (ret < 0) {
        printf("Unable to configure color properties\n");
        goto clean;
    }

    RaPoint primaries[] = {
        { .x = 0.68 * (1 << 16),  .y = 0.32 * (1 << 16) },
        { .x = 0.265 * (1 << 16), .y = 0.69 * (1 << 16) },
        { .x = 0.15 * (1 << 16),  .y = 0.06 * (1 << 16) },
    };
    RaPoint wp = { .x = 0.31268 * (1 << 16), .y = 0.329 * (1 << 16) };
    ret = rav1e_config_set_mastering_display(rac, primaries, wp, 1000 * (1 << 8), 0 * (1 << 14));
    if (ret < 0) {
        printf("Unable to configure mastering display\n");
        goto clean;
    }

    ret = rav1e_config_set_content_light(rac, 1000, 0);
    if (ret < 0) {
        printf("Unable to configure mastering display\n");
        goto clean;
    }
*/
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

    int limit = 30;

    printf("Encoding %d frames\n", limit);

    /// Feed `limit` frames directly
    for (int i = 0; i < limit; i++) {
        printf("Sending frame\n");
        ret = rav1e_send_frame(rax, f);
        if (ret < 0) {
            printf("Unable to send frame %d\n", i);
            goto clean;
        } else if (ret > 0) {
            printf("Unable to append frame %d to the internal queue\n", i);
        }
    }

    /// Signal the encoder we want to flush
    rav1e_send_frame(rax, NULL);

    ret = 0;

    /// Test that we cleanly exit once we hit the limit
    for (int i = 0; i < limit + 5;) {
        RaPacket *p;
        ret = rav1e_receive_packet(rax, &p);
        if (ret < 0) {
            printf("Unable to receive packet %d\n", i);
            goto clean;
        } else if (ret == 0) {
            printf("Packet %"PRIu64"\n", p->number);
            rav1e_packet_unref(p);
            i++;
        } else if (ret == RA_ENCODER_STATUS_LIMIT_REACHED) {
            printf("Limit reached\n");
            break;
        }
    }

    ret = 0;

clean:
    rav1e_frame_unref(f);
    rav1e_context_unref(rax);
    rav1e_config_unref(rac);

    return -ret;
}
