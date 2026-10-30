#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

uint32 parseUint32(char **input) {
    uint32 result = 0;
    while ('0' <= (**input) && (**input) <= '9') {
        result = result * 10 + (uint32)(*((*input)++) - '0');
    }
    (*input)++;
    return result;
}

constexpr uint32 QUESTS = 20;
constexpr uint32 PARTS = 3;
typedef struct Contestant {
    uint32 time[QUESTS][PARTS];
    uint32 points;
} Contestant;

typedef struct ContestantPartTime {
    uint32 id;
    uint32 time;
} ContestantPartTime;

bool contestantComp(ContestantPartTime a, ContestantPartTime b) {
    return a.time && ((b.time && a.time < b.time) || !b.time);
}

void mergeSort(ContestantPartTime *array, int32 left, int32 right) {
    if (right - left <= 1) return;
    int32 mid = (right - left) / 2 + left;
    mergeSort(array, left, mid);
    mergeSort(array, mid, right);

    ContestantPartTime *temp = (ContestantPartTime *)malloc(sizeof(ContestantPartTime) * (uint64)(right-left));
    int32 i = left, j = mid, k = 0;
    while (i < mid && j < right) {
        if (contestantComp(array[i], array[j])) temp[k++] = array[i++];
        else temp[k++] = array[j++];
    }
    while (i < mid) temp[k++] = array[i++];
    while (j < right) temp[k++] = array[j++];
    for (k = 0; k < (right - left); k++) {
        array[left + k] = temp[k];
    }
    free(temp);
    return;
}

bool scoreComp(Contestant *T, uint32 a, uint32 b) {
    return T[a].points > T[b].points;
}

void mergeSortScore(Contestant *T, uint32 *array, int32 left, int32 right) {
    if (right - left <= 1) return;
    int32 mid = (right - left) / 2 + left;
    mergeSortScore(T, array, left, mid);
    mergeSortScore(T, array, mid, right);

    uint32 *temp = (uint32 *)malloc(sizeof(uint32) * (uint64)(right-left));
    int32 i = left, j = mid, k = 0;
    while (i < mid && j < right) {
        if (scoreComp(T, array[i], array[j])) temp[k++] = array[i++];
        else temp[k++] = array[j++];
    }
    while (i < mid) temp[k++] = array[i++];
    while (j < right) temp[k++] = array[j++];
    for (k = 0; k < (right - left); k++) {
        array[left + k] = temp[k];
    }
    free(temp);
    return;
}

int main() {
    char *input = nullptr;
    size_t inputSize = 0;
    ssize_t bytesRead;

    constexpr uint32 CONTESTANTS = 256;
    Contestant tournament[CONTESTANTS] = {0};
    while ((bytesRead = getline(&input, &inputSize, stdin)) != -1) {
        char *baseInput = input;

        input += 3;
        uint32 id = parseUint32(&input);
        input += 6;
        uint32 quest = parseUint32(&input);
        input += 5;
        uint32 part = parseUint32(&input);
        input += 5;
        uint32 time = parseUint32(&input);
        assert(id <= CONTESTANTS && quest <= QUESTS && part <= PARTS);
        tournament[id - 1].time[quest - 1][part - 1] = time;

        free(baseInput);
        input = nullptr;
        inputSize = 0;
    }
    

    ContestantPartTime partTimes[CONTESTANTS];
    for (uint32 quest = 0; quest < QUESTS; quest++) {
        for (uint32 i = 0; i < CONTESTANTS; i++) {
            partTimes[i] = (struct ContestantPartTime){i, 0};
        }
        for (uint32 part = 0; part < PARTS; part++) {
            for (uint32 i = 0; i < CONTESTANTS; i++) {
                if (tournament[partTimes[i].id].time[quest][part]) {
                    partTimes[i].time += tournament[partTimes[i].id].time[quest][part];
                } else {
                    partTimes[i].time = 0;
                }
            }
            mergeSort(partTimes, 0, CONTESTANTS);
            int32 points = 50 * (int32)(part + 1);
            uint32 last = partTimes[0].time;
            int32 tied = 0;
            printf("Quest %d, Part %d:\n", quest+1, part+1);
            for (uint32 i = 0; i < CONTESTANTS && points > 0; i++) {
                if (partTimes[i].time) {
                    if (partTimes[i].time != last) {
                        points -= tied;
                        tied = 1;
                        last = partTimes[i].time;
                    } else {
                        tied++;
                    }
                    if (points > 0) {
                        printf("id=%d, time=%d, points=%d\n", partTimes[i].id+1, partTimes[i].time, points);
                        tournament[partTimes[i].id].points += (uint32)points;
                    }
                }
                else {
                    points = 0;
                }
            }
            puts("\n");
        }
    }

    uint32 ids[CONTESTANTS];
    for (uint32 i = 0; i < CONTESTANTS; i++) {
        ids[i] = i;
    }
    mergeSortScore(tournament, ids, 0, CONTESTANTS);

    uint32 place = 1;
    uint32 last = tournament[ids[0]].points;
    uint32 tied = 0;
    uint64 checksum = 0;
    puts("Final Ranking:\n");
    for (uint32 i = 0; i < CONTESTANTS; i++) {
        if (tournament[ids[i]].points != last) {
            place+= tied;
            tied = 1;
            last = tournament[ids[i]].points;
        } else {
            tied++;
        }
        printf("%d. points=%d\n", place, tournament[ids[i]].points);
        checksum += place * tournament[ids[i]].points;
    }
    puts("\n");

    printf("Checksum: %ld\n", checksum);
    return 0;
}
