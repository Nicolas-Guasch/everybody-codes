#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

int32 powInt32(int32 base, int32 exp) {
    assert(exp >= 0);
    int32 result = 1;
    while (exp--) result *= base;
    return result;
}

int32 parseDigits(char **input, int *digits) {
    int32 length = 0;
    while ('0' <= **input && **input <= '9') {
        *(digits++) = *((*input)++) - '0';
        length++;
    }
    (*input)++;
    return length;
}

int main() {
    char *input = nullptr;
    size_t inputSize = 0;
    ssize_t bytesRead;

    constexpr int32 DIGITCOUNT = 10;
    int64 score = 0;
    while ((bytesRead = getline(&input, &inputSize, stdin)) != -1) {
        char *baseInput = input;
        int32 base[DIGITCOUNT] = {0};
        int32 voteDigits[DIGITCOUNT] = {0};

        input += 4;
        int32 baseLength = parseDigits(&input, base);
        int32 voteLength = parseDigits(&input, voteDigits);

        int64 vote = 0;
        for (int32 i = 0; i < voteLength; i++) {
            vote += (int64)voteDigits[voteLength - 1 - i] * 
                (int64)powInt32(base[baseLength - 1 - (i%baseLength)], i);
        }
        score += vote;
        
        free(baseInput);
        input = nullptr;
        inputSize = 0;
    }

    printf("The candidate's total score is: %ld\n", score);
}
