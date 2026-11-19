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

constexpr uint32 MAX_CLOUDS = 110,
          EXPLOSION_IMPULSE = 10, MAX_STRENGTH = 10;
constexpr int32 CLOUD_CAPACITY = 1000000;
bool simulateStage(int32 cloudEnergy[MAX_CLOUDS], const uint128 cloudConnections[MAX_CLOUDS], bool exploded[MAX_CLOUDS], int32 impulses[MAX_CLOUDS][MAX_STRENGTH]) {
    int32 nextStage[MAX_CLOUDS][MAX_STRENGTH] = {0};
    for (uint32 u = 0; u < MAX_CLOUDS; u++) {
        if (!exploded[u]) {
            for (uint32 k = 0; k < MAX_STRENGTH; k++) {
                int32 energy = impulses[u][k] * ((int32)k + 1);
                cloudEnergy[u] += energy;
            }
            for (uint32 k = 1; k < MAX_STRENGTH; k++) {
                uint128 connections = cloudConnections[u];
                while (connections) {
                    uint32 v = (uint32)__builtin_ctzg(connections);
                    connections &= connections - 1;
                    if (!exploded[v]) {
                        nextStage[v][k - 1] += impulses[u][k];
                    }
                }
            }
            if (cloudEnergy[u] >= CLOUD_CAPACITY) {
                exploded[u] = true;
                cloudEnergy[u] = 0;
                uint128 connections = cloudConnections[u];
                while (connections) {
                    uint32 v = (uint32)__builtin_ctzg(connections);
                    connections &= connections - 1;
                    if (!exploded[v]) {
                        nextStage[v][EXPLOSION_IMPULSE - 1]++;
                    }
                }
            } else {
            }
        }
    }

    bool mustContinue = false;
    for (uint32 u = 0; u < MAX_CLOUDS; u++) {
        if (!exploded[u]) {
            for (uint32 k = 0; k < MAX_STRENGTH; k++) {
                impulses[u][k] = nextStage[u][k];
                mustContinue |= impulses[u][k] > 0;
            }
        }
    }
    return mustContinue;
}

int main() {
    char *input = nullptr;
    size_t capacity = 0;
    ssize_t bytesRead;

    int32 cloudBaseEnergy[MAX_CLOUDS] = {0};
    uint128 cloudConnections[MAX_CLOUDS] = {0};
    uint32 cloudCount = 0;
    while ((bytesRead = getline(&input, &capacity, stdin)) >= 0) {
        char *cursor = input;
        cursor += 3;
        uint32 id = parseUint32(&cursor) - 1;
        cursor += 9;
        cloudBaseEnergy[id] = (int32)parseUint32(&cursor);
        cursor += 14;
        while (*cursor) {
            cloudConnections[id] |= (uint128)(1) << (uint128)(parseUint32(&cursor) - 1);
            cursor++;
        }
        cloudCount++;
    }

    bool exploded[MAX_CLOUDS] = {0};
    int32 cloudEnergy[MAX_CLOUDS] = {0};
    int32 impulses[MAX_CLOUDS][MAX_STRENGTH] = {0};
    constexpr int32 initialEnergy = 10;
    int32 highestTotalEnergy = 0;
    for (uint32 cloud = 0; cloud < cloudCount; cloud++) {
        for (uint32 u = 0; u < cloudCount; u++) {
            exploded[u] = false;
            cloudEnergy[u] = cloudBaseEnergy[u];
            for (uint32 k = 0; k < MAX_STRENGTH; k++) {
                impulses[u][k] = 0;
            }
        }
        impulses[cloud][initialEnergy - 1] = 1;
        bool impulsesRemaining;
        do {
            impulsesRemaining = simulateStage(cloudEnergy, cloudConnections, exploded, impulses);
        } while (impulsesRemaining);
        int32 totalEnergy = 0;
        for (uint32 u = 0; u < cloudCount; u++) {
            totalEnergy += cloudEnergy[u];
        }
        printf("Cloud %d's total energy: %d\n", cloud + 1, totalEnergy);
        if (totalEnergy > highestTotalEnergy) {
            highestTotalEnergy = totalEnergy;
        }
        int32 explodeCount = 0;
        for (uint32 u = 0; u < cloudCount; u++) {
            if (exploded[u]) {
                explodeCount++;
            }
        }
        printf("Cloud %d's explosions: %d\n", cloud + 1, explodeCount);
    }

    printf("Highest total energy: %d\n", highestTotalEnergy);
    free(input);

    return 0;
}
