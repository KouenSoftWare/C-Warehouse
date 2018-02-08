#ifndef JK_DS_B_TREE_H  
#define JK_DS_B_TREE_H  

#include "tree.h"

#include <vector>
#include <algorithm>

namespace jk{
namespace ds{

using std::shared_ptr;
using std::vector;

template<class T_Value, int Width=5>
class BTree:public DSBase<T_Value>{
    public:
        void insert(const Node<T_Value>& n){
            if(!checkSplit(root.get())){
                shared_ptr<BNode> newRoot(new BNode());
                newRoot.get()->hasLeaf = false;
                newRoot.get()->childrens[0] = root;
                newRoot.get()->split(0);
                root.swap(newRoot);
            }
            insert(root, n);
        }

        void remove(const Node<T_Value>& n){ remove(root, n); }
        const T_Value* const find(const Node<T_Value>& n) const { return find(root.get(), n); }

        template<class Key>
        T_Value& operator[](const Key& k){
            Node<T_Value> n(k);  
            const T_Value* const ptr = find(n);
            if (ptr == nullptr){
                throw NodeFindError(); 
            }
            return *const_cast<T_Value*>(ptr); 
        }
        
        void clear(){ ; }

        std::vector<Node<T_Value>> all(){
            std::vector<Node<T_Value>> vc;
            root.get()->getDatas(vc); 
            return vc;
        }
    private:
        struct BNode{
            bool hasLeaf;
            int sign[Width-1]; 
            int useSign;
            shared_ptr<void> childrens[Width];

            BNode(): useSign(0){
                for (int i=0; i!=Width-1; i++)
                    sign[i] = 0;
            }
            
            void add(const Node<T_Value>& n){
                for (int i=0; i!=useSign; i++){
                    if (sign[i] == 0)
                        sign[i] = n.GetHash();

                    if (n.GetHash() < sign[i]){
                        if (hasLeaf){
                            insert(n, i);
                        }else{
                            ((BNode*)childrens[i].get())->add(n);
                        }
                        return;
                    }
                }
                if (hasLeaf)
                    insert(n, useSign);
                else
                    ((BNode*)childrens[useSign].get())->add(n);
            }

            int min(){
                if (hasLeaf){
                    vector<Node<T_Value>>& vc = getVC(0);
                    std::sort(vc.begin(), vc.end());
                    return vc.begin()->GetHash(); 
                }else{
                    return ((BNode*)childrens[0].get())->min();
                } 
            }

            int getInsertPositon(int value){
                for (int i=0; i!=useSign; i++){
                    if (value <= sign[i])
                        return i;
                    else if (value <= sign[i+1] || sign[i+1] == 0)
                        return i+1;
                }
                return -1;
            }

            vector<Node<T_Value>>& getVC(int pos){
                 return *((vector<Node<T_Value>>*)childrens[pos].get());
            }

            void insert(const Node<T_Value>& n, int pos){
                vector<Node<T_Value>>& vc = getVC(pos);
                auto it = std::find(vc.begin(), vc.end(), n); 
                if (it != vc.end()){
                    *it = n; 
                }else{
                    vc.push_back(n);
                }
            }

            T_Value* find(const Node<T_Value>& n, int pos){
                vector<Node<T_Value>>& vc = getVC(pos);
                for (int i=0; i!=vc.size(); i++){
                    if (vc[i] == n)
                        return vc[i].val();
                } 
                return nullptr;
            }

            void remove(const Node<T_Value>& n, int pos){
                vector<Node<T_Value>>& vc = getVC(pos);
                auto iter = std::find(vc.begin(),vc.end(), n.GetHash());
                if (iter != vc.end())
                    vc.erase(iter);
            }

            BNode& operator=(const BNode& n){
                hasLeaf = n.hasLeaf; 
                for (int i=0; i!=Width-1; i++)
                    sign[i] = n.sign[i];
                for (int i=0; i!=Width; i++)
                    childrens[i] = n.childrens[i];
            }  

            bool split(int pos){
                if (useSign == Width-1){
                    return false;
                }
                for (int i=useSign+1; i!=pos; i--)
                    childrens[i] = childrens[i-1];            

                for (int i=0; i!=useSign-pos;i++)
                    sign[useSign-i] = sign[useSign-i-1];

                if (hasLeaf){
                    vector<Node<T_Value>>* p = new vector<Node<T_Value>>();
                    vector<Node<T_Value>>& vc = getVC(pos);
                    std::sort(vc.begin(), vc.end());
                    p->insert(p->begin(), vc.begin(), vc.begin()+int(Width/2.0+0.5));
                    vc.erase(vc.begin(), vc.begin()+int(Width/2.0+0.5));
                    sign[pos] = vc.begin()->GetHash();
                    childrens[pos].reset(p); 
                }else{
                    BNode* p = new BNode(); 
                    BNode* fullNodePtr = (BNode*)childrens[pos].get();

                    p->hasLeaf = fullNodePtr->hasLeaf;
                    for (int i=0; i!=int(Width/2.0+0.5);i++){
                        p->childrens[i].swap(fullNodePtr->childrens[i]); 
                    }
                    p->useSign = int(Width/2.0+0.5)-1;
                    for (int i=0; i!=p->useSign; i++){
                        if (!p->hasLeaf)
                            p->sign[i] = ((BNode*)p->childrens[i+1].get())->min(); 
                        else{
                            auto vc = p->getVC(i+1);
                            std::sort(vc.begin(), vc.end());
                            p->sign[i] = vc.begin()->GetHash();
                        }
                    }
                    for (int i=int(Width/2.0+0.5),j=0; i!=Width;i++,j++)
                        fullNodePtr->childrens[j].swap(fullNodePtr->childrens[i]);
                    fullNodePtr->useSign = Width-int(Width/2.0+0.5)-1;

                    for (int i=0; i!=Width-1; i++)
                        fullNodePtr->sign[i] = 0;
                    for (int i=0; i!=fullNodePtr->useSign; i++){
                        if (!p->hasLeaf){
                            fullNodePtr->sign[i] = ((BNode*)fullNodePtr->childrens[i+1].get())->min();    
                        }else{
                            auto vc = fullNodePtr->getVC(i+1);
                            std::sort(vc.begin(), vc.end());
                            fullNodePtr->sign[i] = vc.begin()->GetHash();    
                        }
                    }

                    int minValue = fullNodePtr->min();
                    sign[pos] = minValue;
                    childrens[pos].reset(p); 
                }
                useSign++;
                return true;
            }

            void getDatas(std::vector<Node<T_Value>>& vc){
                if (hasLeaf){
                    for (int i=0; i<=useSign; i++){
                        auto v = getVC(i); 
                        vc.insert(vc.end(), v.begin(), v.end());
                    }
                }else{
                    for (int i=0; i!=useSign; i++)
                        ((BNode*)childrens[i].get())->getDatas(vc);
                } 
            }
        };

        void insert(shared_ptr<BNode>& t, const Node<T_Value>& n){
            if (t.get() == nullptr){
                BNode* p = new BNode();
                p->hasLeaf = true;
                p->useSign = 1;
                p->childrens[0].reset(new vector<Node<T_Value>>()); 
                p->childrens[1].reset(new vector<Node<T_Value>>()); 
                t.reset(p);
            }

            t.get()->add(n);    
        }

        void remove(shared_ptr<BNode>& t, const Node<T_Value> n){
            if (t.get() == nullptr) return; 

            int pos = t.get()->getInsertPositon(n.GetHash());
            if (pos == -1) return; 

            if(t.get()->hasLeaf)
                t.get()->remove(n, pos);
            else{
                shared_ptr<BNode> p(((BNode*)t.get()->childrens[pos].get()));
                remove(p, n); 
            }
        } 

        const T_Value* const find(BNode* t, const Node<T_Value> n) const{
            if (t == nullptr) return nullptr; 

            int pos = t->getInsertPositon(n.GetHash());
            if (pos == -1) return nullptr; 

            if(t->hasLeaf){
                T_Value* p = t->find(n, pos);
                if (p == nullptr) return nullptr;

                return (const T_Value* const)p;
            }else{
                return find(((BNode*)t->childrens[pos].get()), n); 
            }
        } 

        bool checkSplit(BNode* t){
            if (t == nullptr) return true; 
            if(!t->hasLeaf){
                for (int i=0; i<=t->useSign;i++){
                    if (!checkSplit((BNode*)t->childrens[i].get()))
                        return t->split(i);
                }
            }else{
                for (int i=0; i<=t->useSign;i++){
                    if(t->getVC(i).size() == Width){
                        return t->split(i);
                    }
                }
            }
            return true;
        }
    private:
        shared_ptr<BNode> root;
};

}
}

#endif
