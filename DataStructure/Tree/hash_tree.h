#ifndef JK_DS_HASH_TREE_H
#define JK_DS_HASH_TREE_H


#include "../Common/base.h"

#include <memory>
#include <vector>

namespace jk{
namespace ds{

using std::shared_ptr;
using std::vector;

template<class T_Value>
class HashTree:public DSBase<T_Value>{
    public:
        void insert(const Node<T_Value>& n){
            if (root.get() == nullptr)
                root = shared_ptr<Leaf>(new Leaf(0)); 
            insert(root, n);
        }

        void remove(const Node<T_Value>& n){
            remove(root, n);
        }

        const T_Value* const find(const Node<T_Value>& n) const{
            return find(root, n); 
        }
    private:
        struct Leaf{
            bool noUse;
            int level;
            Node<T_Value> data;
            vector<shared_ptr<Leaf>> childrens;

            Leaf(): noUse(true), data(), childrens(), level(-1) {}
            Leaf(int l): noUse(true), data(), level(l), childrens() {}
            void buildChildrens(){
                for (int i=0; i!=prime(level+1); i++){
                    shared_ptr<Leaf> p(new Leaf(level+1));
                    childrens.push_back(p);
                }
            }

            int prime(int l){
                int start = 1;
                while (l >= 0){
                    start++;
                    if (isPrime(start))
                        l--; 
                } 
                return start;
            }
            bool isPrime(int n){
                bool ret = true; 
                for (int i=2; i<n; i++){
                    if (n % i == 0){
                        ret = false;
                        break;
                    }
                }
                return ret;
            }
        };

        void insert(const shared_ptr<Leaf>& p, const Node<T_Value>& n){
            if (p.get()->noUse){
                p.get()->data = n; 
                p.get()->noUse = false;
                if (p.get()->childrens.empty())
                    p.get()->buildChildrens(); 
            }else{
                int subIndex = n.GetHash(p.get()->prime(p.get()->level+1)); 
                insert(p->childrens[subIndex], n);
            }
        }

        void remove(const shared_ptr<Leaf>& p, const Node<T_Value>& n){
            if (p.get()->data == n){
                p.get()->noUse = true;
            }else{
                int subIndex = n.GetHash(p.get()->prime(p.get()->level+1)); 
                if (!p.get()->childrens.empty()){
                    remove(p.get()->childrens[subIndex], n);
                }
            }
        }

        const T_Value* const find(const shared_ptr<Leaf>& p, const Node<T_Value>& n) const {
            if (p.get()->data == n && !p.get()->noUse){
                return (const T_Value* const)p.get()->data.val();  
            }else{
                int subIndex = n.GetHash(p.get()->prime(p.get()->level+1)); 
                if (!p.get()->childrens.empty()){
                    find(p.get()->childrens[subIndex], n);
                }else{
                    return nullptr; 
                }
            }
        }
    private:
        shared_ptr<Leaf> root;
};


}
}

#endif
