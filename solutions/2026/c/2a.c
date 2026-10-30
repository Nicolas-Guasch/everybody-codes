#include <stdio.h>
#include "utils.h"

#define ALLOYS 5
int32 parseComparison(char **line) {
    int result = 0;
    for (int byte = 0; byte < 4; byte++) {
        assert(line[byte] != nullptr);
    }
    char a = (*line)[0], 
         operator = (*line)[1],
         b = (*line)[2];
    if (operator == '<') {
        result = b - '0';
    } else if (operator == '>') {
        result = a - '0';
    } else {
        assert(false);
    }
    assert(0 < result && result <= ALLOYS);

    (*line) += 4;
    return result;
}

int main() {
    char *input = nullptr;
    size_t inputSize = 0;   
    ssize_t bytesRead;

    const int32 comparisons = (ALLOYS * (ALLOYS - 1)) / 2;

    int32 checksum = 0;
    while ((bytesRead = getline(&input, &inputSize, stdin)) != -1) {
        char *baseInput = input;
        if (bytesRead != 4 * comparisons) {
            puts("Input format invalid");
            return 1;
        }
        int32 order[ALLOYS] = {0};
        int32 digits[ALLOYS] = {0};
        int32 number = 0;

        for (int32 i = 0; i < comparisons; i++) {
            int32 greater = parseComparison(&input);
            order[greater - 1]++;
        }
        for (int32 digit = 1; digit <= ALLOYS; digit++) {
            digits[ALLOYS - 1 - order[digit - 1]] = digit;
        }
        for (int32 index = 0; index < ALLOYS; index++) {
            number = number * 10 + digits[index];
        }
        printf("number: %d\n", number);
        checksum += number;
        free(baseInput);
        input = nullptr;
        inputSize = 0;
    }

    printf("Checksum: %d\n", checksum);


    return 0;
}
