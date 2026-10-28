#include <stdio.h>
#include "utils.h"

int32 parseNextNum(char **line) {
    int32 result = 0;
    while ((**line != ',') && (**line != '\n')) {
        assert(*line != nullptr);
        result = result * 10 + (**line - '0');
        (*line)++;
    }
    (*line)++;
    return result;
}

inline int32 calcFrame(int32 index) {
    return index * index - (index - 2) * (index - 2);
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
            squareFrame = calcFrame(++squareIndex);
        }
        neededPineCones += squareFrame - outpostPineCones;
    }
    printf("%d pine cones must be collected.\n", neededPineCones);
    return 0;
}
