#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

typedef struct Comparison {
    int32 greater;
    int32 lesser;
} Comparison;

constexpr int32 ALLOYS = 9;
Comparison parseComparison(char **line) {
    Comparison result;
    for (int byte = 0; byte < 4; byte++) {
        assert((*line)[byte] != '\0');
    }
    char a = (*line)[0], 
         operator = (*line)[1],
         b = (*line)[2];
    if (operator == '<') {
        result.greater = b - '0' - 1;
        result.lesser = a - '0';
    } else if (operator == '>') {
        result.greater = a - '0' - 1;
        result.lesser = b - '0' - 1;
    } else {
        assert(false);
        __builtin_unreachable();
    }
    assert(0 <= result.greater && result.greater < ALLOYS
        && 0 <= result.lesser && result.lesser < ALLOYS);

    (*line) += 4;
    return result;
}

void toposort(const int32 u, const uint32 adjacent[ALLOYS], int32 checked[ALLOYS], int32 digits[ALLOYS], int32 *current) {
    checked[u] = 1;
    uint32 uAdjacent = adjacent[u];
    while (uAdjacent) {
        const int32 v = __builtin_ctz(uAdjacent);
        uAdjacent &= uAdjacent - 1;
        if (!checked[v]) {
            toposort(v, adjacent, checked, digits, current);
        }
    }
    digits[(*current)--] = u + 1;
}

int main() {
    char *input = nullptr;
    size_t inputSize = 0;   
    ssize_t bytesRead;

    int64 checksum = 0;
    while ((bytesRead = getline(&input, &inputSize, stdin)) != -1) {
        char *cursor = input;
        if (bytesRead % 4 != 0) {
            puts("Input format invalid");
            return 1;
        }

        uint32 adjacent[ALLOYS] = {0};
        int32 checked[ALLOYS] = {0};
        while (*cursor) {
            Comparison cmp = parseComparison(&cursor);
            adjacent[cmp.greater] |= 1u << cmp.lesser;
        }

        int32 digits[ALLOYS] = {0};
        int32 current = ALLOYS - 1;
        for (int32 u = 0; u < ALLOYS; u++) {
            if (!checked[u]) {
                toposort(u, adjacent, checked, digits, &current);
            }
        }

        int64 number = 0;
        for (int32 index = 0; index < ALLOYS; index++) {
            number = number * 10ll + digits[index];
        }
        checksum += number;
    }

    printf("Checksum: %ld\n", checksum);


    return 0;
}
