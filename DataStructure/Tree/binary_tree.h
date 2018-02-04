#ifndef JK_DS_BINARY_TREE_H  
#define JK_DS_BINARY_TREE_H  

#include "tree.h"

namespace jk{
namespace ds{

using std::shared_ptr;

template<class T_Value>
class BinaryTree:public DSBase<T_Value>{
    public:  
        BinaryTree():root(nullptr), length(0) {}
        void insert(const Node<T_Value>& n){
            this->insert(root, n);
        }

        template<class KEY>
        T_Value& operator[](const KEY& key) {
            Node<T_Value> n(key);
            shared_ptr<Leaf<T_Value>>& ptr = const_cast<shared_ptr<Leaf<T_Value>>&>(this->find(root, key));
            if (ptr.get() == nullptr) {
                ptr.reset(new Leaf<T_Value>(n));
                length++;
            }
            return *(ptr.get()->data.val());
        }

        void remove(const Node<T_Value>& n){
            this->remove(root, n);    
        }

        const T_Value* const find(const Node<T_Value>& n) const{
            const shared_ptr<Leaf<T_Value>>& p = this->find(root, n);
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
    protected:
        void printTree(const shared_ptr<Leaf<T_Value>>& leaf, std::ostream& out, int d) const{
            std::string space(d, '*');
            if (leaf.get() != nullptr){
                out << space << leaf.get()->data.GetHash() << std::endl;
                printTree(leaf.get()->left, out, d+1);
                printTree(leaf.get()->right, out, d+1);
            }
        }

        virtual void insert(shared_ptr<Leaf<T_Value>>& leaf, const Node<T_Value>& n){
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
                }else{
                    insert(leaf.get()->right, n); 
                } 
            }
            int l = height(leaf.get()->left.get()), r = height(leaf.get()->right.get());
            leaf.get()->height = (l > r ? l : r)+1;
        }

        void remove(shared_ptr<Leaf<T_Value>>& leaf, const Node<T_Value>& n){
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

        const shared_ptr<Leaf<T_Value>>& find(const shared_ptr<Leaf<T_Value>>& leaf, const Node<T_Value>& n) const{
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

        int height(Leaf<T_Value>* leaf){
            return leaf == nullptr?-1:leaf->height; 
        }
    protected:
        int length;
        shared_ptr<Leaf<T_Value>> root;
};


} // ds namespace end
} // jk namespace end

#endif
