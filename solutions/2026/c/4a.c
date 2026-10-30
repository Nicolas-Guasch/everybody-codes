#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

uint32 parseUint32(char **input) {
    uint32 result = 0;
    while ('0' <= (**input) && (**input) <= '9') {
        result = result * 10 + (uint32)(*((*input)++) - '0');
    }
    (*input)++;
    return result;
}

void mergeSort(uint32 *array, int32 left, int32 right) {
    if (right - left <= 1) return;
    int32 mid = (right - left) / 2 + left;
    mergeSort(array, left, mid);
    mergeSort(array, mid, right);

    uint32 *temp = (uint32 *)malloc(sizeof(uint32) * (uint64)(right-left));
    int32 i = left, j = mid, k = 0;
    while (i < mid && j < right) {
        if (array[i] < array[j]) temp[k++] = array[i++];
        else temp[k++] = array[j++];
    }
    while (i < mid) temp[k++] = array[i++];
    while (j < right) temp[k++] = array[j++];
    for (k = 0; k < (right - left); k++) {
        array[left + k] = temp[k];
    }
    free(temp);
    return;
}

int main() {
    char *input = nullptr;
    size_t capacity = 0;
    ssize_t bytesRead;

    constexpr uint32 CONTESTANTS = 10;
    uint32 contestantTime[CONTESTANTS] = {0};
    while ((bytesRead = getline(&input, &capacity, stdin)) != -1) {
        char *cursor = input;

        input += 3;
        uint32 id = parseUint32(&cursor);
        input += 8;
        input += 7;
        input += 5;
        uint32 time = parseUint32(&cursor);
        contestantTime[id - 1] += time;
    }
    
    uint64 checksum = 0;
    mergeSort(contestantTime, 0, CONTESTANTS);
    for (int64 i = 0; i <  CONTESTANTS; i++) {
        checksum += (uint64)(contestantTime[i]) * (uint64)(i + 1);
    }

    printf("The checksum is: %ld\n", checksum);
    return 0;
}
