#include "stack.hpp"
#include "vector.hpp"
#include "map.hpp"
using namespace ft;
// #include <stack>
// #include <vector>
// #include <map>
// using namespace std;
#include <list>
#include <deque>
#include <iostream>
#include <ctime>

template <class T>
void	printVec(vector<T> v){
	std::string r = "";
	for (typename vector<T>::size_type i = 0; i < v.size(); i++){
		r += (i == 0 ? "" : " ");
		r += v[i];
	}
	std::cout << r << "; s" << v.size() << "; c" << v.capacity() << std::endl;
}

template <class K, class V>
void	printMap(map<K, V> m){
	std::string r = "";
	for (typename map<K,V>::iterator it = m.begin(); it != m.end(); it++){
		r += std::to_string((*it).first);
		r += ":";
		r += std::to_string((*it).second);
		r += " ";
	}
	std::cout << r << std::endl;
}

void	vector_basiques(){
	#define VECTYPE char
	//CONSTRUCTORS
	vector<VECTYPE> v1;
	vector<VECTYPE> v2(4, 97);
	v1 = v2;
	vector<VECTYPE> v3(v1.begin(),v1.end());
	vector<VECTYPE> v4(v3);
	printVec(v4);
	//ELEMENT ACCESS
	v4[1] = 65 + 1;
	v4.at(2) = 65 + 2;
	v4.front() = 65 + 0;
	v4.back() = 65 + 3;
	//ITERATORS
	printVec(v4);
	for (vector<VECTYPE>::iterator it = v4.begin(); it != v4.end(); it++)
		v4[v4.begin()-it] = 65 - (v4.begin()-it);
	printVec(v4);
	for (vector<VECTYPE>::reverse_iterator it = v4.rbegin(); it != v4.rend(); it++)
		*it += 2;
	//CAPACITY
	printVec(v4);
	std::cout << v4.max_size() << std::endl;
	v4.resize(6, 70);
	std::cout << v4.capacity() << std::endl;
	std::cout << v4.empty() << std::endl;
	v4.reserve(9);
	std::cout << v4.capacity() << std::endl;
	v4.push_back(72);
	v4.push_back(72);
	v4.push_back(72);
	v4.push_back(72);
	std::cout << v4.capacity() << std::endl;
	//MODIFIERS
	v4.clear();
	v3.clear();
	std::cout << v3.empty() << std::endl;
	for (vector<VECTYPE>::size_type i = 0; i < 7; i++){
		v4.insert(v4.begin() + i, 'a' + i);
		v3.insert(v3.begin() + i, 'A' + i);
	}
	v3.insert(v3.begin()+3, 2, 'X');
	v3.insert(v3.begin()+4, v4.begin(), v4.end());
	printVec(v3);
	v3.swap(v4);
	printVec(v3);
	printVec(v4);
	std::cout << *(v4.erase(v4.begin() + 4)) << std::endl;
	std::cout << *(v4.erase(v4.end() - 4, v4.end() - 2)) << std::endl;
	printVec(v4);
	//SURCHARGE
	std::cout << (v3 == v4) << (v3 != v4) << (v3 <= v4) << (v3 < v4) << (v3 >= v4) << (v3 > v4) << std::endl;
}

void	vector_en_details(){
	#define ATYPE char
	vector<ATYPE> a;
	for (int i = 0 ; i < 6; i ++)
		a.push_back(i + 'a');
	vector<ATYPE>::iterator it = a.begin() + 1;
	vector<ATYPE>::const_iterator cit = a.begin() + 2;
	std::cout << (it != cit) << std::endl;
	std::cout << (it > cit) << std::endl;
	vector<ATYPE> b;
	for (int i = 0 ; i < 9; i ++)
		b.push_back(i + 'A');
	
	vector<ATYPE>::reference r = a.at(2);
	a.swap(b);
	it++;
	*it = 'Z';
	r = 'X';
	printVec(a);
	printVec(b);
}

void	vector_performances(){
	float t;
	clock_t t1, t2;
	t1 = clock();

	vector<char> v(12000000, 'a');
	vector<char> w(v);
	printVec(v);

	t2 = clock();
	t = (float)(t2 - t1)/CLOCKS_PER_SEC;
	std::cout << "T:" << t << std::endl;
}

void	map_performances(){
	float t;
	clock_t t1, t2;
	int r;
	map<int, int> m;
	t1 = clock();

	for (int i = 0; i < 100000; i++){
		m[i] = i;
	}
	printMap(m);

	t2 = clock();
	t = (float)(t2 - t1)/CLOCKS_PER_SEC;
	std::cout << "T:" << t << std::endl;
}

void	map_basiques(){
	#define K int
	#define M int
//CONSTRUCTORS
	map<K,M> m;
	m[-34] = 1;
	m[0345] = 12;
	m[654] = -24;
	m[234] = 53;
	map<K,M> n(m);
	map<K,M> o = n;
//ITERATORS
	for (map<K,M>::iterator it = m.begin(); it != m.end(); it++)
		it->second = 0;
	for (map<K,M>::const_iterator it = m.begin(); it != m.end(); it++)
		std::cout << it->first << ":" << it->second << " ";
	std::cout << std::endl;
	int i = 9;
	for (map<K,M>::reverse_iterator it = m.rbegin(); it != m.rend(); it++)
		it->second = i++;
	for (map<K,M>::const_reverse_iterator it = m.rbegin(); it != m.rend(); it++)
		std::cout << it->first << ":" << it->second << " ";
	std::cout << std::endl;
//CAPACITY
	std::cout << m.empty() << m.size() << m.max_size() << std::endl;
//ELEMENT_ACCESS
	printMap(m);
	std::cout << m[14];
	printMap(m);
	std::cout << m[229] << std::endl;
//MODIFIERS
	m.insert(make_pair(35,54));
	printMap(m);
	map<K,M>::iterator hh = m.begin();
	hh++;
	m.insert(hh, make_pair(-200,-200));
	m.insert(n.begin(), n.end());
	for (int i = 0; i < 4; i++)
		n.insert(make_pair(i, i));
	m.insert(n.begin(), n.end());
	printMap(m);
	m.erase(m.begin());
	m.erase(1);
	printMap(m);
	m.clear();
	for (int i = 0; i < 12; i++){
		m.insert(make_pair(i*5,i*5));
		n.insert(make_pair(i,i));
	}
	m.swap(n);
	printMap(m);
	printMap(n);
//OPERATIONS
	std::cout << m.find(5)->second << m.count(5) << m.count(43) << std::endl;
	std::cout << m.lower_bound(30)->second << m.upper_bound(30)->second << std::endl;
}

void	map_en_details(){
	#define AK int
	#define AM char
	map<AK,AM>m;
	for (int i = 0; i < 7; i++){
		m[i*5] = 'a' + i;
	}
	m.insert(make_pair(15, 'z'));
	m.insert(make_pair(25, 'y'));
	m[10] = 'G';
	m.insert(make_pair(-10, 'z'));
	printMap(m);
	map<AK,AM>::reverse_iterator it = m.rend();
	it--;
	it--;
	std::cout << it->first << std::endl;
	m.insert(make_pair(3, '$'));
	printMap(m);
	std::cout << it->first << std::endl;
	m.erase(3);
	printMap(m);
	std::cout << it->first << std::endl;
	map<AK,AM> n;
	for (int i = 0; i < 4; i++){
		n[i*3] = 'A' + i;
	}
	map<AK,AM>::iterator it2 = n.begin();
	n.swap(m);
	std::cout << it2->first << std::endl;
	printMap(m);
	printMap(n);
}

void	stack_basiques(){
	//member_functions
	stack<char> s1;
	std::cout << s1.size() << std::endl;
	stack<char> d;
	d.push('d');
	d.push('q');
	stack<char> s2 = d;
	s2.push('!');
	for (;!s2.empty(); s2.pop())
		std::cout << s2.top() << std::endl;
	//non_member functions oerloads
	s1.push(';');
	std::cout << (s1 == s2) << std::endl;
	std::cout << (s1 != s2) << std::endl;
	std::cout << (s1 < s2) << std::endl;
	std::cout << (s1 <= s2) << std::endl;
	std::cout << (s1 > s2) << std::endl;
	std::cout << (s1 >= s2) << std::endl;
}

void	stack_en_details(){
	#define S int
	stack<S, std::deque<S> > a;
	stack<S, std::list<S> > b;
	for (int i = 0; i < 5; i++){
		a.push(i*1);
		b.push(i*2);
	}
	for (int i = 0; i < 5; i++){
		std::cout << a.top();
		std::cout << b.top();
		std::cout << std::endl;
		a.pop();
		b.pop();
	}
}

int		main()
{
	vector_basiques();
	vector_en_details();
	// vector_performances();
	map_basiques();
	map_en_details();
	// map_performances();
	stack_basiques();
	stack_en_details();
	return 1;
}