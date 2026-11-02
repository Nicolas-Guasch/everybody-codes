#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

typedef struct Position {
    int32 i;
    int32 j;
} Position;

typedef struct Queue {
    Position *queue;
    uint64 capacity;
    uint64 head;
    uint64 tail;
    bool empty;
} Queue;

constexpr uint32 defaultQueue = 100;
internal Queue queueCreate() {
    Queue q = {0};
    q.queue = (Position *)malloc(sizeof(Position) * defaultQueue);
    q.capacity = defaultQueue;
    q.empty = true;
    return q;
}

internal void queuePush(Queue *q, const int32 i, const int32 j) {
    const Position pos = {i, j};
    const uint64 size = (q->tail >= q->head) ? q->tail - q->head : (q->capacity + q->tail) - q->head;
    if (size + 1 == q->capacity) {
        q->capacity *= 2;
        Position *expandQueue = (Position *)malloc(sizeof(Position) * q->capacity);
        uint64 queuePos = q->head;
        for (uint64 ind = 0; ind < size - 1; ind++) {
            expandQueue[ind] = q->queue[queuePos];
            queuePos = (queuePos + 1 == size) ? 0 : queuePos + 1;
        }
        free(q->queue);
        q->queue = expandQueue;
        q->head = 0;
        q->tail = size - 1;
    }
    q->queue[q->tail] = pos;
    q->tail = (q->tail + 1 == q->capacity) ? 0 : q->tail + 1;
    q->empty = false;
}

internal Position queuePop(Queue *q) {
    assert(q->head != q->tail);
    Position result = q->queue[q->head];
    q->head = (q->head + 1 == q->capacity) ? 0 : q->head + 1;
    q->empty = q->head == q->tail;
    return result;
}

constexpr int32 WIDTH = 299, HEIGHT = 163;

internal inline bool inbounds(const int32 i, const int32 j) {
    return 0 <= i && i < HEIGHT && 0 <= j && j < WIDTH;
}

constexpr int32 di[] = {-1, -1, -1, +0, +1, +1, +1, +0};
constexpr int32 dj[] = {-1, +0, +1, +1, +1, +0, -1, -1};
constexpr int32 touching = 8;
internal void addNeighbors(uint8 neighbors[HEIGHT][WIDTH], const int32 i, const int32 j) {
    for (int32 k = 0; k < touching; k++) {
        int32 neighI = i + di[k],
        neighJ = j + dj[k];
        if (inbounds(neighI, neighJ)) {
            neighbors[neighI][neighJ]++;
        }
    }
}

int main() {
    freopen("5c.in", "r", stdin);
    char *input = nullptr;
    size_t capacity = 0;
    ssize_t bytesRead;

    // TODO: Compress dark, neighbors, land, sand, tree into a Cell struct.
    uint8 dark[HEIGHT][WIDTH] = {0};
    uint8 neighbors[HEIGHT][WIDTH] = {0};
    bool land[HEIGHT][WIDTH] = {0};

    {
        int32 i = 0;
        while ((bytesRead = getline(&input, &capacity, stdin)) >= 0) {
            assert(bytesRead == WIDTH + 1);
            char *cursor = input;
            int32 j = 0;
            while (*cursor == '0' || *cursor == '1') {
                if (*(cursor++) == '1') {
                    dark[i][j] = 1;
                    addNeighbors(neighbors, i, j);
                }
                j++;
            }
            i++;
        }
    }

    int32 landCells = 1;
    const int32 startI = HEIGHT/2,
          startJ = WIDTH/2;
    land[startI][startJ] = true;
    Queue processQueue = queueCreate();
    queuePush(&processQueue, startI, startJ);

    while (!processQueue.empty) {
        const Position pos = queuePop(&processQueue);

        const int32 light = dark[pos.i][pos.j] == 0;
        for (int32 k = 0; k < touching; k++) {
            const Position adj = {pos.i + di[k], pos.j + dj[k]};
            if (inbounds(adj.i, adj.j)) {
                neighbors[adj.i][adj.j] += light;
                if (!land[adj.i][adj.j] && neighbors[adj.i][adj.j] >= 5) {
                    landCells++;
                    land[adj.i][adj.j] = true;
                    queuePush(&processQueue, adj.i, adj.j);
                }
            }
        }
    }

    printf("There are %d land cells.\n", landCells);

    bool sand[HEIGHT][WIDTH] = {0};
    int32 sandCells = 0;
    for (int32 i = 0; i < HEIGHT; i++) {
        for (int32 j = 0; j < WIDTH; j++) {
            neighbors[i][j] = 0;

            if (land[i][j]) {
                for (int32 k = 0; k < touching && !sand[i][j]; k++) {
                    const Position adj = {i + di[k], j + dj[k]};
                    if (!inbounds(adj.i, adj.j) || !land[adj.i][adj.j]) {
                        sand[i][j] = true;
                        sandCells++;
                        queuePush(&processQueue, i, j);
                    }
                }
            }
        }
    }

    while (!processQueue.empty) {
        const Position pos = queuePop(&processQueue);

        for (int32 k = 0; k < touching; k++) {
            const Position adj = {pos.i + di[k], pos.j + dj[k]};
            if (inbounds(adj.i, adj.j) &&
                    land[adj.i][adj.j] &&
                    !sand[adj.i][adj.j]) {
                neighbors[adj.i][adj.j]++;
                if (neighbors[adj.i][adj.j] >= 5) {
                    sand[adj.i][adj.j] = true;
                    sandCells++;
                    queuePush(&processQueue, adj.i, adj.j);
                }
            }
        }
    }
    printf("There are %d sand cells.\n", sandCells);

    bool tree[HEIGHT][WIDTH] = {0};
    int32 treeCells = 0;

    for (int32 i = 2; i < HEIGHT - 2; i++) {
        for (int32 j = 2; j < WIDTH -2; j++) {
            bool canPlant = true;
            for (int32 nI = i-2; nI <= i+2 && canPlant; nI++) {
                for (int32 nJ = j-2; nJ <= j+2 && canPlant; nJ++) {
                    if (((i-1) <= nI && nI <= (i+1)) || 
                        ((j-1) <= nJ && nJ <= (j+1))) {
                        canPlant &= land[nI][nJ] && !sand[nI][nJ];
                    }
                }
            }
            tree[i][j] = canPlant;
            treeCells += canPlant;
        }
    }

    for (int32 _passes = 0; _passes < 2; _passes++) {
        for (int32 i = 1; i < HEIGHT - 1; i++) {
            for (int32 j = 1; j < WIDTH - 1; j++) {
                if (land[i][j] && !sand[i][j] && !tree[i][j]) {
                    int32 treeCount = 0;
                    int32 sandCount = 0;
                    for (int32 k = 0; k < touching; k++) {
                        Position adj = {i + di[k], j + dj[k]};
                        treeCount += tree[adj.i][adj.j];
                        sandCount += sand[adj.i][adj.j];
                    }
                    if (treeCount >= 4 && sandCount == 0) {
                        queuePush(&processQueue, i, j);
                        treeCells++;
                    }
                }
            }
        }
        while (!processQueue.empty) {
            Position newTree = queuePop(&processQueue);
            tree[newTree.i][newTree.j] = true;
        }
    }

    printf("There are %d tree cells.\n", treeCells);


    return 0;
}
