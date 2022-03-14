#ifndef COMPACT_TRIE_IV_H
#define COMPACT_TRIE_IV_H

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sdsl/vectors.hpp>
#include "utils.hpp"
#include "trie_interface.hpp"

using namespace std;
using namespace sdsl;

class CompactTrieIV: public TrieInterface{
    private:
        bit_vector B;
        int_vector<> seq;
        // wm_int<> wt;

        /*
            Initializes rank and select support for B
        */
       void initializeSupport(){
            util::init_support(b_rank1,&B);
            util::init_support(b_rank0,&B);
            util::init_support(b_sel1,&B);
            util::init_support(b_sel0,&B);
       }

    public:

        //Rank & Support arrays
        rank_support_v<1> b_rank1;
        rank_support_v<0> b_rank0;
        select_support_mcl<0> b_sel0;
        select_support_mcl<1> b_sel1;

        CompactTrieIV(bit_vector b, int_vector<> s){
            B = b;
            seq = s;
            initializeSupport();
        }
        /*
            Constructor from file with representation
        */
        CompactTrieIV(string file_name){
            loadFromFile(file_name);
        };

        /*
            Destructor
        */
        ~CompactTrieIV(){};

        uint64_t size(){
            return size_in_bytes(B) + size_in_bytes(seq) + size_in_bytes(b_rank1) + \
            size_in_bytes(b_rank0) + size_in_bytes(b_sel0) + size_in_bytes(b_sel1);
        }
        /*
            Recives index in bit vector
            Returns index of next 0
        */
        uint64_t succ0(uint64_t it){
            uint64_t cant_0 = b_rank0(it);
            return b_sel0(cant_0 + 1);
        }
        
        /*
            Recives index in bit vector
            Returns index of previous 0
        */
        uint64_t prev0(uint64_t it){
            uint64_t cant_0 = b_rank0(it);
            return b_sel0(cant_0);
        }

        /*
            Recives index of current node and the child that is required
            Returns index of the nth child of current node
        */
        uint64_t child(uint64_t it, uint64_t n){
            return b_sel0(b_rank1(it+n)) + 1;
        }

        /*
            Recives index of node whos children we want to count
            Returns how many children said node has
        */
        uint64_t childrenCount(uint64_t it){
            return succ0(it) - it;
        }

        /*
            Recives node index
            Returns index of position in parent
        */
        uint64_t getPosInParent(uint64_t it){
            return b_sel1(b_rank0(it));
        }

        /*
            Recives index of node
            Return which child of its parent it is
        */
        uint64_t childRank(uint64_t it){
            uint64_t pos = getPosInParent(it);
            return pos - prev0(pos);
        }

        /*
            Recives index of node
            Returns index of parent node
        */  
        uint64_t parent(uint64_t it){
            uint64_t pos = getPosInParent(it);
            return prev0(pos) + 1;
        }

        /*
            Returns key that corresponds to given node(it)
        */
        uint64_t key_at(uint64_t it){
            return seq[b_rank0(it)-2];
        }

        pair<uint64_t, uint64_t> binary_search_seek(uint64_t val, uint64_t i, uint64_t f){
            if(seq[f]<val)return make_pair(0,f+1);
            uint64_t mid; 
            while(i<f){
                mid = (i + f)/2;
                if(seq[mid]<val)i = mid+1;
                else if(seq[mid]>=val)f = mid;
            }
            return make_pair(seq[i], i);
        }
        
        /*
            Stores Compact Trie Iterator to file saving the size of B, B and S.
        */
        void storeToFile(string file_name){
            string B_file = file_name + ".B";
            string IV_file = file_name + ".IV";
            store_to_file(B, B_file);
            store_to_file(seq, IV_file);
        }

        /*
            Loads Compact Trie from file restoring B and the Wavelet Tree
        */
        void loadFromFile(string file_name){
            string B_file = file_name + ".B";
            string IV_file = file_name + ".IV";
            load_from_file(B, B_file);
            load_from_file(seq, IV_file);
            initializeSupport();
        }
};

#endif