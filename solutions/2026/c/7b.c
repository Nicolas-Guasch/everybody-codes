#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

typedef __uint128_t uint128;

uint32 parseUint32(char **buf) {
    uint32 number = 0;
    while ('0' <= **buf && **buf <= '9') {
        number = number * 10 + (uint32)(*((*buf)++) - '0');
    }
    return number;
}

constexpr uint32 MAX_CLOUDS = 110;
void dfs(int32 cloudEnergy[MAX_CLOUDS], uint128 cloudConnections[MAX_CLOUDS], uint32 u, int32 energy) {
    cloudEnergy[u] += energy;
    energy--;
    if (energy != 0) {
        uint128 connections = cloudConnections[u];
        while (connections) {
            uint32 v = (uint32)__builtin_ctzg(connections);
            connections &= connections - 1;
            dfs(cloudEnergy, cloudConnections, v, energy);
        }
    }
}

int main() {
    char *input = nullptr;
    size_t capacity = 0;
    ssize_t bytesRead;

    int32 cloudEnergy[MAX_CLOUDS] = {0};
    uint128 cloudConnections[MAX_CLOUDS] = {0};
    uint32 mostOutgoingId = 0;
    while ((bytesRead = getline(&input, &capacity, stdin)) >= 0) {
        char *cursor = input;
        cursor += 3;
        uint32 id = parseUint32(&cursor) - 1;
        cursor += 9;
        cloudEnergy[id] = (int32)parseUint32(&cursor);
        cursor += 14;
        while (*cursor) {
            cloudConnections[id] |= (uint128)(1) << (uint128)(parseUint32(&cursor) - 1);
            cursor++;
        }
        if (__builtin_popcountg(cloudConnections[id])
                > __builtin_popcountg(cloudConnections[mostOutgoingId])) {
            mostOutgoingId = id;
        }
    }

    constexpr int32 initialEnergy = 10;
    dfs(cloudEnergy, cloudConnections, mostOutgoingId, initialEnergy);

    int32 totalEnergy = 0;
    for (uint32 i = 0; i < MAX_CLOUDS; i++) {
        totalEnergy += cloudEnergy[i];
    }
    printf("Total energy: %d\n", totalEnergy);
    free(input);

    return 0;
}
