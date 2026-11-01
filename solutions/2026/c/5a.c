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
    
internal void queuePush(Queue *q, int32 i, int32 j) {
    Position pos = {i, j};
    uint64 size = (q->tail >= q->head) ? q->tail - q->head : (q->capacity + q->tail) - q->head;
    if (size == q->capacity) {
        q->capacity *= 2;
        Position *expandQueue = (Position *)malloc(sizeof(Position) * q->capacity);
        uint64 queuePos = q->head;
        for (uint64 ind = 0; ind < size; ind++) {
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

constexpr int32 WIDTH = 101, HEIGHT = 21;

internal inline bool inbounds(int i, int j) {
    return 0 <= i && i < HEIGHT && 0 <= j && j < WIDTH;
}

internal void addNeighbors(uint8 neighbors[HEIGHT][WIDTH], int i, int j) {
    const int32 di[] = {-1, -1, -1, +0, +1, +1, +1, +0};
    const int32 dj[] = {-1, +0, +1, +1, +1, +0, -1, -1};
    const int32 touching = 8;

    for (int k = 0; k < touching; k++) {
        int32 neighI = i + di[k],
              neighJ = j + dj[k];
        if (inbounds(neighI, neighJ)) {
            neighbors[neighI][neighJ]++;
        }
    }
}

int main() {
    freopen("5a.in", "r", stdin);
    char *input = nullptr;
    size_t capacity = 0;
    ssize_t bytesRead;

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
        const int32 di[] = {-1, -1, -1, +0, +1, +1, +1, +0};
        const int32 dj[] = {-1, +0, +1, +1, +1, +0, -1, -1};
        const int32 touching = 8;
        

        const int32 light = dark[pos.i][pos.j] == 0;
        for (int k = 0; k < touching; k++) {
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

    return 0;
}
