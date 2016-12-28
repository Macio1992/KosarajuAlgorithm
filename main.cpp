#include <iostream>
#include "Graph.h"

using namespace std;

int main() {
	
	Graph graph(9);
	graph.fillTheMatrix();
	graph.wypisz();
	graph.DFSPostOrder(0);
	graph.KosarajuAlgorithm();
	
	return 0;
}
