#include <stdio.h>
#include "utils.h"

constexpr uint32 ID_LENGTH = 5;
typedef struct Ingredient {
    char id[ID_LENGTH];
    int32 index;
} Ingredient;

// TODO: Implement with dynamic arrays instead.
constexpr uint32 MAX_INGREDIENTS = 2100, MAX_RECIPE_SIZE = 200;

// TODO: Implement with a hashmap.
typedef struct IngredientMap {
    Ingredient entries[MAX_INGREDIENTS];
    uint32 size;
} IngredientMap;

internal IngredientMap mapCreate() {
    IngredientMap map = {0};
    map.size = 0;
    return map;
}

bool idComp(char a[ID_LENGTH], char b[ID_LENGTH]) {
    for (uint32 i = 0; i < ID_LENGTH; i++) {
        if (a[i] != b[i]) {
            return a[i] < b[i];
        }
    }
    return false;
}

bool idEq(char a[ID_LENGTH], char b[ID_LENGTH]) {
    for (uint32 i = 0; i < ID_LENGTH; i++) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}

internal uint32 mapFind(IngredientMap *map, char key[ID_LENGTH]) {
    uint32 lo = 0, hi = map->size;

    if (idEq(map->entries[lo].id, key) || 
            idComp(key, map->entries[lo].id)) {
        return lo;
    }
    while (lo + 1 < hi) {
        uint32 mid = (hi - lo) / 2 + lo;
        if (idComp(map->entries[mid].id, key)) {
            lo = mid;
        } else {
            hi = mid;
        }
    }

    return lo + 1;
}

internal int32 mapGet(IngredientMap *map, char key[ID_LENGTH]) {
    uint32 pos = mapFind(map, key); 
    if (pos < map->size && idEq(map->entries[pos].id, key)) {
        return map->entries[pos].index;
    } else {
        return -1;
    }
}

internal void mapSet(IngredientMap *map, char key[ID_LENGTH], int32 value) {
    uint32 pos = mapFind(map, key); 
    assert(pos != MAX_INGREDIENTS);
    if (idEq(map->entries[pos].id, key)) {
        map->entries[pos].index = value;
    } else {
        uint32 slot = map->size;
        assert(slot < MAX_INGREDIENTS);
        while (slot > pos) {
            map->entries[slot] = map->entries[slot - 1];           
            slot--;
        }

        for (uint32 i = 0; i < ID_LENGTH; i++) {
            map->entries[slot].id[i] = key[i];
        }
        map->entries[slot].index = value;
        map->size++;
    }
}

internal int32 getIndex(IngredientMap *map, char key[ID_LENGTH]) {
    int32 result = mapGet(map, key);
    if (result == -1) {
        result = (int32)map->size;
        mapSet(map, key, result);
    }
    return result;
}

internal void copyId(char **buf, char out[ID_LENGTH]) {
    for (uint32 i = 0; i < ID_LENGTH-1; i++) {
        assert(('A' <= **buf && **buf <= 'Z') || 
                ('1' <= **buf && **buf <= '3'));
        out[i] = *((*buf)++);
    }
    out[ID_LENGTH - 1] = '\0';
}

internal int32 dfs(int32 adj[MAX_INGREDIENTS][MAX_RECIPE_SIZE], bool visited[MAX_INGREDIENTS], int32 u) {
    int32 result = 0;
    for (uint32 i = 0; i < MAX_RECIPE_SIZE && adj[u][i] != -1; i++) {
        int32 v = adj[u][i];
        if (!visited[v]) {
            visited[v] = true;
            result += 1 + dfs(adj, visited, v);
        }
    }
    return result;
}

int main() {
    char *input = nullptr;
    size_t capacity = 0;
    ssize_t bytesRead;

    int32 recipeInverse[MAX_INGREDIENTS][MAX_RECIPE_SIZE] = {0};
    uint32 recipeIndex[MAX_INGREDIENTS] = {0};
    for (uint32 itemIndex = 0; itemIndex < MAX_INGREDIENTS; itemIndex++) {
        for (uint32 recipeInverseIndex = 0; recipeInverseIndex < MAX_RECIPE_SIZE; recipeInverseIndex++) {
            recipeInverse[itemIndex][recipeInverseIndex] = -1;
        }
    }
    IngredientMap indexes = mapCreate();
    // NOTE: Edited the input file so NULLv1, NULLv2, NULLv3 become 1ULL, 2ULL, 3ULL.
    // TODO: Improve parsing so ingredients map can take strings of variable length of up to 6 chars and copyId chunks the input properly.
    while ((bytesRead = getline(&input, &capacity, stdin)) >= 0) {
        assert(bytesRead % 5 == 0);
        char *cursor = input;
        char item[ID_LENGTH];
        copyId(&cursor, item);
        cursor++;
        int32 itemIndex = getIndex(&indexes, item);
        while (*cursor) {
            char ingredient[ID_LENGTH];
            copyId(&cursor, ingredient);
            cursor++;
            int32 ingredientIndex = getIndex(&indexes, ingredient);
            uint32 *ingredientRecipeIndex = &recipeIndex[ingredientIndex];
            assert(*ingredientRecipeIndex < MAX_RECIPE_SIZE);
            recipeInverse[ingredientIndex][*ingredientRecipeIndex] = itemIndex;
            (*ingredientRecipeIndex)++;
        }
    }

    char nullv1Id[ID_LENGTH] = "1ULL";
    int32 nullv1Index = getIndex(&indexes, nullv1Id);
    bool visitedByV1[MAX_INGREDIENTS] = {0};
    visitedByV1[nullv1Index] = true;
    int32 nullv1Count = dfs(recipeInverse, visitedByV1, nullv1Index);

    printf("Products of NULLv1: %d\n", nullv1Count);

    char nullv2Id[ID_LENGTH] = "2ULL";
    int32 nullv2Index = getIndex(&indexes, nullv2Id);
    bool visitedByV2[MAX_INGREDIENTS] = {0};
    visitedByV2[nullv2Index] = true;
    int32 nullv2Count = dfs(recipeInverse, visitedByV2, nullv2Index);

    printf("Products of NULLv2: %d\n", nullv2Count);

    char nullv3Id[ID_LENGTH] = "3ULL";
    int32 nullv3Index = getIndex(&indexes, nullv3Id);
    bool visitedByV3[MAX_INGREDIENTS] = {0};
    visitedByV3[nullv3Index] = true;
    int32 nullv3Count = dfs(recipeInverse, visitedByV3, nullv3Index);

    printf("Products of NULLv3: %d\n", nullv3Count);

    int32 dangerCount = 0;
    for (uint32 index = 0; index < indexes.size; index++) {
        int32 nullVersions =
            (int32)visitedByV1[index] +
            (int32)visitedByV2[index] +
            (int32)visitedByV3[index];
        if (nullVersions >= 2) {
            dangerCount++;
        }
    }

    printf("%d different products use at least two different versions of NULL\n", dangerCount);

    return 0;
}

