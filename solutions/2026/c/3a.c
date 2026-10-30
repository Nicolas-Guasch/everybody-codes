#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

int32 powInt32(int32 base, int32 exp) {
    assert(exp >= 0);
    int32 result = 1;
    while (exp--) result *= base;
    return result;
}

void parseDigits(char **input, int *digits) {
    while ('0' <= **input && **input <= '9') {
        *(digits++) = *((*input)++) - '0';
    }
    (*input)++;
}

int main() {
    char *input = nullptr;
    size_t capacity = 0;
    ssize_t bytesRead;

    constexpr int32 DIGITCOUNT = 4;
    [[maybe_unused]] constexpr int32 INPUTSIZE = 4 + (DIGITCOUNT + 1) * 2;
    int64 score = 0;
    while ((bytesRead = getline(&input, &capacity, stdin)) != -1) {
        assert(bytesRead == INPUTSIZE);
        char *cursor = input;
        int32 base[DIGITCOUNT] = {0};
        int32 voteDigits[DIGITCOUNT] = {0};

        cursor += 4;
        parseDigits(&cursor, base);
        parseDigits(&cursor, voteDigits);

        int32 vote = 0;
        for (int32 i = 0; i < DIGITCOUNT; i++) {
            vote += voteDigits[DIGITCOUNT - 1 - i] * 
                powInt32(base[DIGITCOUNT - 1 - i], i);
        }
        score += vote;
    }

    printf("The candidate's total score is: %ld\n", score);
}
