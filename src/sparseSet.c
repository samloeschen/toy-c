#include <stdlib.h>

int min(int a, int b) {
    return a < b ? a : b;
};
int max(int a, int b) {
    return a > b ? a : b;
};

typedef struct SparseSet {
    int *sparse;
    int *dense;
    int count;
    int capacity;
    int maxValue;
} SparseSet;

void makeSparseSet(int maxValue, int capacity, SparseSet *set) {
    set->sparse     = (int *)malloc(sizeof(int) * (maxValue + 1));
    set->dense      = (int *)malloc(sizeof(int) * capacity);
    set->count      = 0;
    set->capacity   = capacity;
    set->maxValue   = maxValue;
};

int setContains(int x, SparseSet *set) {
    if (x > set->maxValue) {
        return -1;
    }
    int *dense  = set->dense;
    int *sparse = set->sparse;
    int n       = set->count;
    if (sparse[x] < n && dense[sparse[x]] == x) {
        return (sparse[x]);
    }
    return -1; //not found
}

int setAdd(int x, SparseSet *set) {
    if (x > set->maxValue) {
        return -1;
    }
    if (set->count >= set->capacity) {
        return -1; // set would overflow
    }
    if (setContains(x, set) != -1) { // set already contains value
        return -1;
    }
    int n       = set->count;
    int *dense  = set->dense;
    int *sparse = set->sparse;
    dense[n] = x;
    sparse[x] = n;

    (set->count)++;
    return 0;
}

int setRemove(int x, SparseSet *set) {
    if (setContains(x, set)) {
        return -1;
    }
    int n       = set->count;
    int *dense  = set->dense;
    int *sparse = set->sparse;

    int temp = dense[n - 1];
    dense[sparse[x]] = temp;
    sparse[temp] = sparse[x];

    (set->count)--;
    return 0;
}

void setIntersection(SparseSet *a, SparseSet *b, SparseSet *result) {
    int capacity = min(a->capacity, b->capacity);
    int maxValue = max(a->maxValue, b->maxValue);
    makeSparseSet(maxValue, capacity, result);

    int *dense;
    int n;
    SparseSet *targetSet;
    if (a->count < b->count) {
        dense = a->dense;
        n = a->count;
        targetSet = b;
    } else {
        dense = b->dense;
        n = b->count;
        targetSet = a;
    }
    for (int i = 0; i < n; i++) {
        if (setContains(dense[i], targetSet) != -1) {
            setAdd(dense[i], result);
        }
    }
}

void setUnion(SparseSet *a, SparseSet *b, SparseSet *result) {
    int capacity = a->count + b->count;
    int maxValue = max(a->maxValue, b->maxValue);
    makeSparseSet(maxValue, capacity, result);
    for (int i = 0; i < a->count; i++) {
        setAdd(a->dense[i], result);
    }
    for (int i = 0; i < b->count; i++) {
        setAdd(b->dense[i], result);
    }
}