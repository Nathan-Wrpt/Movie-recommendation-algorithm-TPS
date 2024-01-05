#include <stdio.h>

/**
 * @file progressbar.c
 * @brief Function for displaying a progress bar.
 */

void updateProgressBar(int percentage) {
    int i;
    int numBars = percentage / 5; // 100% divided by 5 gives 20 bars
    printf("[");
    for (i = 0; i < numBars; ++i) {
        printf("=");
    }
    for (i = numBars; i < 20; ++i) {
        printf(" ");
    }
    printf("] %d%%", percentage);
    printf("\r"); // Move the cursor to the beginning of the line

    fflush(stdout); // Flush the output buffer
}