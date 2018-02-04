#ifndef JK_DS_B_TREE_H  
#define JK_DS_B_TREE_H  

#include "tree.h"

#include <vector>

namespace jk{
namespace ds{

using std::shared_ptr;

template<class T_Value, int Width=5>
class BTree:public DSBase<T_Value>{
    public:
        void insert(const Node<T_Value>& n){
            if (insert(root, n)){
                shared_ptr<BNode> newRoot(new BNode(false));
                newRoot.get()->useSign = 2;

                std::vector<Node<T_Value>> vc;
                root.get()->getDatas(vc); 
                int perNode = int(vc.size()/2+0.5); 
                int middle = int(perNode/2);

                for (int i=0; i!=2; i++){
                    newRoot.get()->setSign(vc[middle+perNode*i], i);
                    for (int j=0; j!=perNode; j++){
                        newRoot.get()->add(vc[j+(i*perNode)]);
                    }
                }
                root = newRoot;
                insert(root, n);
            }
        }
        void remove(const Node<T_Value>&);
        const T_Value* const find(const Node<T_Value>& n) const;

        template<class Key>
        T_Value& operator[](const Key& k);
    private:
        struct BNode{
            bool hasLeaf;
            int sign[Width]; 
            int dataCount[Width];
            int useSign;
            shared_ptr<void> childrens[Width];

            BNode(): useSign(0), hasLeaf(true) { init(); }
            BNode(bool b): useSign(0), hasLeaf(b) { init(); }
            BNode(const Node<T_Value>& n): useSign(0), hasLeaf(true) { 
                init(); 
                sign[1] = n.GetHash();
                useSign++;
                add(n);
            }

            void init(){
                if (hasLeaf)
                    for (int i=0; i!=Width;i++) 
                        childrens[i] = std::make_shared<Node<T_Value>>(
                            new Node<T_Value>[Width], 
                            std::default_delete<Node<T_Value>[]>()
                        );
                else
                    for (int i=0; i!=Width;i++) 
                        childrens[i] = shared_ptr<BNode>(new BNode());
            }

            void reset(){
                for (int i=0; i!=Width;i++) 
                    dataCount[i] = sign[i] = 0;
                if (hasLeaf)
                    for (int i=0; i!=Width;i++) 
                        childrens[i] = std::make_shared<Node<T_Value>>(
                            new Node<T_Value>[Width], 
                            std::default_delete<Node<T_Value>[]>()
                        );
                else
                    for (int i=0; i<=useSign;i++) 
                        ((BNode*)childrens[i].get())->reset();
                useSign = 0;
            }
            
            bool add(const Node<T_Value>& n){
                for (int i=0; i!=Width-1 && (sign[i] != 0 || i == 0); i++){
                    if (n.GetHash() <= sign[i]){
                        if (hasLeaf)
                            return insert(n, i);
                        else
                            return ((BNode*)childrens[i].get())->add(n);
                    }else if (n.GetHash <= sign[i+1] || sign[i+1] == 0){
                        if (hasLeaf)
                            return insert(n, i);
                        else
                            return ((BNode*)childrens[i].get())->add(n);
                    }
                }
                return false;
            }

            int getInsertPositon(int value){
                for (int i=0; i!=Width-1 && (sign[i] != 0 || i == 0); i++){
                    if (value <= sign[i])
                        return i;
                    else if (value <= sign[i+1] || sign[i+1] == 0)
                        return i+1;
                }
                return -1;
            }

            bool insert(const Node<T_Value>& n, int pos){
                if (dataCount[pos] != Width){
                    for(int i=0; i!=Width-1; i++){
                        if (((Node<T_Value>*)childrens[pos].get())[i].GetHash() == 0){
                            ((Node<T_Value>*)childrens[pos].get())[i] = n;
                            dataCount[pos]++;
                            return true;
                        }else if (((Node<T_Value>*)childrens[pos].get())[i] >= n){
                            for(int j=Width-1; j != i; j--){
                                ((Node<T_Value>*)childrens[pos].get())[j] = ((Node<T_Value>*)childrens[pos].get())[j-1];
                            }        
                            ((Node<T_Value>*)childrens[pos].get())[i] = n;
                            dataCount[pos]++;
                            return true;
                        }
                    }
                }
                return false;
            }

            void setSign(int s, int pos){
                if (pos < Width-1){
                    sign[pos] = s; 
                }
            }

            BNode& operator=(const BNode& n){
                hasLeaf = n.hasLeaf; 
                for (int i=0; i!=Width-1; i++)
                    sign[i] = n.sign[i];
                for (int i=0; i!=Width; i++)
                    childrens[i] = n.childrens[i];
            }  

            bool split(){
                if (useSign != Width-1){
                    return false; 
                }
                std::vector<Node<T_Value>> vc;
                this->getDatas(vc); 
                int perNode = int(vc.size()/(useSign+1)+0.5); 

                if (hasLeaf){
                    if (perNode >= Width) throw std::string("split，进行分配，判断每批超过上限");

                    this->reset();
                    for(int i=0; i!=useSign+1; i++){
                        for (int j=0; j!=perNode; j++){
                            ((Node<T_Value>*)childrens[i].get())[j] = vc[j+(i*perNode)]; 
                        }
                    }
                }else{
                    int middle = int(perNode/2);
                    for (int i=0; i!=useSign+1; i++){
                        setSign(vc[middle+perNode*i], i);
                        ((BNode*)childrens[i].get())->reset();
                        for (int j=0; j!=perNode; j++){
                            ((BNode*)childrens[i].get())->add(vc[j+(i*perNode)]);
                        }
                    }
                }

                useSign++;
                return true;
            }

            void getDatas(std::vector<Node<T_Value>>& vc){
                if (hasLeaf){
                    for (int i=0; i!=Width; i++)
                        for (int j=0; j!=dataCount[i];j++){
                            vc.push_back(((Node<T_Value>*)childrens[i].get())[j]);
                        }
                }else{
                    for (int i=0; i!=Width; i++)
                        ((BNode*)childrens[i].get())->getDatas(vc);
                } 
            }
        };

        bool insert(shared_ptr<BNode>& t, const Node<T_Value>& n){
            bool result;
            if (t.get() == nullptr) {
                t = shared_ptr<BNode>(new BNode(n));
            }else{
                if (t.get()->hasLeaf()){
                    result = t.get()->add(n);    
                }else{
                    int pos = t.get()->getInsertPositon(n);
                    if (pos == -1){
                        return false; 
                    }else{
                        result = insert(t.get()->childrens[pos], n);
                    }
                }
            }
            if (!result){
                if (t.get()->split())
                    return insert(t, n);
                else
                    return false;
            }
        }
    private:
        shared_ptr<BNode> root;
};

}
}

#endif
