#pragma once
#include<iostream>
#include "KeyComp.h"
#include<string>
using namespace std;

template<class K, class V, class F> class Dictionary;
template<class K, class V, class F> class RBT;

template<class K, class V>
class Nod
{
	K key;
	V value;
	Nod* left;
	Nod* right;
	Nod* parent;
	string color;


	template<class K, class V, class F> friend class Dictionary;
	template<class K, class V, class F> friend class RBT;
public:
	Nod();
	Nod(K, V);


};

template<class K, class V>
inline Nod<K, V>::Nod()
{
	key = 0;
	value = 0;
	color = "";
	left = NULL;
	right = NULL;
	parent = NULL;
}

template<class K, class V>
inline Nod<K, V>::Nod(K k, V v)
{
	this->key = k;
	this->value = v;
	this->color = "red";
	this->parent = NULL;
	this->left = NULL;
	this->right = NULL;
}
