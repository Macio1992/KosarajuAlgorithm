#include "Graph.h"
#include "Stack.h"
#include <iostream>
#include <fstream>

using namespace std;

Graph::Graph(int x){
	
	//w konstruktorze tworzymy: macierz (graph), tablice odwiedzonych wierzcholkow (visited),
	//tablice do przechowania kolejnosci POST-ORDER (postorder)
	
	size = x;
		
	graph = new int*[size];
	for(int i = 0; i < size; i++)
		graph[i] = new int[size];
	
	//zerowanie macierzy
	zeroGraph();
	
	visited = new bool[size];
	postorder = new int[size];
	
}

Graph::~Graph(){
	
	//w destruktorze usuwamy wszystkie tablice
	
	for (int i = 0; i < size ; i++)
		delete [] graph[i];
	
	delete [] graph;
	delete [] visited;
	delete [] postorder;
}

void Graph::wypisz(){
	
	cout<<" ";
	for(int i = 0; i < size; i++)
		cout<<" "<<i;
	cout<<endl;
	
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
	while(ifile >> a >> b)
		graph[a][b] = 1;
	
	ifile.close();
}

int Graph::znajdzSasiada(int w){
	
	int l = 0;
	
	//petla znajduje sasiada, czyli wierzcholek do ktorego moze isc przeszukiwanie
	//warunki to: komorka tablica nie moze miec 0 (tam gdzie jest krawedz jest 1) - czyli jak komorka ma 1ke
	//wierzcholek nie moze byc wczesniej odwiedzony
	//zawsze wybiera pierwzego spotkanego sasiada ktory spelnia dwa powyzsze warunki. Najmniejszego wzrostowo
	for(int i = 0; i < size; i++)
		if(graph[w][i] != 0 && !visited[i] && l < 1){
			l++;
			//przypisuje do parametru w nowego sasiada
			w = i;
		}
	
	//i zwraca go
	return w;
}

bool Graph::sprawdzCzyMaSasiadow(int w){
	
	int l = 0;
	for(int i = 0; i < size; i++)
		if(graph[w][i] != 0 && !visited[i])
			l++;
	
	//zlicza czy wierzcholek w ma sasiadow,czyli: czy jakies pozycje w tablicy z pierwzzym indeksem w maja drugi 
	//rozny 0 (rowny 1) i czy te wierzcholki sa nieodwiedzone
	
	//jesli licznik jest wiekszy od 0 (tu rozny, mogloby byc wiekszy) to znaczy ze sa takowe wierzcholki ktore
	//moze odwiedzic wierzcholek w.
	return (l != 0) ? true : false;
}

void Graph::transposeMatrix(){
	
	//najpierw znajduje czy sa jakies wierzcholki dwukierunkowe czyli: np. 7-8 i jednoczesnie 8-7
	//jesli takie sa to zmienia w dwie strony 1ki na 2ki. zrobilem to po to ze wystepowal potem konflikt
	//on zamienial jedna strone a druga wymazywal.
	
	for(int i = 0; i < size; i++)
		for(int j = 0; j < size; j++)
			if(graph[i][j] == 1 && graph[j][i] == 1){
				graph[i][j] = 2;
				graph[j][i] = 2;
			}
	
	//teraz szuka 1-nek. tam gdzie znajdzie daje -1. i odwrotnej stronie daje 2ke. bo transpozycja ma odwrocic
	//kierunki strzalek.
	for(int i = 0; i < size; i++)
		for(int j = 0; j < size; j++)
			if(graph[i][j] == 1){
				graph[i][j] = -1;
				graph[j][i] = 2;
			}
	
	// i na koniec wymazuje -1.
	for(int i = 0; i < size; i++)
		for(int j = 0; j < size; j++)
			if(graph[i][j] == -1){
				graph[i][j] = 0;
			}
	
}

void Graph::DFSPostOrder(int beg){
	
	for(int i = 0; i < size; i++) visited[i] = false;
	for(int i = 0; i < size; i++) postorder[i] = 0;
	
	int k = 0;

	Stack stack(size);
	
	//wsadz pierwszy wierzcholek na stos
	stack.push(beg);
	//oznacz go jakos odwiedzony
	visited[beg] = true;
	
	//dopoki stos nie jest pusty
	while(!stack.checkIfStackEmpty()){
		//wsadz na stos dostepnego sasiada wierzcholka ktory aktualnie jest na topie stosu
		stack.push(znajdzSasiada(stack.getStackTop()));
		//oznacz nowy top stosu jako odwiedzony
		visited[stack.getStackTop()] = true;
		//dopoki wierzcholek nie ma sasiadow. jesli wierzcholek ma sasiadow w ogole nie wchodzi do tej petli
		//jesli nie ma to oznacza ten wierzcholek jako odwiedzony (tu wlasnie powstaje POST-ORDER. Idzie dopoki
		//nie napotka wierzcholka z ktorego nigdzie nie mozna isc i wtedy dopiero ustala porzadek). Wrzuca ten
		//wierzcholek do tablicy post-order i zdejmuje wierzcholek ze stosu dopoki nie natraf na taki wierzcholek
		//ktory ma sasiadow jeszcze nie odwiedzonych. jesli w koncu natrafi na koniec stosu konczy dzialanie.
		
		while(!stack.checkIfStackEmpty() && !sprawdzCzyMaSasiadow(stack.getStackTop())){
			visited[stack.getStackTop()] = true;
			postorder[k] = stack.getStackTop();
			k++;
			stack.pop();
		}
	}
		
}

void Graph::KosarajuAlgorithm(){
	
	//ten algorytm ma wykonywac na transponowanym grafie DFS-y. po kolei od konca tablicy POST-ORDER
	transposeMatrix();
	
	//tablica pomocnicza to przechowywania ustalonego wczesniej przez algorytm DFS POST ORDER porzadku.
	int *x = new int[size];
	
	for(int i = 0; i < size; i++)
		x[i] = postorder[i];
	
	cout<<"Zbiory wierzcholkow indukujace silnie spojne skladowe digrafu: "<<endl;
	
	int i = 1;
	while(i <= size){
		//jesli element tablicy x nie byl wczesniej ustawiony na -1
		if(x[size-i] != -1){
			//wykonaj DFS zaczynajac od ostatniego elementu z porzadku
			DFSPostOrder(x[size-i]);
			
			cout<<"{ ";
			//ta petla zaznacza w tablicy pomocniczej x ktorym wierzcholkom juz zrobilismy DFS. Po wykonaniu DFS() w
			//tablicy visited mamy odwiedzone wierzcholki. Ustawia elementy tablicy x na -1
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
