#pragma once
template < class T >
class LList {
public:
	// declare LLNode as a nested class
	class LLNode {
	private:
		T _d; // data
		LLNode* _next;// pointer to next
		// LLNode * _prev ;// pointer to prev
	protected:
		// constructor
		LLNode(const T& d, LLNode* next = NULL) : _d(d), _next(next) { }
		T& data() { return _d; }
		LLNode*& next() { return _next; }
		void printRec(ostream& os) {
			if (this == NULL) {
				return;
			}
			os << _d << " " << _next << ", ";
			_next->printRec(os);
		}
		~LLNode() {
			if (_next) {
				// how is it recursive ?
				delete _next;
			}
		}
		// optional for doubly linked :
		// LLNode *& prev () { return _prev ;}
		friend class LList;
	};
	typedef LLNode* LLNodeP;
	LLNodeP head, tail;
	unsigned int size;
	// constructor
	LList() : head(0), tail(0), size(0) {}
	// destructor
	~LList() {
		if (head) {
			delete head;
		}
		head = tail = NULL;
		size = 0;
	}
	
	// insert at head
	LLNodeP insert(const T& d) {
		//n has _d=d, _next = head
		LLNodeP n = new LLNode(d, head);
		if (head == NULL) { tail = n; }
		head = n; size = size + 1;
		return n;
	}
	//  after pointer
	LLNodeP insertafter(const T& d, LLNodeP after) {
		if (head == NULL || after == NULL) { return insert(d); }
		LLNodeP n = new LLNode(d, after->next());
		after->next() = n;
		if (tail == after) tail = after->next();
		size++;
		return after->next();
	}
	// find element with data;
	LList& operator << (const T& d) {
		insert(d);
		return *this;
	}
	void print(ostream& os) {
		LLNodeP n = head;
		while (n != NULL) {
			os << n->data() << ",";
			n = n->next();
		}
	}
	void printRec(ostream& os) {
		head->printRec(os);
	}
	void append(LList& ol) {
		tail->_next = ol.head;
		tail = ol.tail;
		size += ol.size;
		ol.tail = NULL;
		ol.head = NULL;
		ol.size = 0;
	}
	void remove(T& e) {
		if (head != NULL){
			if (head == tail && head->data() == e) removeHead();
			else if (head == tail) Donothing();
		else {
			LLNodeP node = head;
			LLNodeP previous = NULL;
			while (node->next() != NULL) {
				if (node->next()->data() == e) {
					LLNodeP t = node->next();
					node->next() = t->next();
					t = NULL;
					size--;
					break;
				}
				previous = node;
				node = node->next();
			}
			if (node == tail) {
				if (node->data() == e) {
					LLNodeP k = tail;
					tail = previous;
					k = NULL;
				}
			}
		}
		}
	}
	LLNodeP find(T& d) {
		LLNodeP node = head;
		while (node != NULL) {
			if (node->data() == d) return node;
			node = node->next();
		}
		return NULL;
	}
	T boomrang(T& d) {
			tail = insertafter(d, tail);
			return tail->data();
	}
	LLNodeP findplace(const T& t, LLNodeP he) {
		if (he->_d <= t && he->_next->_d >= t) {
			LLNodeP o = new LLNode(t, he->_next);
			he->_next = o;
			size += 1;
			return o;
		}
		else return findplace(t, he->_next);
	}
	LLNodeP insertplace(const T& d) {
		if (head == NULL || d <= head->_d) return insert(d);
		else if (head == tail) {
			tail = insertafter(d, head);
			return tail;
		}
		else if (d >= tail->_d) {
			tail = insertafter(d, tail);
			return tail;
		}
		return findplace(d, head);
	}
	T& removeHead() {
		if (size == 1) {
			T& q = head->_d;
			head = NULL;
			tail = NULL;
			return q;
		}
		T& g = head->_d;
		head = head->_next;
		size--;
		return g;
	}
	T& dat() {
		return head->data();
	}
	void reverse()
	{
		// Initialize current, previous and
		// next pointers
		LLNodeP node = head;
		LLNodeP prev = NULL, next = NULL;

		while (node != NULL) {
			// Store next
			next = node->_next;

			// Reverse current node's pointer
			node->_next = prev;

			// Move pointers one position ahead.
			prev = node;
			node = next;
		}
		head = prev;
	}
	void merge(LList& ol) {
		LLNodeP node = head;
		LLNodeP dfe = ol.head;
		while (dfe->data() <= head->data()) {
			insert(dfe->data());
			dfe = dfe->next();
		}
		while (node != NULL && dfe != NULL) {
			if (node != tail) {
				while (node->next()->data() >= dfe->data() && node != tail && dfe != ol.tail) {
					LLNodeP u = insertafter(dfe->data(), node);
					node = u;
					dfe = dfe->next();
				}
				node = node->next();
			}
			else
				while (dfe != NULL) {
					LLNodeP g = insertafter(dfe->data(), tail);
					tail = g;
					dfe = dfe->next();
				}
		}
		ol.head = NULL;
		ol.tail = NULL;
		ol.size = 0;
	}
};