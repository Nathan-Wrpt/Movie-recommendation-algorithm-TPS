#include <stdio.h>

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

    // Move the cursor to the beginning of the line
    printf("\r");
    fflush(stdout); // Flush the output buffer
}

int main() {
    int totalUsers = 11480189;
    int currentUser;

    // printf("\033[1;33m"); // 33 yellow 32 green 36 blue 37 white
    printf("\033[1;22m"); // yellow

    for (currentUser = 1; currentUser <= totalUsers; ++currentUser) {
        int percentage = (currentUser * 100) / totalUsers;

        if (percentage % 5 == 0) { // Update every 5%
            updateProgressBar(percentage);
            
        }
    }

    printf("\nLoading complete!\n");

    return 0;
}