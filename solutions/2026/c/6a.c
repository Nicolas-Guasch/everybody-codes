#include <stdio.h>
#include "utils.h"

constexpr uint32 ID_LENGTH = 5;
typedef struct Ingredient {
    char id[ID_LENGTH];
    int32 index;
} Ingredient;

constexpr uint32 MAX_INGREDIENTS = 100, MAX_RECIPE_SIZE = 5;

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
        assert('A' <= **buf && **buf <= 'Z');
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

    int32 recipe[MAX_INGREDIENTS][MAX_RECIPE_SIZE] = {0};
    for (uint32 itemIndex = 0; itemIndex < MAX_INGREDIENTS; itemIndex++) {
        for (uint32 recipeIndex = 0; recipeIndex  < MAX_RECIPE_SIZE; recipeIndex++) {
            recipe[itemIndex][recipeIndex] = -1;
        }
    }
    IngredientMap indexes = mapCreate();
    while ((bytesRead = getline(&input, &capacity, stdin)) >= 0) {
        assert(bytesRead % 5 == 0);
        char *cursor = input;
        char item[ID_LENGTH];
        copyId(&cursor, item);
        cursor++;
        int32 itemIndex = getIndex(&indexes, item);
        uint32 recipeIndex = 0;
        while (*cursor) {
            char ingredient[ID_LENGTH];
            copyId(&cursor, ingredient);
            cursor++;
            int32 ingredientIndex = getIndex(&indexes, ingredient);
            recipe[itemIndex][recipeIndex++] = ingredientIndex;
        }
    }

    char nullId[ID_LENGTH] = "NULL";
    int32 nullIndex = getIndex(&indexes, nullId);
    bool visited[MAX_INGREDIENTS] = {0};
    visited[nullIndex] = true;
    int32 nullCount = dfs(recipe, visited, nullIndex);

    printf("Ingredients of NULL: %d\n", nullCount);

    return 0;
}
