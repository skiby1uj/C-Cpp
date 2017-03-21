#include <iostream>
#include <list>
#include <map>

using namespace std;

int n;

template<class type>
class PriorityQueue{
	public:
		int n;//max ilosc elementow w kolejce
		int endQueue;
		type *chars;//znaki
		int *tab;//ich wagi
		PriorityQueue(int n){
			tab = new int[n];
			chars = new type[n];
			endQueue = 0;
		}
		void push(int weight, char znak){
			int i = endQueue;
			endQueue++;
			int j = (i-1) / 2;
			while(i > 0 && tab[j] > weight){
				tab[i] = tab[j];
				chars[i] = chars[j];
				i = j;
				j = (i-1) / 2;
			}
			tab[i] = weight;
			chars[i] = znak;
		}
		type pop(){
			type v;
			char min, w;
			int i = 0;
			int j = 1;
			if(endQueue > 0){
				min = chars[0];
				endQueue--;
				v = tab[endQueue];
				w = chars[endQueue];
				while(j < endQueue){
					if(j + 1 < endQueue && tab[j + 1] < tab[j]){
						j++;
					}
					if(v <= tab[j]){
						break;
					}
					tab[i] = tab[j];
					chars[i] = chars[j];
					i = j;
					j = 2*j + i;
				}
				tab[i] = v;
				chars[i] = w;
				return min;
			}
			
		}
		bool empty(){
			if(endQueue <= 0){
				return true;
			}
			else{
				return false;
			}		
		}
};

struct vertex{
	char parent;
	map<char, int> listaSasiedztwa;//key - sšsiad, value - waga drogi
	int weight;//`
	bool odwiedzony;
	vertex(){
		weight = 100000;
		odwiedzony = false;
	}
};

void createListaSasiedztwa(map<char, vertex> &mapaWierzcholkow){
	int m;
	int weight;
	char tmp;
	char tmp2;
	cin>>m;
	while(m > 0){
		m--;
		cin>>tmp>>tmp2>>weight;
		mapaWierzcholkow[tmp].listaSasiedztwa[tmp2] = weight;
	}
}

void createGraf(map<char, vertex> &mapaWierzcholkow, list<char> &output){
	char tmp;
	int t;
	cin>>t;
	n= t;
	while(t > 0){
		t--;
		cin>>tmp;
		vertex ver;
		mapaWierzcholkow[tmp] = ver;
		output.push_front(tmp);
	}
	createListaSasiedztwa(mapaWierzcholkow);
}

bool IsInMap(char wierzcholek, map<char, vertex> &mapa){
	map<char, vertex>::iterator ite = mapa.begin();
	for(; ite != mapa.end(); ite++){
		if(ite->first == wierzcholek)
			return true;
	}
	return false;
}

void Dijkstra(map<char, vertex> &mapaWierzcholkow, char start){
	mapaWierzcholkow[start].weight = 0;
	PriorityQueue<char> queue(n);
	queue.push(mapaWierzcholkow[start].weight, start);
	while(queue.empty() == false){
		char min = queue.pop();
		map<char, int>::iterator ite = mapaWierzcholkow[min].listaSasiedztwa.begin();
		for(; ite != mapaWierzcholkow[min].listaSasiedztwa.end(); ite++){
			if(mapaWierzcholkow[ite->first].odwiedzony == false){
				queue.push(mapaWierzcholkow[ite->first].weight ,ite->first);
			}
			if(mapaWierzcholkow[ite->first].weight > (mapaWierzcholkow[min].weight + ite->second)){
								
				mapaWierzcholkow[ite->first].weight = (mapaWierzcholkow[min].weight + ite->second);
				mapaWierzcholkow[ite->first].parent = min;
			}
		}
		mapaWierzcholkow[min].odwiedzony = true;
	}
}

bool BellmanFord(map<char, vertex> &mapaWierzcholkow, char start){
	Dijkstra(mapaWierzcholkow, start);
	map<char, vertex>::iterator ite = mapaWierzcholkow.begin();
	for(; ite != mapaWierzcholkow.end(); ite++){
		map<char, int>::iterator iteSasiad = mapaWierzcholkow[ite->first].listaSasiedztwa.begin();
		for(; iteSasiad != mapaWierzcholkow[ite->first].listaSasiedztwa.end(); iteSasiad++){
			if(mapaWierzcholkow[iteSasiad->first].weight > (mapaWierzcholkow[ite->first].weight + mapaWierzcholkow[ite->first].listaSasiedztwa[iteSasiad->first]))
				return false;
		}
	}
	return true;
}

main(int argc, char *argv[]){
	string znak = argv[1];
    string szukana = argv[2];
	map<char, vertex> mapaWierzcholkow;
	list<char> out;
	createGraf(mapaWierzcholkow, out);
	if(znak[1] == 'd')
		Dijkstra(mapaWierzcholkow, szukana[0]);
	else{
		if(!BellmanFord(mapaWierzcholkow, szukana[0]))
			cout<<"!"<<endl;
	}
	out.sort();
	list<char>::iterator ite = out.begin();
	for(; ite != out.end(); ite++){
		cout<<*ite<<" "<<mapaWierzcholkow[*ite].weight<<endl;
	}
}
