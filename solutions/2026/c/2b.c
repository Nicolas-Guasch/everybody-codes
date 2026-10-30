#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

constexpr int32 ALLOYS = 9;
void parseComparison(char **line, int *u, int *v) {
    for (int byte = 0; byte < 4; byte++) {
        assert((*line)[byte] != '\0');
    }
    char a = (*line)[0], 
         operator = (*line)[1],
         b = (*line)[2];
    if (operator == '<') {
        *u = b - '0';
        *v = a - '0';
    } else if (operator == '>') {
        *u = a - '0';
        *v = b - '0';
    } else {
        assert(false);
    }
    (*u)--;
    (*v)--;
    assert(0 <= *u && *u < ALLOYS
        && 0 <= *v && *v < ALLOYS);

    (*line) += 4;
    return;
}

void toposort(int32 u, int32 adjacent[ALLOYS][ALLOYS], int32 checked[ALLOYS], int32 digits[ALLOYS], int32 *current) {
    checked[u] = 1;
    for (int32 v = 0; v < ALLOYS; v++) {
        if (adjacent[u][v] && !checked[v]) {
            toposort(v, adjacent, checked, digits, current);
        }
    }
    digits[(*current)--] = u + 1;
}

int main() {
    char *input = nullptr;
    size_t capacity = 0;   
    ssize_t bytesRead;

    const int32 comparisons = (ALLOYS * (ALLOYS - 1)) / 2 - 1;

    int64 checksum = 0;
    while ((bytesRead = getline(&input, &capacity, stdin)) != -1) {
        char *cursor = input;
        if (bytesRead != 4 * comparisons) {
            puts("Input format invalid");
            return 1;
        }

        int32 adjacent[ALLOYS][ALLOYS] = {{0}};
        int32 checked[ALLOYS] = {0};
        for (int i = 0; i < comparisons; i++) {
            int u, v;
            parseComparison(&cursor, &u, &v);
            adjacent[u][v] = 1;
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
        printf("number: %ld\n", number);
        checksum += number;
    }

    printf("Checksum: %ld\n", checksum);


    return 0;
}
