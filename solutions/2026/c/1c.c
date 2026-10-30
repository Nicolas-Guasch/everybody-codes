#include <stdio.h>
#include "utils.h"

int32 parseNextNum(char **line) {
    assert(*line != nullptr);
    int32 result = 0;
    while (('0' < **line) && (**line < '9')) {
        result = result * 10 + (**line - '0');
        (*line)++;
    }
    (*line)++;
    return result;
}

static inline int32 calcFrame(int32 index) {
    return index * index - 1;
}

int main() {
    char *input = nullptr;
    size_t inputSize;
    ssize_t bytesRead = getline(&input, &inputSize, stdin);

    if (bytesRead == -1 || bytesRead == 0) {
        puts("Error reading input");
        return 1;
    }

    int32 squareIndex = 3;
    int32 squareFrame = calcFrame(squareIndex);
    int32 neededPineCones = 0;
    while (*input) {
        int32 outpostPineCones = parseNextNum(&input);
        while (squareFrame < outpostPineCones) {
            squareIndex += 2;
            squareFrame = calcFrame(squareIndex);
        }
        neededPineCones += squareFrame - outpostPineCones;
    }
    printf("%d pine cones must be collected.\n", neededPineCones);
    return 0;
}
