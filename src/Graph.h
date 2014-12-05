#pragma once
// ---------------------------------------------------------------------------
// Graph.h
// Undirected graph representation (with weights).
//
//
// Remarks
// Header file
// ---------------------------------------------------------------------------
#include <algorithm>


/// <summary>Edge datatype</summary>
struct edge {
	float w;
	int s, t;
};
typedef edge edge_t;

/// <summary>Boolean operator to sort a graph.</summary>
bool pred_edge_t (edge_t i, edge_t j);


class Graph
{
#define resize(a) \
	if (a > _numEdges) _numEdges = a;

private:
	edge_t *_edges;
	int _numEdges;
	int _size;

public:
	Graph(void);
	Graph(int n);
	~Graph(void);

	// Operations
	void setEdge(int, int, int, float);
	void addEdge(int, int, float);

	// Getters & Setters
	int getNumEdges();
	int getSize();
	float getWeight(int);
	edge_t* getEdgeArray();
	edge_t  getEdge(int);
};

