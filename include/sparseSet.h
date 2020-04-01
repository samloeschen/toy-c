#ifndef SPARSE_SET_H
#define SPARSE_SET_H

typedef struct SparseSet {
    int *sparse;
    int *dense;
    int count;
    int capacity;
    int maxValue;
} SparseSet;

SparseSet makeSparseSet(int maxValue, int capacity, SparseSet *set);
int setContains(int x, SparseSet *set);
int setAdd(int x, SparseSet *set);
int setRemove(int x, SparseSet *set);
SparseSet* setIntersection(SparseSet *a, SparseSet *b, SparseSet *result);
SparseSet* setUnion(SparseSet *a, SparseSet *b, SparseSet *result);

#endif