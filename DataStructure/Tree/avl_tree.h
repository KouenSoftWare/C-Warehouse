#ifndef JK_DS_AVL_TREE_H  
#define JK_DS_AVL_TREE_H  

#include "binary_tree.h"
#include "tree.h"

#include <memory>

namespace jk{
namespace ds{

using std::shared_ptr;

template<class T_Value>
class AVLTree:public BinaryTree<T_Value>{
    public:
        void insert(const Node<T_Value>& n){
            insert(this->root, n);
        }

    private:
        void insert(shared_ptr<Leaf<T_Value>>& leaf, const Node<T_Value>& n){
            if (leaf.get() == nullptr){
                leaf.reset(new Leaf<T_Value>(n));
                this->length++;
            }else if(!(leaf.get()->data.isAlive())) {
                leaf.get()->data = n; 
                this->length++;
            }else if(leaf.get()->data == n){
                leaf.get()->data = n; 
            }else{
                if (leaf.get()->data.GetHash() > n.GetHash()){
                    insert(leaf.get()->left, n); 
                    if (this->height(leaf.get()->left.get()) - this->height(leaf.get()->right.get()) == 2){
                        if (n.GetHash() < leaf.get()->left.get()->data.GetHash())  
                            singleLeft(leaf);
                        else
                            doubleLeft(leaf);
                    }
                }else{
                    insert(leaf.get()->right, n); 
                    if (this->height(leaf.get()->left.get()) - this->height(leaf.get()->right.get()) == -2){
                        if (n.GetHash() > leaf.get()->right.get()->data.GetHash())  
                            singleRight(leaf);
                        else
                            doubleRight(leaf);
                    }
                } 
            }
            leaf.get()->height = max(this->height(leaf.get()->left.get()), this->height(leaf.get()->right.get()))+1;
        }

        void singleLeft(shared_ptr<Leaf<T_Value>>& leaf){
            shared_ptr<Leaf<T_Value>> left = leaf.get()->left; 
            leaf.get()->left = left.get()->right;
            left.get()->right = leaf;
            leaf.get()->height = max(this->height(leaf.get()->left.get()), this->height(leaf.get()->right.get()))+1;
            left.get()->height = max(this->height(left.get()->left.get()), leaf.get()->height)+1;
            leaf = left;
        }

        void singleRight(shared_ptr<Leaf<T_Value>>& leaf){
            shared_ptr<Leaf<T_Value>> right = leaf.get()->right; 
            leaf.get()->right = right.get()->left;
            right.get()->left = leaf;
            leaf.get()->height = max(this->height(leaf.get()->left.get()), this->height(leaf.get()->right.get()))+1;
            right.get()->height = max(this->height(right.get()->left.get()), leaf.get()->height)+1;
            leaf = right;
        }

        void doubleLeft(shared_ptr<Leaf<T_Value>>& leaf){
            singleRight(leaf.get()->left);
            singleLeft(leaf);
        }

        void doubleRight(shared_ptr<Leaf<T_Value>>& leaf){
            singleLeft(leaf.get()->right);
            singleRight(leaf);
        }
};


} // ds namespace end
} // jk namespace end

#endif
