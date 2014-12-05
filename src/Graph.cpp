// ---------------------------------------------------------------------------
// Graph.cpp
// Undirected graph representation (with weights).
//
// Remarks
// None.
// ---------------------------------------------------------------------------
#include "Graph.h"


/// <summary>Boolean operator to sort a graph.</summary>
bool pred_edge_t (edge_t i, edge_t j) { 
	return i.w < j.w; 
}


/// Graph
/// <summary>Construct a graph of a n size</summary>
Graph::Graph(int n)
{
	_edges = new edge_t[n];
	_size = n;
}


///<summary>Destroy a graph object.</summary>
Graph::~Graph()
{
	delete [] _edges;
}


/// setEdge
/// <summary>Set the values (vertex and weight) for an edge</summary>
void 
Graph::setEdge(int i, int s, int t, float w)
{
	resize(i);
	_edges[i].s = s;
	_edges[i].t = t;
	_edges[i].w = w;
}


/// setEdge
/// <summary>Set the values (vertex and weight) for an edge</summary>
void 
Graph::addEdge(int s, int t, float w)
{
	_numEdges++;
	int i = _numEdges;

	_edges[i].s = s;
	_edges[i].t = t;
	_edges[i].w = w;
}


/// getNumEdges
/// <summary>Return the size of the array of edges.</summary>
int
Graph::getSize()
{
	return _size;
}


/// getNumEdges
/// <summary>Return the number of edges in the graph</summary>
int
Graph::getNumEdges()
{
	return _numEdges;
}


/// getWeight
/// <summary>Return the weight of a specific edge.</summary>
float 
Graph::getWeight(int i)
{
	return _edges[i].w;
}


/// getEdgeArray
/// <summary>Return the array of edges</summary>
edge_t*
Graph::getEdgeArray()
{
	return _edges;
}


/// getEdge
/// <summary>Return the edge in the ith position.</summary>
edge_t  
Graph::getEdge(int i)
{
	return _edges[i];
}
