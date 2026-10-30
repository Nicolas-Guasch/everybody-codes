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

typedef struct Contestant {
    uint32 parts;
    uint32 time;
} Contestant;

bool contestantComp(Contestant a, Contestant b) {
    return (a.parts == b.parts) 
        ? a.time < b.time
        : a.parts > b.parts;
}

void mergeSort(Contestant *array, int32 left, int32 right) {
    if (right - left <= 1) return;
    int32 mid = (right - left) / 2 + left;
    mergeSort(array, left, mid);
    mergeSort(array, mid, right);

    Contestant *temp = (Contestant *)malloc(sizeof(Contestant) * (uint64)(right-left));
    int32 i = left, j = mid, k = 0;
    while (i < mid && j < right) {
        if (contestantComp(array[i], array[j])) temp[k++] = array[i++];
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

    constexpr uint32 CONTESTANTS = 100;
    Contestant tournament[CONTESTANTS] = {0};
    while ((bytesRead = getline(&input, &capacity, stdin)) != -1) {
        char *cursor = input;

        cursor += 3;
        uint32 id = parseUint32(&cursor);
        cursor += 6;
        [[maybe_unused]]uint32 quest = parseUint32(&cursor);
        cursor += 5;
        [[maybe_unused]]uint32 part = parseUint32(&cursor);
        cursor += 5;
        uint32 time = parseUint32(&cursor);
        tournament[id - 1].parts++;
        tournament[id - 1].time += time;
    }
    
    uint64 checksum = 0;
    mergeSort(tournament, 0, CONTESTANTS);
    for (int64 i = 0; i <  CONTESTANTS; i++) {
        checksum += (uint64)(tournament[i].time) * (uint64)(i + 1);
    }

    printf("The checksum is: %ld\n", checksum);
    return 0;
}
