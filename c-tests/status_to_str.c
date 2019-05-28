#include <stdio.h>

#include "rav1e.h"

int main(void) {
    char *str;

    /*
    RA_ENCODER_STATUS_SUCCESS = 0,
    RA_ENCODER_STATUS_NEED_MORE_DATA,
    RA_ENCODER_STATUS_ENOUGH_DATA,
    RA_ENCODER_STATUS_LIMIT_REACHED,
    RA_ENCODER_STATUS_ENCODED,
    RA_ENCODER_STATUS_FAILURE = -1,
    */

    for (int i = RA_ENCODER_STATUS_FAILURE;
            i <= RA_ENCODER_STATUS_ENCODED;
            i++) {
        char  *s = rav1e_status_to_str(i);
        printf("%d -> %s\n", i, s);
        free(s);
    }

    if (rav1e_status_to_str(-42) != NULL) {
        printf("Failure in rejecting invalid values");
    }

    return 0;
}
