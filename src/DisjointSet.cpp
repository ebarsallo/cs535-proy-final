// ---------------------------------------------------------------------------
// DisjointSet.h
// Class that represent a disjoint set of components (builded from a undirected 
// weighted graph).
//
// author. ebarsall
//
// Remarks
// None
// ---------------------------------------------------------------------------
#include "DisjointSet.h"


// ---------------------------------------------------------------------------
// Classes Definition
// ---------------------------------------------------------------------------

/// DisjointSet
/// <summary>Construct a Disjoint Set of n size/summary>
DisjointSet::DisjointSet(int n)
{
	_forest = new elem_t[n];
	_num = n;

	// init
	for (int i=0; i<n; i++) {
		_forest[i].head = i;
		_forest[i].rank = 0;
		_forest[i].size = 1;
		_forest[i].weight = 0;
	}
}


/// DisjointSet
/// <summary>Construct a Disjoint Set from a graph G</summary>
DisjointSet::DisjointSet(Graph *g)
{
	int n = g->getNumEdges();

	_forest = new elem_t[n];
	_num = n;

	// init
	for (int i=0; i<n; i++) {
		_forest[i].head = i;
		_forest[i].rank = 0;
		_forest[i].size = 1;
		_forest[i].weight = g->getWeight(i);
	}
}


/// <summary>Destroy a Disjoin Set class.</summary>
DisjointSet::~DisjointSet() {
	delete [] _forest;
}


/// find
/// <summary>Find an object "a" in the forest</summary>
int
DisjointSet::find(int a)
{
	int b = a;
	while(b != _forest[b].head) {
		b =_forest[b].head;
	}
	_forest[a].head = b;
	return b;
}


/// join
/// <summary>Join two disjoint set.</summary>
void
DisjointSet::join(int a, int b)
{
	if (_forest[a].rank > _forest[b].rank) {
		_forest[b].head = a;
		_forest[a].size   += _forest[b].size;
		_forest[a].weight += _forest[b].weight;
	} else {
		_forest[a].head = b;
		_forest[b].size   += _forest[a].size;
		_forest[b].weight += _forest[a].weight;

		if (_forest[a].rank == _forest[b].rank) _forest[b].rank++;
	}

	_num --;	// Decrement size of forest;
}



/// getSize()
/// <summary>Return the size of the whole forest.</summary>
int
DisjointSet::getSize()
{
	return _num;
}


/// getSize(int a)
/// <summary>Return the size of an specific element</summary>
int
DisjointSet::getSize(int a)
{
	return _forest[a].size;
}


/// getElem(int i)
/// <summary>Return the ith element of the forest.</summary>
elem_t 
DisjointSet::getElem(int i)
{
	return _forest[i];
}