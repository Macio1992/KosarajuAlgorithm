#include "Graph.h"
#include "Stack.h"
#include <iostream>
#include <fstream>

using namespace std;

Graph::Graph(int x){
	
	size = x;
		
	graph = new int*[size];
	for(int i = 0; i < size; i++)
		graph[i] = new int[size];
		
	zeroGraph();
	
}

Graph::~Graph(){
	
	for (int i = 0; i < size ; i++)
		delete [] graph[i];
	delete [] graph;
	
	delete [] visited;
	
	delete [] postorder;
}

void Graph::wypisz(){
	
	cout<<"  0 1 2 3 4 5"<<endl<<endl;
	for(int i = 0; i < size; i++){
		cout<<i<<" ";
		for(int j = 0; j < size; j++){
			cout << graph[i][j]<<" ";
		}
		cout <<"\n";
	}
	cout <<"\n";
	
}

void Graph::zeroGraph(){
	
	for(int i = 0; i < size; i++)
		for(int j = 0; j < size; j++)
			graph[i][j] = 0;
	
}

void Graph::fillTheMatrix(){
	ifstream ifile;
	ifile.open("data.txt");
	int a, b;
	while(ifile >> a >> b){
		graph[a][b] = 1;
		//graph[b][a] = 1;
	}
	ifile.close();
}

void showVisited(bool *tab, int n){
	
	cout<<"visited: "<<endl;
	for(int i = 0; i < n; i++){
		cout<<i<<". ";
		if(tab[i]) cout<<"TRUE"<<endl;
		else cout<<"FALSE"<<endl;
	}
	
}
void showPostOrder(int n, int *po){
	
	for(int i = 0; i < n; i++)
		cout<<po[i]<<" ";
	cout<<endl;
	
}

int Graph::znajdzSasiada(int w){
	int l = 0;
	for(int i = 0; i < size; i++){
		if(graph[w][i] != 0 && !visited[i] && l < 1){
			l++;
			w = i;
		}
	}
	return w;
}

bool Graph::sprawdzCzyMaSasiadow(int w){
	
	int l = 0;
	for(int i = 0; i < size; i++)
		if(graph[w][i] != 0 && !visited[i])
			l++;
	
	return (l != 0) ? true : false;
}

void Graph::transposeMatrix(){
	
	for(int i = 0; i < size; i++)
		for(int j = 0; j < size; j++)
			if(graph[i][j] == 1 && graph[j][i] == 1){
				graph[i][j] = 2;
				graph[j][i] = 2;
			}
	
	for(int i = 0; i < size; i++)
		for(int j = 0; j < size; j++)
			if(graph[i][j] == 1){
				graph[i][j] = -1;
				graph[j][i] = 2;
			}
	
	for(int i = 0; i < size; i++)
		for(int j = 0; j < size; j++)
			if(graph[i][j] == -1){
				graph[i][j] = 0;
			}
	
}

void Graph::DFSPostOrder(int beg){
	
	visited = new bool[size];
	for(int i = 0; i < size; i++) visited[i] = false;
	
	postorder = new int[size];
	for(int i = 0; i < size; i++) postorder[i] = 0;
	
	int k = 0;

	Stack stack(size);
	
	stack.push(beg);
	visited[beg] = true;
	
	while(!stack.checkIfStackEmpty()){
		stack.push(znajdzSasiada(stack.getStackTop()));
		visited[stack.getStackTop()] = true;
		while(!stack.checkIfStackEmpty() && !sprawdzCzyMaSasiadow(stack.getStackTop())){
			visited[stack.getStackTop()] = true;
			postorder[k] = stack.getStackTop();
			k++;
			stack.pop();
		}
	}
	
	//showVisited(visited, size, postorder);
		
}

void Graph::KosarajuAlgorithm(){
	
	transposeMatrix();
	
	int *x = new int[size];
	
	for(int i = 0; i < size; i++)
		x[i] = postorder[i];
	
	cout<<"Zbiory wierzcholkow indukujace silnie spojne skladowe digrafu: "<<endl;
	int i = 1;
	while(i <= size){
		if(x[size-i] != -1){
			DFSPostOrder(x[size-i]);
			
			cout<<"{ ";
			for(int i = 0; i < size; i++)
				if(visited[i]){
					for(int j = 0; j < size; j++){
						if(x[j] == i){
							cout<<x[j]<<" ";
							x[j] = -1;
						}
					}
				}
			cout<<"}"<<endl;
		}
		
		i++;
	}
	
	delete [] x;
}
