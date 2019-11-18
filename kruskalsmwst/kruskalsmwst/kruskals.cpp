/*
	Justin Stitt
	11/15/2019
	Uses Kruskal's algorithm to find the Minimum Weight Spanning Tree (MWST) of a given undirected and weighted graph.
	also uses disjoint-set data structure.
*/
#include <iostream>
#include <vector>
#include <string>

using namespace std;




struct vertex
{
	char name;
	int weight = 1;
};

struct edge {
	//has two vertices and a weight
	vertex *v1, *v2;
	int weight;
};

string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";


class Graph {
private:
public:
	vector<vertex> v;
	vector<edge> edges;//edges are vertex pairs (v1,v2)
	//a two edge graph example : { (v1,v2) , (v2,v3) } 
	Graph()
	{
		
	}
	Graph(int num_vertices, vector<vector<int>> connection_matrix,vector<int> weight_matrix)
	{
		v.resize(num_vertices);
		assign_vertex_names();
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
			cout << edges[x].v1->name << "---" << edges[x].weight << "---" << edges[x].v2->name << endl;
		}
	}
	void print_vertices()
	{
		cout << "Number of Vertices: " << v.size() << endl;
		for (int x = 0; x < v.size(); x++)
		{
			cout << v[x].name << endl;
		}
	}
	void assign_vertex_names()
	{
		for (int x = 0; x < v.size(); x++)
			v[x].name = alphabet[x];
	}

};

vector<edge> find_mwst(Graph);
bool is_in(vertex*, vector<edge>);
int calc_weight(vector<edge>);
int exists_in(vertex*, vector<vector<edge>>);
template<typename T>
void swap_elements(vector<T>&, int, int);
void sort_matrix(vector<int> &, vector<vector<int>>&);

int main()
{
	vector<vector<int>> connection_matrix = 
	{ {2,6},{3,6},{0,1},{0,4},{5,8},{5,7},{4,7},{2,3},{6,9},{6,5},{3,5},{6,8},{8,9},{7,8},{1,3},{1,2},{4,5} };
	vector<int> weight_matrix = { 18,4,3,2,5,6,7,8,9,10,11,12,13,15,16,17,1 };
	sort_matrix(weight_matrix, connection_matrix);

	cout << "Graph (Undirected and Weighted) \n";
	cout << string(50, '=') << endl;
	Graph g(10,connection_matrix, weight_matrix);//dont forget to change amount of nodes!!

	cout << string(50, '=') << endl;
	cout << "Minimum Weight Spanning Tree: \n";

	vector<edge> mwst = find_mwst(g);
	g.edges = mwst;
	g.print_graph();
	cout << string(50, '=') << endl;
	cout << "total weight: " << calc_weight(mwst) << endl;

	cin.get();
	return 0;
}

vector<edge> find_mwst(Graph g)//given a Graph g, find the minimum weight spanning tree (mwst) and return it some new edges
{	
	vector<edge> new_edges;
	edge e;
	vector<vector<edge>> sets;
	e = g.edges[0];
	new_edges.push_back(e);
	sets.push_back({ e });
	for (int x = 1; x < g.edges.size(); x++)
	{//loop through each edge we are looking to add
		int _v1 = exists_in(g.edges[x].v1, sets);
		int _v2 = exists_in(g.edges[x].v2, sets);
		if (    _v1 == _v2 && _v1 != -1)//if v1 and v2 exist in the same set. dont add
		{
			//do nothing
			continue;//just go next iteration... this wont work
		}
		else
		{
			if (_v2 == -1 && _v1 != -1)
			{//if v2 doesnt exist in any Sets but v1 does, then we just add v1,v2 to set that v1 is in
				int index = _v1;
				e = g.edges[x];
				new_edges.push_back(e);
				sets[index].push_back({e});
			}
			else if (_v2 != -1 && _v1 == -1)
			{//if v2 exists in a set and v1 doesnt exist in a set
				int index = _v2;
				e = g.edges[x];
				new_edges.push_back(e);
				sets[index].push_back({ e });
			}
			else if (_v1 == -1 && _v2 == -1)
			{//if both v1 and v2 dont exist in any sets, then make a new set with this edge in it and add to mwst
				e = g.edges[x];
				new_edges.push_back(e);
				sets.push_back({ e });
			}
			else if (_v1 != _v2)
			{//if both v1 and v2 exist but are not in the same set
				if (is_in(g.edges[x].v1, sets[_v2]))
				{
					continue;
				}
				else
				{
					//add edge
					e = g.edges[x];
					new_edges.push_back(e);
					for (edge ed : sets[_v2])
					{//puts all edges from the set containing v2 to the set containing v1
						sets[_v1].push_back(ed);
					}
					sets[_v2].erase(sets[_v2].begin(), sets[_v2].end());//erase the entire set after we've merged it
					//merge sets
				}
			}
		}
	}
	return new_edges;
}


bool is_in(vertex  *v, vector<edge> edges)
{
	for (int x = 0; x < edges.size(); x++)
	{
		if (edges[x].v1 == v || edges[x].v2 == v)
		{
			return true;
		}
	}
	return false;
}

int exists_in(vertex *v, vector<vector<edge>> sets)//returns the index of the first set v is in
{
	for (int x = 0; x < sets.size(); x++)
	{
		if (is_in(v, sets[x]))
		{
			return x;//the first time we find v in a set
		}
	}
	return -1;//not in set
}

int calc_weight(vector<edge> edges)
{
	int total = 0;
	for (edge e : edges)
	{
		total += e.weight;
	}
	return total;
}

template<typename T>
void swap_elements(vector<T> &vec,int n,int m)
{//swaps n and m
	T _element = vec[n];
	vec[n] = vec[m];
	vec[m] = _element;
}

void sort_matrix(vector<int> &weight_matrix,vector<vector<int>> &cm)
{
	for (int x = 0; x < weight_matrix.size(); x++)
	{
		for (int y = 0; y < weight_matrix.size() - 1; y++)
		{
			if (weight_matrix[y] > weight_matrix[y + 1])
			{
				swap_elements(weight_matrix, y, y + 1);
				swap_elements(cm, y, y + 1);
			}
		}
	}
}

