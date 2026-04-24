#include <stdio.h>

int main(void) {
    int count = 0;
    int arr[1024];
    int val;

    while (count < 1024 && scanf("%d", &val) == 1) {
        arr[count++] = val;
    }

    if (count == 0) {
        return 0;
    }

    if (count == 1) {
        printf("%d\n", arr[0]);
        return 0;
    }

    int suit_start = 0;
    int suit_end = 0;
    long long suit_sum = arr[0];

    int curr_start = 0;
    long long curr_sum = arr[0];

    for (int i = 1; i < count; i++) {
        if (arr[i] <= arr[i - 1]) {
            curr_start = i;
            curr_sum = arr[i];
        } else {
            curr_sum += arr[i];
        }

        int curr_len = i - curr_start + 1;
        int suit_len = suit_end - suit_start + 1;

        if (curr_len > suit_len || (curr_len == suit_len && curr_sum > suit_sum)) {
            suit_start = curr_start;
            suit_end = i;
            suit_sum = curr_sum;
        }
    }

    for (int i = suit_start; i <= suit_end; i++) {
        if (i > suit_start) {
            printf(" ");
        }
        printf("%d", arr[i]);
    }
    printf("\n");

    return 0;
}