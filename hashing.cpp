#include <iostream>
#include <fstream>
#include<sstream>
#include<time.h>
#include <iomanip>// used for set precision
#include <ctime>
using namespace std;
// lllist is the same header in lab 5 but I deleted the llstack and llqueue snd added 3 new functions find, remove and boomrang, boomrang inserts an element at the end whilst checking if 
//it is already in the llist by the remove function, remove removes a specific element in the llist and  if it isn't there it does nothing, find finds an element inside a llist by traversing and
// returns null if it isn't there. They are consectutively written begining at line 87 of lllist
 class Word {
public:
	string valu;
	Word() : valu(" ") {}
	Word (string va) {
		for (int u = 0; u < va.length(); u++) {
			va[u] = tolower(va[u]);
		}
		valu = va;
	}
	Word(const Word& e) : valu(e.valu) {}
	bool equals(Word* word) {
		return (word->valu == valu);
	}
	bool operator==(Word& y) {
		return this->equals(&y);
	}
	bool operator< (Word u) { // the operator < works in the sens that the word with the first string in the alphabetical order of two strings is superior to that string, for example b is smaller than a and ag is smaller than ae
		int i = 0;
		while (valu[i] == u.valu[i] && i<valu.length()) i++;
		if (i == valu.length()) {
			if (valu.length() < u.valu.length()) return false;
			else if (u.valu.length() < valu.length()) return true;
			else if (valu.length() == u.valu.length()) return true;
		}
		else return this->valu[i] > u.valu[i];
	}
	bool operator> (Word u) { // the oposie of the < operator, so a is bigger than z, and zb is bigger than zd
		int i = 0;
		while (valu[i] == u.valu[i] && i < valu.length()) i++;
		if (i == valu.length()) {
			if (valu.length() > u.valu.length()) return false;
			else if (u.valu.length() > valu.length()) return true;
			else if (valu.length() == u.valu.length()) return true;
		}
		else return this->valu[i] < u.valu[i];
	}
	unsigned long hashCode() {
		unsigned long  h = 0;
		for (int i = 0; i < valu.length(); i++) {
			h = 31 * h + valu[i];
		}
		return h;

	}
};
void Donothing() { // used in the remove function in lllist to do nothing because we couldn't return since remove is a void
	return;
}
template<class T>
class Heap { // I created this heap class in order to store the nearmisses in a maxheap and pop them in alphabetical order thanks to heapsort. Using the < and > operators for Word we say that the Words with their string with a smaller alphabetical letter is superior to others with bigger letters alphabetically so when we pop each Word we will get them in terms of the alphabetical order of their strings; so a is bigger than z and x etc. so a will be poped before z and x using heapsort
	int capacity;
	T* data = new T[1000];
public:
	Heap() : capacity(-1) {}
	Heap(int u) : capacity(u) {}
	Heap(Heap& y) : capacity(y.capacity), data(y.data) {}
	int sid() {
		return capacity+1;
	}
	void insert(T h) {
		capacity++;
		bool isThere = false;
		for (int i = 0; i < capacity; i++) if (data[i] == h) isThere = true;
		if (isThere == false) {
			data[capacity] = h;
			if (capacity != 0) {
				int current = capacity;
				int previous = (current - 1) / 2;
				while ((previous != 0 || current != 0) && data[previous] < data[current]) {
					T temp = data[previous];
					data[previous] = data[current];
					data[current] = temp;
					current = previous;
					previous = (current - 1) / 2;
				}
			}
		}
	}
	T removeroot() {
		T ret = data[0];
		data[0] = data[capacity];
		capacity--;
		int current = 0;
		int child=0;
		int lchild = 2 * current + 1;
		int rchild = 2 * current + 2;
		if (data[lchild]<data[rchild]) child =rchild;
		else child = lchild;
		while (lchild <= capacity && data[current] < data[child]) {
			T temp = data[current];
			data[current] = data[child];
			data[child] = temp;
			current = child;
			lchild = 2 * current + 1;
			rchild = 2 * current + 2;
			if (data[lchild] < data[rchild]) child = rchild;
			else child = lchild;
		}
		return ret;

	}
};
#include"lllist.h"
class HashTable {
	int capacity;
	LList<Word>* keys = new LList<Word>[capacity];
	LList<Word>* word = new LList<Word>[capacity];
	int siz = 0;
public:
	HashTable() : capacity(15000) {}// differe between 1000 5000 10000 15000 20000 25000
	HashTable(int size) : capacity(size) {}
	Word* push(Word k, Word w) {
			unsigned long i = k.hashCode();
			unsigned long r = i % capacity;
			int a = keys[r].size;
			keys[r].remove(k);
			keys[r].boomrang(k);
            siz++;
			return &word[r].boomrang(w);
     }
	bool contains(Word fg) {
		int w = fg.hashCode() % capacity;
		return (word[w].find(fg) != NULL);
	}
	void remove(Word e) {
		int r = e.hashCode()%capacity;
		keys[r].remove(e);
		word[r].remove(e);
		siz--;
	}
	void search(Word e) {
		unsigned long g = e.hashCode();
		LList<Word>::LLNodeP i=keys[g%capacity].find(e);
		if (i != NULL) {
				cout<< "ok" << endl;  //removed it for the time complexity measurements
		}
		else {
			Word Nearmisses[10000];
			int index = 0;
				cout << "not found "; //removed it for the time complexity measurements
			for (int i = 0; i < e.valu.length(); i++) {
				Word f;
				f.valu.resize(e.valu.length() - 1);
				for (int j = 0; j < e.valu.length() - 1; j++) {
					if (j < i) f.valu[j] = e.valu[j];
					else if (j >= i) f.valu[j] = e.valu[j + 1];
				}
				Nearmisses[index]=f;
				index++;
			}
			for (char c = 'a'; c <= 'z'; c++) {
				for (int h = 0; h < e.valu.length() + 1; h++) {
					Word d;
					d.valu.resize(e.valu.length() + 1);
					for (int u = 0; u < e.valu.length() + 1; u++) {
						if (u < h) d.valu[u] = e.valu[u];
						else if (h == u) d.valu[u] = c;
						else if (u > h) d.valu[u] = e.valu[u - 1];
					}
					Nearmisses[index]=d;
					index++;
				}
			}
			for (int y = 0; y < e.valu.length(); y++) {
				Word ab;
				ab.valu.resize(e.valu.length());
				ab.valu = e.valu;
				char temp = ab.valu[y];
				ab.valu[y] = ab.valu[y + 1];
				ab.valu[y + 1] = temp;
				Nearmisses[index]=ab;
				index++;
			}
			for (int t = 0; t < e.valu.length(); t++) {
				for (char c = 'a'; c <= 'z'; c++) {
					Word we;
					we.valu.resize(e.valu.length());
					we.valu = e.valu;
					we.valu[t] = c;
					Nearmisses[index]=we;
					index++;
				}
			}
			for (int mn = 1; mn < e.valu.length(); mn++) {
						Word d1;
						Word d2;
						d1.valu.resize(mn);
						for (int u = 0; u < mn; u++) d1.valu[u] = e.valu[u];
						Nearmisses[index] = d1;
						index++;
						d2.valu.resize(e.valu.length() - mn);
						for (int vc = 0; vc < e.valu.length()-mn; vc++) d2.valu[vc] = e.valu[vc+mn];
						Nearmisses[index] = d2;
						index++;
				}
				int counter = 0;
				int cut = 0;
				Heap<Word> filo;
				for (int hy = 0 ; hy <index;hy++) {
					Word eu(Nearmisses[hy]);
					cut++;
					unsigned long f = eu.hashCode();
					LList<Word>::LLNodeP i = keys[f % capacity].find(eu);
					if (i != NULL) {
						filo.insert(eu);
						counter++;
					}
				} 
				//this for loop was removed for the time complexity measurements
				for (int t = 0; t < counter; t++) {
						if (t == 0) cout << "Near misses:";
						cout << filo.removeroot().valu << ", ";
					}
				cout << '\n';
			}
	}
	int size() {
		return siz;
	}
};
string randomgen(string gtr, int foo) { //used to generate english_modified.txt (the second half of it) into words that have near misses in english.txt
	string d;
	if (foo == 1) {
		char c = rand() % 26 + 97;
		int h = rand() % (gtr.length()+1);
		d.resize(gtr.length() + 1);
		for (int u = 0; u < gtr.length() + 1; u++) {
			if (u < h) d[u] = gtr[u];
			else if (h == u) d[u] = c;
			else if (u > h) d[u] = gtr[u - 1];
				
		}
	}
	else if (foo == 2) {
		int uyt = rand() % gtr.length();
		d.resize(gtr.length() - 1);
		for (int j = 0; j < gtr.length() - 1; j++) {
			if (j < uyt) d[j] = gtr[j];
			else if (j >= uyt) d[j] = gtr[j + 1];
		}
	}
	else if (foo == 3) {
		int qy = rand() % (gtr.length()-1);
		d.resize(gtr.length());
		d = gtr;
		char temp = d[qy];
		d[qy] = d[qy + 1];
		d[qy + 1] = temp;
           
	}
	else if (foo == 4) {
		int gf = rand() % gtr.length();
		char fd = rand() % 26 + 97;
		d.resize(gtr.length());
	    d = gtr;
		d[gf] = fd;
	}
	else if (foo == 5) {
		int concat = rand() % 5+1;
		d.resize(gtr.length()+concat);
		for (int tq = 0; tq < gtr.length(); tq++) d[tq] = gtr[tq];
		for (int rew = 0; rew < concat; rew++) {
			char gh = rand() % 26 + 97;
			d[rew+gtr.length()] = gh;
		}
	}
	return d;
}
int main() {
	// comments below were used to generate english_modified.txt
	//srand(time(0));
	string line;
	ifstream myfile("English.txt");
	//ofstream yourfile("English_modified.txt");
	HashTable hash;
	if (myfile.is_open())
	{
		//if (yourfile.is_open()) {
			int i = 0;
			while (getline(myfile, line)) {
				if (i == 0) {
					i++;
				}
				else {
					//if (i < 10433) {
						if (line != " ") {
							//yourfile << line << endl;
							Word q(line);
							hash.push(q, q);
							//i++;
						}
					}

					/*else {
						if (line != " ") {
							int decision = rand() % 5 + 1;
							string gw = randomgen(line, decision);
							yourfile << gw << endl;
						}
					}*/
				//}
			}
		//}
		//yourfile.close();
	}
  myfile.close();
  // random hard tests;
  hash.search(Word("hi"));
  hash.search(Word("pi"));
  hash.search(Word("abigil"));
  hash.search(Word("zoologi"));
  hash.search(Word("hello"));
  hash.search(Word("Raymond"));
  hash.search(Word("computer"));
  // time complexity using english_modified.txt as input
	    std::clock_t c_start = std::clock();
		ifstream yourfile("English_modified.txt");
		string mytext;
		if (yourfile.is_open())
		{
			while (getline(yourfile, mytext)) {
				if (mytext != " ") {
					Word q(mytext);
					hash.search(q);
				}
			}
		}
		std::clock_t c_end = std::clock();
		double elapsed = (c_end - c_start) /
			CLOCKS_PER_SEC;
		std::cout << std::fixed << std::setprecision(2)
			<< " CPU time used : " << elapsed << " s"
			<< std::endl;
		return 0;
		// for 1000 capacity hashtable the time needed is 183 s
		// for 5000 it is 177 s
		// for 10 000 capacity hashtable the time needed is 194s
		// for 15 000 it is 191s
		// for 20 000 it is 195s
		// for 25 000 it is 197 s
}