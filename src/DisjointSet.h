#pragma once
// ---------------------------------------------------------------------------
// DisjointSet.h
// Class that represent a disjoint set of components (builded from a undirected 
// weighted graph).
//
//
// Remarks
// Header file
// ---------------------------------------------------------------------------
#include "Graph.h"


// ---------------------------------------------------------------------------
// Data structures
// ---------------------------------------------------------------------------

// element 
struct elem {
	int head;
	int rank;
	int size;
};
typedef elem elem_t;


// ---------------------------------------------------------------------------
// Classes Definition
// ---------------------------------------------------------------------------

/// DisjointSet
/// <summary>Class that represent a disjoint set forest</summary>
class DisjointSet
{
private:
	elem_t *_forest;
	int _num;

public:
	DisjointSet(void);
	DisjointSet(int);
	~DisjointSet(void);

	// Operations
	int find(int);	
	void join(int, int);

	// Getters & Setters
	int getSize();
	int getSize(int);
};

