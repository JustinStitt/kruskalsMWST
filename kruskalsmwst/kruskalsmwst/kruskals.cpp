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
	Graph()
	{
		
	}
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

vector<edge> find_mwst(Graph);
bool is_in(vertex*, vector<edge>);
vector<edge> find_mwst_new(Graph);
int calc_weight(vector<edge>);

int main()
{
	vector<vector<int>> connection_matrix = 
	{ {0,1},{1,2},{2,0},{0,3},{2,3} };
	vector<int> weight_matrix = {1,2,3,4,5}; 
	cout << "Graph (Undirected and Weighted) \n";
	cout << string(50, '=') << endl;
	Graph g(4,connection_matrix, weight_matrix);//dont forget to change amount of nodes!!

	cout << string(50, '=') << endl;
	cout << "Minimum Weight Spanning Tree: \n";

	vector<edge> mwst = find_mwst_new(g);
	g.edges = mwst;
	g.print_graph();
	cout << string(50, '=') << endl;
	cout << "total weight: " << calc_weight(mwst) << endl;

	cin.get();
	return 0;
}

vector<edge> find_mwst(Graph g)//given a Graph g, find the minimum weight spanning tree (mwst) and return it some new edges
{	//our weight matrix has to be sorted least to greatest, done manually for now
	vector<edge> new_edges;//we will append our mwst edges here iteratively
	//here we are manually adding our first edge as it is arbitrarily chosen to be in our mwst since its sorted least to greatest
	edge e;
	e.v1 = g.edges[0].v1;
	e.v2 = g.edges[0].v2;
	e.weight = g.edges[0].weight;
	new_edges.push_back(e);
	for (int x = 0; x < g.edges.size(); x++)
	{//loop through every edge in our graph starting with lowest weight edges since its pre-sorted
		for (int y = 0; y < new_edges.size(); y++)
		{
			if (g.edges[x].v2 == new_edges[y].v1 || g.edges[x].v2 == new_edges[y].v2)
			{//we found a match for our v2 in our mwst already so we dont add it
				break;
			}

			if (y == new_edges.size() - 1)//if we are on last index and we havent break yet
			{
				e = g.edges[x];//does this copy values or mem. address?
				new_edges.push_back(e);
				break;
			}
		}
	}
		return new_edges;
}

vector<edge> find_mwst_new(Graph g)
{
	vector<edge> new_edges;
	edge e;
	vector<vector<edge>> sets;

	//cout << "a" << is_in(g.edges[0].v1, sets[0]) << endl;
	e = g.edges[0];
	new_edges.push_back(e);
	sets.push_back({ e });
	for (int x = 0; x < g.edges.size(); x++)
	{//loop through all edges we wish to check to place into ur mwst
		for (int y = 0; y < sets.size(); y++)
		{
			if (is_in(g.edges[x].v1, sets[y])  && is_in(g.edges[x].v2,sets[y]) )
			{//if v1 and v2 are in set y then dont add
				break;
			}
			else if (is_in(g.edges[x].v1, sets[y]) && !is_in(g.edges[x].v2, sets[y]))
			{//if v1 is in set y but v2 is not then add v2 to set y and to mwst

				//if v1 is in the set y and v2 is not in the set y, then check if v2 is in ANY other set Z, if so. merge Y and Z sets.
				for (int z = 0; z < sets.size(); z++)
				{
					if (is_in(g.edges[x].v2, sets[z]))
					{
						//MERGE SET Y AND Z
						for (edge ed : sets[z])
						{
							sets[y].push_back(ed);
						}
						sets[z].erase(sets[z].begin(), sets[z].end());
					}
				}
				e = g.edges[x];
				new_edges.push_back(e);
				sets[y].push_back(e);
				break;

			}
			else if (!is_in(g.edges[x].v1, sets[y]) && !is_in(g.edges[x].v2, sets[y]))
			{//if both v1 and v2 are not in set y then make a new set
				e = g.edges[x];
				new_edges.push_back(e);
				sets.push_back({e});
				break;
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

int calc_weight(vector<edge> edges)
{
	int total = 0;
	for (edge e : edges)
	{
		total += e.weight;
	}
	return total;
}