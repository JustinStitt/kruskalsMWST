/*
	Justin Stitt
	11/15/2019
	Uses Kruskal's algorithm to find the Minimum Weight Spanning Tree (MWST) of a given undirected and weighted graph.
	also uses disjoint-set data structure.
*/
#include <iostream>
#include <vector>

using namespace std;




struct vertex
{
	int weight = 1;
};

struct edge {
	//has two vertices and a weight
	vertex *v1, *v2;
	int weight;
};

class Graph {
private:
public:
	vector<vertex> v;
	vector<edge> edges;//edges are vertex pairs (v1,v2)
	//a two edge graph example : { (v1,v2) , (v2,v3) } 
	Graph(int num_vertices, vector<vector<int>> connection_matrix,vector<int> weight_matrix)
	{
		v.resize(num_vertices);
		form_edges(edges,connection_matrix,weight_matrix);
		print_graph();
		//print_vertices();
	}
	void form_edges(vector<edge> &e,vector<vector<int>> cm,vector<int> wm)
	{
		edge new_edge;
		for (int x = 0; x < cm.size(); x++)//for each layer in our connection matrix  e.g: {{1,2},{2,3}}
		{
			new_edge.v1 = &v[cm[x][0]];
			new_edge.v2 = &v[cm[x][1]];
			new_edge.weight = wm[x];
			edges.push_back(new_edge);
		}//outer
	}
	void print_graph(){
		for (int x = 0; x < edges.size(); x++) {
			cout << edges[x].v1 << "---" << edges[x].weight << "---" << edges[x].v2 << endl;
		}
	}
	void print_vertices()
	{
		cout << "Number of Vertices: " << v.size() << endl;
		for (int x = 0; x < v.size(); x++)
		{
			cout << &v[x] << endl;
		}
	}

};


int main()
{
	vector<vector<int>> connection_matrix = { {0,1},{1,2},{2,3},{3,1} };//which indexed vertices to connect (kind of arbitrary but relativity matters
	vector<int> weight_matrix = { 2,1,2,4 };//parallel to our connection_matrix

	Graph g(4,connection_matrix, weight_matrix);


	cin.get();
	return 0;
}
