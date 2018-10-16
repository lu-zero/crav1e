// Encode the same tiny blank frame 30 times
//
// Usage:
// $ cargo build --release
// $ cc -I ../include -L../target/release -lrav1e simple_encoding.c -o simple_encoding

#include <rav1e.h>
#include <stdio.h>

int main() {
    RaConfig *rac = rav1e_config_default(64, 48, 8, Cs420, (RaRatio){ 1, 60 });
    rav1e_config_parse(rac, "speed", "9");

    RaContext *rax = rav1e_context_new(rac);
    RaFrame *f = rav1e_frame_new(rax);

    for (int i = 0; i < 30; i++) {
        printf("Sending frame\n");
        rav1e_send_frame(rax, f);
    }

    for (int i = 0; i < 30; i++) {
        RaPacket *p;
        printf("Encoding frame\n");
        rav1e_receive_packet(rax, &p);
        printf("Packet %lld\n", p->number);
        rav1e_packet_drop(p);
    }

    rav1e_frame_drop(f);
    rav1e_context_drop(rax);
    rav1e_config_drop(rac);
}
