#ifndef GRAPH_H
#define GRAPH_H

class Graph{
	
	private:
		int **graph;
		int size;
		bool *visited;
		int *postorder;
		
	public:
		Graph(int);
		~Graph();
		
		void wypisz();
		void zeroGraph();
		void fillTheMatrix();
		void DFSPostOrder(int);
		bool sprawdzCzyMaSasiadow(int);
		int znajdzSasiada(int);
		void transposeMatrix();
		void KosarajuAlgorithm();
};

#endif
