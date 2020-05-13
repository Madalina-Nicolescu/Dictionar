#include <iostream>
#include "KeyComp.h"
#include "Nod.h"
#include<vector>
#include<string>


using namespace std;


template<class K, class V, class F = KeyComp<K>> class Dictionary;
template<class K, class V, class F = KeyComp<K>> ostream& operator <<(ostream&, Dictionary<K, V, F>&);


template<class K, class V, class F = KeyComp<K>> class Dictionary
{
    Nod<K, V>* root;
    int height;
    F cmp;


    //HELPERS

    void left_rotate(Nod<K, V>*);
    void right_rotate(Nod<K, V>*);
    Nod<K, V>* search(K);
    void fixViolation(Nod<K, V>*);
    Nod<K, V>* copyHelper(const Nod<K, V>*);
    void inorder(Nod<K, V>*);
    void preorder(Nod<K, V>*);
    Nod<K, V>* getRoot();
    void deleteNod(Nod<K, V>*);
    Nod<K, V>* replace(Nod<K, V>*);
    void fixDelete(Nod<K, V>*);
    Nod<K, V>* succesor(Nod<K, V>*);
    void clearHelper(Nod<K, V>*);

public:


    Dictionary();
    Dictionary(const Dictionary& d1);

    Dictionary& operator = (const Dictionary&);

    void addPair(K, V);
    void removeKey(K);
    bool findKey(K);
    void empty();


    V operator [](K);
    friend ostream& operator << <> (ostream&, Dictionary<K, V, F>&);


};




//-----CONSTRUCTORS-----

template<class K, class V, class F>
Dictionary<K, V, F>::Dictionary()
{
    root = NULL;
    height = 0;
}



template<class K, class V, class F>
Dictionary<K, V, F>::Dictionary(const Dictionary& d1) //copy constructor
{
    this->height = d1.height;
    this->root = copyHelper(d1.root); //functie recursiva pentru copierea arborelui
}




//-----HELPERS-----



template<class K, class V, class F>
Nod<K, V>* Dictionary<K, V, F>::copyHelper(const Nod<K, V>* r)
{
    if (r != NULL)
    {
        Nod<K, V>* copyNod = new Nod<K, V>;

        copyNod->key = r->key;
        copyNod->value = r->value;
        copyNod->parent = r->parent;
        copyNod->color = r->color;
        copyNod->left = copyHelper(r->left);
        copyNod->right = copyHelper(r->right);

        return copyNod;
    }
    return NULL;
}


template<class K, class V, class F>
void Dictionary<K, V, F>::left_rotate(Nod<K, V>* x)
{
    cout << "left";
    Nod<K, V>* p = x->right->left;
    Nod<K, V>* newPar = x->right;
    if (x == this->root)
        this->root = newPar;

    if (x->parent != NULL && x->parent->left == x)
    {
        x->parent->left = newPar;
    }
    else if (x->parent != NULL && x->parent->right == x)
    {
        x->parent->right = newPar;
    }

    newPar->parent = x->parent;
    x->parent = newPar;

    newPar->left = x;
    x->right = p;
    if (p != NULL)
        p->parent = x;
}

template<class K, class V, class F>
void Dictionary<K, V, F>::right_rotate(Nod<K, V>* x)
{
    cout << "right";
    Nod<K, V>* p = x->left->right;
    Nod<K, V>* newPar = x->left;
    if (x == this->root)
        this->root = newPar;

    if (x->parent != NULL && x->parent->left == x)
    {
        x->parent->left = newPar;
    }
    else if (x->parent != NULL && x->parent->right == x)
    {
        x->parent->right = newPar;
    }

    newPar->parent = x->parent;
    x->parent = newPar;

    newPar->right = x;
    x->left = p;
    if (p != NULL)
        p->parent = x;
}

template<class K, class V, class F>
Nod<K, V>* Dictionary<K, V, F>::search(K key)
{
    Nod<K, V>* p = root;
    while (p != NULL)
    {
        if (cmp(key, p->key) == -1)
        {
            if (p->left == NULL)
                break;
            else
                p = p->left;
        }
        else if (cmp(key, p->key) == 0)
            break;
        else
        {
            if (p->right == NULL)
                break;
            else
                p = p->right;
        }
    }
    return p;
}

template<class K, class V, class F>
void Dictionary<K, V, F>::fixViolation(Nod<K, V>* x)
{

    if (x == root)
    {
        x->color = "black";
        return;
    }


    Nod<K, V>* grandparent = x->parent->parent;

    Nod<K, V>* uncle = NULL;
    if (x->parent->parent == NULL)
    {
        uncle = NULL;
    }
    else
    {
        if (grandparent->left == x->parent)
        {
            uncle = grandparent->right;
        }
        else if (grandparent->right == x->parent) {
            uncle = grandparent->left;
        }
    }

    if (x->parent->color != "black")
    {
        if (uncle != NULL && uncle->color == "red")
        {
            uncle->color = "black";
            x->parent->color = "black";
            grandparent->color = "red";
            fixViolation(grandparent);
        }
        else
        {
            if (grandparent->left == x->parent)
            {
                if (x->parent->left == x)
                {

                    right_rotate(grandparent);
                    string s = grandparent->color;
                    grandparent->color = x->parent->color;
                    x->parent->color = s;
                }
                else
                {

                    left_rotate(x->parent);
                    string s = grandparent->color;
                    grandparent->color = x->parent->color;
                    x->parent->color = s;
                    right_rotate(grandparent);

                }
            }
            else
            {
                if (x->parent->left == x)
                {
                    right_rotate(x->parent);
                    left_rotate(grandparent);
                    string s = grandparent->color;
                    grandparent->color = x->parent->color;
                    x->parent->color = s;
                }
                else
                {
                    left_rotate(grandparent);
                    string s = grandparent->color;
                    grandparent->color = x->parent->color;
                    x->parent->color = s;
                }

            }
        }
    }
}



template<class K, class V, class F>
void Dictionary<K, V, F>::inorder(Nod<K, V>* r)
{
    if (r == NULL)
        return;
    inorder(r->left);
    cout << r->key << " " << r->value << endl;
    inorder(r->right);
}

template<class K, class V, class F>
void Dictionary<K, V, F>::preorder(Nod<K, V>* r)
{
    if (r == NULL)
        return;
    cout << r->key << " " << r->value << endl;
    inorder(r->left);

    inorder(r->right);
}

template<class K, class V, class F>
Nod<K, V>* Dictionary<K, V, F>::getRoot()
{
    return this->root;
}

template<class K, class V, class F>
void Dictionary<K, V, F>::deleteNod(Nod<K, V>* x)
{
    Nod<K, V>* y = replace(x); //nodul care o sa il inlocuiasca pe x;

    if (y == NULL)//daca x e frunza
    {
        if (x == root)
        {
            root = NULL;
        }
        else
        {
            if (x->color == "black")
                fixDelete(x);
            else
            {
                if (x->parent->right == x) //daca x e in dreapta
                {
                    x->parent->left->color = "red";
                }
                else //x e in stanga
                {
                    x->parent->right->color = "red";
                }
            }

            //stergem x
            if (x->parent->left == x)
                x->parent->left = NULL;
            if (x->parent->right == x)
            {
                x->parent->right = NULL;
            }
        }
        delete x;
        return;
    }


    if (x->left == NULL || x->right == NULL) //daca are doar un copil
    {
        if (x == root)
        {
            x->key = y->key;
            x->value = y->value;
            x->right = NULL;
            x->left = NULL;
            delete y;
        }
        else
        {
            if (x->parent->left == x)
            {
                x->parent->left = y;
            }
            else
            {
                x->parent->right = y;
            }
            y->parent = x->parent;
            string col = x->color;
            delete x;
            if (y->color == "black" && col == "black")
            {
                fixDelete(y);
            }
            else
            {
                y->color = "black";
            }



        }
        return;
    }

    //daca x are ambii fii, valoarea si cheia lui y se muta in x, iar procedeul de stergere e aplicat pentru y

    x->key = y->key;
    x->value = y->value;
    deleteNod(y);

}

template<class K, class V, class F>
Nod<K, V>* Dictionary<K, V, F>::replace(Nod<K, V>* x) //gasesc nodul care o sa il inlocuiasca pe x
{
    if (x->right == NULL && x->left == NULL)
    {
        //daca x e frunza
        return NULL;
    }
    if (x->right != NULL && x->left != NULL) //daca are ambii fii
    {
        return succesor(x->right);
    }
    if (x->right != NULL)
    {
        return x->right;
    }
    return x->left;
}

template<class K, class V, class F>
void Dictionary<K, V, F>::fixDelete(Nod<K, V>* x)
{
    if (x != NULL)
    {
        if (x == root)
            return;
        Nod<K, V>* s = NULL;
        if (x->parent != NULL && x->parent->right == x)
        {
            s = x->parent->left;
        }
        if (x->parent != NULL && x->parent->left == x)
        {
            s = x->parent->right;
        }
        if (s == NULL)
        {
            fixDelete(x->parent);
        }
        else
        {
            if (s->color == "red")
            {
                x->parent->color = "red";
                s->color = "black";
                if (s->parent->left == s)
                    right_rotate(x->parent);
                else left_rotate(x->parent);
                fixDelete(x);
            }
            else
            {
                if ((s->left != NULL && s->left->color == "red") || (s->right != NULL && s->right->color == "red"))
                {
                    if (s->left != NULL && s->left->color == "red")
                    {
                        if (s->parent->left == s)
                        {
                            s->left->color = s->color;
                            s->color = x->parent->color;
                            right_rotate(x->parent);
                        }
                        else
                        {
                            s->left->color = x->parent->color;
                            right_rotate(s);
                            left_rotate(x->parent);
                        }
                    }
                    else
                    {
                        if (s->parent->left == s)
                        {
                            s->right->color = x->parent->color;
                            left_rotate(s);
                            right_rotate(x->parent);
                        }
                        else
                        {
                            s->right->color = s->color;
                            s->color = x->parent->color;
                            left_rotate(x->parent);
                        }
                    }
                    x->parent->color = "black";
                }
                else
                {
                    s->color = "red";
                    if (x->parent->color == "black")
                        fixDelete(x->parent);
                    else x->parent->color = "black";
                }

            }
        }
    }
}

template<class K, class V, class F>
Nod<K, V>* Dictionary<K, V, F>::succesor(Nod<K, V>* x)
{
    Nod<K, V>* p = x;
    while (p->left != NULL)
    {
        p = p->left;
    }
    return p;
}

template<class K, class V, class F>
void Dictionary<K, V, F>::clearHelper(Nod<K, V>* r)
{
    if (r != NULL)
    {
        clearHelper(r->left);
        clearHelper(r->right);
        delete r;
    }

}







template<class K, class V, class F>
void Dictionary<K, V, F>::addPair(K k, V v)
{

    Nod<K, V>* newNod = new Nod<K, V>(k, v);
    if (this->root == NULL)
    {
        this->root = newNod;
        root->color = "black";
    }
    else
    {
        Nod<K, V>* p = search(k);
        if (cmp(p->key, k) == 0)
        {
            p->value = v;
        }
        else
        {
            newNod->parent = p;
            if (cmp(k, p->key) == -1)
            {
                p->left = newNod;

            }
            else
            {
                p->right = newNod;
            }
        }
        if (newNod->parent != NULL && newNod->parent->parent != NULL)
        {

            fixViolation(newNod);
        }
    }


}

template<class K, class V, class F>
void Dictionary<K, V, F>::removeKey(K k)
{
    Nod<K, V>* p = search(k);

    if (p != NULL)
    {
        deleteNod(p);
    }
}

template<class K, class V, class F>
bool Dictionary<K, V, F>::findKey(K k)
{
    if (search(k) != NULL)
        return true;
    return false;
}

template<class K, class V, class F>
void Dictionary<K, V, F>::empty()
{

    clearHelper(this->root);
    root = NULL;

}







//-----OPERATORS-----

template<class K, class V, class F>
ostream& operator <<(ostream& out, Dictionary<K, V, F>& d1)
{
    d1.inorder(d1.getRoot());
    out << endl;
    d1.preorder(d1.getRoot());
    return out;

}

template<class K, class V, class F>
Dictionary<K, V, F>& Dictionary<K, V, F>::operator=(const Dictionary& d1)
{
    this->height = d1.height;
    this->root = copyHelper(d1.root);

    return *this;
}


template<class K, class V, class F>
V Dictionary<K, V, F>::operator[](K k)
{
    Nod<K, V>* p = search(k);
    return p->value;
}



int main()
{


    Dictionary<int, int, KeyComp<int>> tree;

    tree.addPair(12, 2);
    tree.addPair(2, 3);
    tree.addPair(1, 2);
    tree.addPair(3, 7);
    tree.addPair(12, 1);
    tree.addPair(9, 1);
    tree.addPair(19, 0);


    Dictionary<int, int, KeyComp<int>> d2;
    d2 = tree;

    tree.removeKey(2);
    tree.removeKey(3);
    tree.removeKey(1);

    cout << tree << endl;



    cout << d2;

    cout << tree[3];

    d2.empty();
    cout << d2;

    return 0;
}

