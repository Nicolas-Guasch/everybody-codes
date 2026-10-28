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

int main() {
    char *input = nullptr;
    size_t inputSize;
    ssize_t bytesRead = getline(&input, &inputSize, stdin);

    if (bytesRead == -1 || bytesRead == 0) {
        puts("Error reading input");
        return 1;
    }

    int32 square = 1;
    int32 squareIndex = 1;
    int32 neededPineCones = 0;
    while (*input) {
        int32 outpostPineCones = parseNextNum(&input);
        while (square < outpostPineCones) {
            square += 2 * (squareIndex++) + 1;
        }
        neededPineCones += square - outpostPineCones;
    }
    printf("%d pine cones must be collected.\n", neededPineCones);
    return 0;
}
