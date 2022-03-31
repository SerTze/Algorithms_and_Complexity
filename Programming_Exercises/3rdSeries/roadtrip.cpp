// First Source used in structs DisjointSet,Graph:
// https://www.geeksforgeeks.org/kruskals-minimum-spanning-tree-algorithm-greedy-algo-2/

#include <iostream>
#include <algorithm>
#include <vector>
#include <list>
#include <cmath>
#include <limits.h>

using namespace std;

struct edge{
	int u, v;
	int w;
}; 

// Struct used for a Cartesian node
struct node{
    int left=0;
	int right=0;
	int value;
	int parent; 	
	int id;
};


// Struct used for Disjoint Sets
struct DisjointSet
{
    int *parent, *rnk;
    int a;
 
    // Default Constructor
	DisjointSet() { 
		this->a = 0; 
	}

	// Constructor
    DisjointSet(int a)
    {
        this->a = a;

		// Memory Allocation
        parent = new int[a+1];
        rnk = new int[a+1];

        //Initialization
        for (int i = 0; i <= a; i++)
        {
            rnk[i] = 0;
            //Set every element as its parent
            parent[i] = i;
        }
    }
 
    /* Function used to find the parent of a node 'u'
    using Path Compression */
    int find(int u)
    {
        if (u != parent[u])
            parent[u] = find(parent[u]);
        return parent[u];
    }
 
    /* Union according to rank. The tree with the 
	smaller height should be a subtree of the other 
	tree*/
    void Union(int x, int y)
    {
        x = find(x);
		y = find(y);

        if (rnk[x] > rnk[y]){
            parent[y] = x;
		}
        else{
            parent[x] = y;
		}
        if (rnk[x] == rnk[y]){
            rnk[y]++;
		}
    }
};

class Graph
{
	vector< vector<int> > AdjList;

public:
	int V, E;

	Graph() {}
	Graph(int V, int E);
	vector<edge> edges;
	
	void addEdge(int u, int v, int d);
	void GraphInit(int E);
	void Kruskal(vector<edge> edges_list);
};

/*<---------Functions--------->*/
// Constructor
Graph::Graph(int V,int E)
{
	this->V = V;
	this->E = E;

	AdjList = vector< vector<int>>(V+1, vector<int>());
}

void Graph::addEdge(int u, int v, int d)
{
	AdjList[u].push_back(v);
	AdjList[v].push_back(u);

	edges.push_back({u, v, d});
}

void Graph::GraphInit(int E)
{
	int i,j;
	int d;
	for (int l=0; l<E; l++) {
		cin >> i;
		cin >> j;
		cin >> d;
		addEdge(i,j,d);
	}
}

bool edgesComp(edge e1, edge e2) { 
	return e1.w < e2.w; 
}

/* Based on the code source code:
https://www.geeksforgeeks.org/kruskals-minimum-spanning-tree-algorithm-greedy-algo-2/ */
void Graph::Kruskal(vector<edge> edges_list)
{
    sort(edges_list.begin(), edges_list.end(), edgesComp);
	
    // Disjoint Set
    DisjointSet Disjoint(V);
 

    vector<edge>::iterator it;
    for (it=edges_list.begin(); it!=edges_list.end(); it++)
    {
        int u = it->u;
        int v = it->v;
 		
        int uu = Disjoint.find(u);
        int vv = Disjoint.find(v);
	 
        // take that edge in MinimumSpanningTree if it does form a cycle
        if (uu != vv)
        {
            addEdge(u, v, it->w); 
            Disjoint.Union(uu, vv);
        }
    }
}

/*This Struct is used for knowing before executing a Union
of two nodes their parent */
struct parentInfo
{
	DisjointSet Disjoint;
	vector<int> Parent;

	parentInfo(int V) {
		Parent = vector<int>(V+1);
		for (int i=1; i<=V; i++) { 
			Parent[i] = i; 
		}
		DisjointSet Disjoint(V);
		this->Disjoint = Disjoint;
	}

	void setParent(int u, int v, int p)
	{
		Disjoint.Union(u, v);
		Parent[Disjoint.find(u)] = p;
	}

	int get(int u)
	{
		return Parent[Disjoint.find(u)];
	}
};

/* Below class and functions are based from:
https://www.topcoder.com/thrive/articles/Range%20Minimum%20Query%20and%20Lowest%20Common%20Ancestor
*/
class Solver
{
	int V,E;
	Graph MinimumSpanningTree;
	node* Cartesian;
	int *Ev, *L, *H;
	int** M;

public:
	Solver(Graph MinimumSpanningTree);
	void cartisian();
	void ELH(int node, int level, int* counter);
	void QueryPreprocessing(int* A, int N);
	int RangeMinimumQuery(int* L, int i, int j);
	int Query(int i, int j);
};

// Constructor
Solver::Solver(Graph MinimumSpanningTree)
{
	this->MinimumSpanningTree = MinimumSpanningTree;
	this->E = MinimumSpanningTree.E;
	this->V = MinimumSpanningTree.V;
	this->Cartesian = (node*)malloc((V + E + 1) * sizeof(node));

	cartisian();

	this->Ev = (int*)malloc(2*(V+E)*sizeof(int));
	this->L = (int*)malloc(2*(V+E)*sizeof(int));
	this->H = (int*)malloc(2*(V+E)*sizeof(int));

	for (int j=0; j<2*(V+E); j++) {
		Ev[j] = L[j] = H[j] = 0;
	}
	int counter = 0;
	ELH(V + E, 0, &counter);
	int nV = 2*(V+E);
	this->M = (int**)malloc(nV*sizeof(int*));
	for (int i=0; i<nV; i++) { M[i] = (int*)malloc(ceil(log2(nV))*sizeof(int)); }
	QueryPreprocessing(L, 2*(E+V));
}

// This function is used to create the Cartesian tree from the MinimumSpanningTree
void Solver::cartisian()
{
	sort(MinimumSpanningTree.edges.begin(), MinimumSpanningTree.edges.end(), edgesComp);
	for (int i=1; i<=V+E; i++) {
		Cartesian[i].left = 0;
		Cartesian[i].right = 0;
		Cartesian[i].value = i;
		Cartesian[i].parent = 0;
		Cartesian[i].id = i;
	}

	// Create parentInfo Struct
	parentInfo parents(V);

	for (int i=V+1; i<=V + E; i++) {
		edge min_edge = MinimumSpanningTree.edges[i-V-1];
		int u = min_edge.u, v = min_edge.v;

		// Assign the correct kids
		Cartesian[i].left = parents.get(u);
		Cartesian[i].right = parents.get(v);
		Cartesian[i].value = min_edge.w;

		// Update the parent values in the cartesian tree
		Cartesian[ Cartesian[i].left ].parent = i;
		Cartesian[ Cartesian[i].right].parent = i;

		parents.setParent(u, v, i);
	}
}

// This function is used for the Eulerian Tour
void Solver::ELH(int node, int level, int* counter)
{
	(*counter)++;
	// Update values
	L[*counter] = level;
	Ev[*counter] = node;
	if (H[node] ==  0) { 
		H[node] = *counter; 
	}

	// DFS 
	if (Cartesian[node].left != 0) {
		ELH(Cartesian[node].left, level+1, counter);

		// Update
		(*counter)++;
		L[*counter] = level;
		Ev[*counter] = node;
	}

	if (Cartesian[node].right != 0) {
		ELH(Cartesian[node].right, level+1, counter);

		// Update
		(*counter)++;
		L[*counter] = level;
		Ev[*counter] = node;	
	}
}

/* This function is used for Sparce Matrix Optimization Method(as seen on the source link)
to solve RangeMininumQueries with O(nlogn) preprocess complexity and O(1) Query complexity
*/
void Solver::QueryPreprocessing(int* A, int N)
{
	// Code Source:
	// https://www.topcoder.com/thrive/articles/Range%20Minimum%20Query%20and%20Lowest%20Common%20Ancestor
	int i, j;
	for (i = 0; i < N; i++)
		M[i][0] = i;
	for (j = 1; 1 << j <= N; j++)
		for (i = 0; i + (1 << j) - 1 < N; i++)
			if (A[M[i][j - 1]] < A[M[i + (1 << (j - 1))][j - 1]])
				M[i][j] = M[i][j - 1];
			else
				M[i][j] = M[i + (1 << (j - 1))][j - 1];
}

int Solver::RangeMinimumQuery(int* A, int i, int j)
{	
	int k = log2(j - i + 1);
	if (A[M[i][k]] <= A[M[j - (int)pow(2,k) + 1][k]]) 
		return M[i][k];
	else
		return M[j - (int)pow(2,k) + 1][k];
}

/* This function is used in order to convert the LCA of the 
cartesian tree problem to the RMQ one because the is RMQ problem 
problem is solvable in O(1)*/
int Solver::Query(int i, int j)
{
	return Cartesian[Ev[RangeMinimumQuery(L, min(H[i],H[j]), max(H[i],H[j]))]].value;
}

int main()
{
	int Vertices, E, Q, u, v, query;
	cin >> Vertices >> E;

	Graph graph(Vertices,E), MinimumSpanningTree(Vertices, Vertices-1);
	graph.GraphInit(E);

	cin >> Q;

	MinimumSpanningTree.Kruskal(graph.edges);
	Solver Solve(MinimumSpanningTree);
	for (int i=0; i<Q; i++) {
		cin >> u >> v;
		query = Solve.Query(u, v);
		cout << query << endl;
	}
	return 0;
}
