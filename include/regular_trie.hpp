#ifndef REGULAR_TRIE_H
#define REGULAR_TRIE_H

#include <iostream>
#include <map> 

using namespace std;

class Trie{
    public:
    // private:
        map<uint64_t, Trie*> children;
        map<uint64_t, Trie*>::iterator it;
        bool has_children;
    // public:
    
        Trie(){
            has_children = false;
        }

        ~Trie(){
            for(auto &child: children){
                delete child.second;
            }
        }

        Trie* insert(uint64_t);
        void traverse();
        bool hasChildren();
        uint64_t childrenCount();
        map<uint64_t, Trie*> getChildren();
};

/*
    Creates a new node in the trie if the tag wasn't already in the trie
*/
Trie* Trie::insert(uint64_t tag){
    has_children = true;
    it = children.find(tag);
    Trie* node;
    if(it == children.end()){
        node = new Trie();
        children[tag] = node;
    }
    else{
        node = children[tag];
    }
    return node;
}

/*
    Traverses trie in preorder printer tags of children on each node
*/
void Trie::traverse(){
    if(has_children){
        it = children.begin();
        while(it!=children.end()){
            cout<< it->first <<" ";
            it++;
        }
        cout<<endl;

        it = children.begin();
        
        while(it != children.end()){
            it->second->traverse();
            it++;
        }
    }
}

/*
    Returns true if the node has children
*/
bool Trie::hasChildren(){
    return has_children;
}

/*
    Returns the amount of children of the node
*/
uint64_t Trie::childrenCount(){
    return children.size();
}

/*
    Returns a map with de children of the node and their associated tags
*/
map<uint64_t, Trie*> Trie::getChildren(){
    return children;
}
#endif