#ifndef JK_DS_AVL_TREE_H  
#define JK_DS_AVL_TREE_H  

#include "../Common/base.h"

#include <memory>

namespace jk{
namespace ds{

using std::shared_ptr;

template<class T_Value>
class AVLTree:public DSBase<T_Value>{
    public:
        void insert(const Node<T_Value>& n){
            this->insert(root, n);
        }

        template<class KEY>
        T_Value& operator[](const KEY& key) {
            Node<T_Value> n(key);
            shared_ptr<Leaf>& ptr = const_cast<shared_ptr<Leaf>&>(this->find(root, key));
            if (ptr.get() == nullptr) {
                ptr.reset(new Leaf(n));
                length++;
            }
            return *(ptr.get()->data.val());
        }

        void remove(const Node<T_Value>& n){
            this->remove(root, n);    
        }

        const T_Value* const find(const Node<T_Value>& n) const{
            const shared_ptr<Leaf>& p = this->find(root, n);
            if (p.get() != nullptr)
                return (const T_Value*)p.get()->data.val();
            return nullptr;    
        }

        void clear(){}
        Node<T_Value>* pop(){ return nullptr; }
        int size(){ return length; }

        void printTree(std::ostream& out) const{
            printTree(root, out, 0);
        }
    private:
        struct Leaf{
            Node<T_Value> data;
            shared_ptr<Leaf> left;
            shared_ptr<Leaf> right;
            int height;

            Leaf(const Node<T_Value>& n):data(n), left(), right(), height(0){}
        };

        void printTree(const shared_ptr<Leaf>& leaf, std::ostream& out, int d) const{
            std::string space(d, '*');
            if (leaf.get() != nullptr){
                out << space << leaf.get()->data.GetHash() << std::endl;
                printTree(leaf.get()->left, out, d+1);
                printTree(leaf.get()->right, out, d+1);
            }
        }
        void insert(shared_ptr<Leaf>& leaf, const Node<T_Value>& n){
            if (leaf.get() == nullptr){
                leaf.reset(new Leaf(n));
                this->length++;
            }else if(!(leaf.get()->data.isAlive())) {
                leaf.get()->data = n; 
                this->length++;
            }else if(leaf.get()->data == n){
                leaf.get()->data = n; 
            }else{
                if (leaf.get()->data.GetHash() > n.GetHash()){
                    insert(leaf.get()->left, n); 
                    if (height(leaf.get()->left.get()) - height(leaf.get()->right.get()) == 2){
                        if (n.GetHash() < leaf.get()->left.get()->data.GetHash())  
                            singleLeft(leaf);
                        else
                            doubleLeft(leaf);
                    }
                }else{
                    insert(leaf.get()->right, n); 
                    if (height(leaf.get()->left.get()) - height(leaf.get()->right.get()) == -2){
                        if (n.GetHash() > leaf.get()->right.get()->data.GetHash())  
                            singleRight(leaf);
                        else
                            doubleRight(leaf);
                    }
                } 
            }
            leaf.get()->height = max(height(leaf.get()->left.get()), height(leaf.get()->right.get()))+1;
        }

        void singleLeft(shared_ptr<Leaf>& leaf){
            shared_ptr<Leaf> left = leaf.get()->left; 
            leaf.get()->left = left.get()->right;
            left.get()->right = leaf;
            leaf.get()->height = max(height(leaf.get()->left.get()), height(leaf.get()->right.get()))+1;
            left.get()->height = max(height(left.get()->left.get()), leaf.get()->height)+1;
            leaf = left;
        }

        void singleRight(shared_ptr<Leaf>& leaf){
            shared_ptr<Leaf> right = leaf.get()->right; 
            leaf.get()->right = right.get()->left;
            right.get()->left = leaf;
            leaf.get()->height = max(height(leaf.get()->left.get()), height(leaf.get()->right.get()))+1;
            right.get()->height = max(height(right.get()->left.get()), leaf.get()->height)+1;
            leaf = right;
        }

        void doubleLeft(shared_ptr<Leaf>& leaf){
            singleRight(leaf.get()->left);
            singleLeft(leaf);
        }

        void doubleRight(shared_ptr<Leaf>& leaf){
            singleLeft(leaf.get()->right);
            singleRight(leaf);
        }

        void remove(shared_ptr<Leaf>& leaf, const Node<T_Value>& n){
            if (leaf.get() != nullptr){
                if (leaf.get()->data.GetHash() == n.GetHash()){
                    leaf.get()->data.del(); 
                    this->length--;
                }else if (leaf.get()->data.GetHash() > n.GetHash()){
                    remove(leaf.get()->left, n); 
                }else{
                    remove(leaf.get()->right, n); 
                } 
            } 
        }

        const shared_ptr<Leaf>& find(const shared_ptr<Leaf>& leaf, const Node<T_Value>& n) const{
            if (leaf.get() != nullptr){
                if (leaf.get()->data.isAlive() && leaf.get()->data.GetHash() == n.GetHash()){
                    return leaf; 
                }else if (leaf.get()->data.GetHash() > n.GetHash()){
                    return find(leaf.get()->left, n); 
                }else{
                    return find(leaf.get()->right, n); 
                } 
            }else{
                return leaf;  
            } 
        }

        int height(Leaf* leaf){
            return leaf == nullptr?-1:leaf->height; 
        }
        int max(int a, int b){
            return a > b? a: b;
        }
    private:
        int length;
        shared_ptr<Leaf> root;
};


} // ds namespace end
} // jk namespace end

#endif
