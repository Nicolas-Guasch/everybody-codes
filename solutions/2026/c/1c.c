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

inline int calcFrame(int index) {
    return index * index - 1;
}

int main() {
    char *input = NULL;
    size_t inputSize;
    ssize_t bytesRead = getline(&input, &inputSize, stdin);

    if (bytesRead == -1 || !inputSize) {
        puts("Error reading input");
        return 1;
    }

    int squareIndex = 3;
    int squareFrame = calcFrame(squareIndex);
    int neededPineCones = 0;
    while (*input) {
        int outpostPineCones = parseNextNum(&input);
        while (squareFrame < outpostPineCones) {
            squareIndex += 2;
            squareFrame = calcFrame(squareIndex);
        }
        neededPineCones += squareFrame - outpostPineCones;
    }
    printf("%d pine cones must be collected.\n", neededPineCones);
}
