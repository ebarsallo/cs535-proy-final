// ---------------------------------------------------------------------------
// DisjointSet.h
// Class that represent a disjoint set of components (builded from a undirected 
// weighted graph).
//
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
		_forest[a].size += _forest[b].size;
	} else {
		_forest[a].head = b;
		_forest[b].size += _forest[a].size;

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
