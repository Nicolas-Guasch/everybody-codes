#include <stdio.h>

int parseNextNum(char **line) {
    int result = 0;
    while ((**line != ',') && (**line != '\n')) {
        if (*line == NULL) return result;
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

    if (bytesRead == -1 || !inputSize) {
        puts("Error reading input");
        return 1;
    }

    int square = 1;
    int squareIndex = 1;
    int neededPineCones = 0;
    while (*input) {
        int outpostPineCones = parseNextNum(&input);
        while (square < outpostPineCones) {
            square += 2 * (squareIndex++) + 1;
        }
        neededPineCones += square - outpostPineCones;
    }
    printf("%d pine cones must be collected.\n", neededPineCones);
}
