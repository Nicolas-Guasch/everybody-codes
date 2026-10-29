#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

int32 powInt32(int32 base, int32 exp) {
    assert(exp >= 0);
    int32 result = 1;
    while (exp--) result *= base;
    return result;
}

constexpr int32 DIGITCOUNT = 10;
typedef struct Diginumber {
    int32 digits[DIGITCOUNT];
    int32 length;
} Diginumber;


#define swapInt32(a, b) a^=b;b^=a;a^=b;
void digitize(int64 value, Diginumber *result) {
    if (!value) {
        result->digits[0] = 0;
        result->length = 1;
        return;
    }

    result->length = 0;
    while (value) {
        result->digits[result->length++] = value % 10;
        value /= 10;
    }

    for (int i = 0; i < result->length - 1 - i; i++) {
        swapInt32(result->digits[i], result->digits[result->length - 1 - i]);
    }
}

int64 diginumberToDecimal(Diginumber *num) {
    int64 result = 0;
    for (int i = 0; i < num->length; i++) {
        result = result * 10 + num->digits[i];
    }
    return result;
}

void parseDigits(char **input, Diginumber *num) {
    num->length = 0;
    while ('0' <= **input && **input <= '9') {
        num->digits[num->length++] = *((*input)++) - '0';
    }
    (*input)++;
    return;
}

int64 calculateISO(Diginumber *num, Diginumber *base) {
    int64 result = 0;
    for (int32 i = 0; i < num->length; i++) {
        result += (int64)num->digits[num->length - 1 - i] * 
            (int64)powInt32(base->digits[base->length - 1 - (i % base->length)], i);
    }
    return result;
}

bool32 minimalRep(int64 rep, int64 value, Diginumber *base) {
    for (int i = 1; i < rep; i++) {
        Diginumber test;
        digitize(i, &test);
        if (calculateISO(&test, base) == value) {
            return false;
        }
    }
    
    return true;
}

int main() {
    char *input = nullptr;
    size_t inputSize = 0;
    ssize_t bytesRead;

    int64 score = 0;
    while ((bytesRead = getline(&input, &inputSize, stdin)) != -1) {
        char *baseInput = input;

        Diginumber digiVote, base;

        input += 5;
        parseDigits(&input, &base);
        parseDigits(&input, &digiVote);

        int64 vote = calculateISO(&digiVote, &base);
        if (minimalRep(diginumberToDecimal(&digiVote), vote, &base)) {
            score += vote;
        }
        
        free(baseInput);
        input = nullptr;
        inputSize = 0;
    }

    printf("The candidate's total score is: %ld\n", score);
}
