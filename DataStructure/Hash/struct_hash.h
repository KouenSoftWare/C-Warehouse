#ifndef JK_STRUCT_HASH_H  
#define JK_STRUCT_HASH_H

#include "hash.h"
#include "../Common/base.h"

#include <string>
#include <memory>

namespace jk{
namespace ds{

template<class T_Value, class T_Container>
class StructHash:public DSBase<T_Value>{
    public:
        StructHash():maxSize(cInitHashSize), length(0) {
            table = std::shared_ptr<Node<DSBase<T_Value>>>(
                new Node<T_Container>[maxSize], 
                std::default_delete<Node<T_Container>[]>()
            );  
        }

        StructHash(int s) :maxSize(s), length(0) {
            table = std::shared_ptr<Node<DSBase<T_Value>>>(
                new Node<T_Container>[maxSize], 
                std::default_delete<Node<T_Container>[]>()
            );  
        }
 
        StructHash(const StructHash<T_Value, T_Container>& sh)
            :maxSize(sh.maxSize), length(sh.length) {
            table = std::shared_ptr<Node<DSBase<T_Value>>>(
                new Node<T_Container>[maxSize], 
                std::default_delete<Node<T_Container>[]>()
            );  
            for (int i=0; i!=maxSize;i++)
                table.get()[i] = sh.table.get()[i];
        }

        void insert(const Node<T_Value>&);
        void remove(const Node<T_Value>&);

        const T_Value* const find(const Node<T_Value>&) const;

        StructHash<T_Value, T_Container>& operator+(const StructHash<T_Value, T_Container>&);
        StructHash<T_Value, T_Container>& operator-(const StructHash<T_Value, T_Container>&);

        StructHash<T_Value, T_Container>& operator=(const StructHash<T_Value, T_Container>&);
        StructHash<T_Value, T_Container>& operator+=(const StructHash<T_Value, T_Container>&);
        StructHash<T_Value, T_Container>& operator-=(const StructHash<T_Value, T_Container>&);

        void resize(int);
        void clear(){ for (auto i:table) i.value.clear(); }
        int size() { return length; }
    private:
        void copy(const StructHash<T_Value, T_Container>&, StructHash<T_Value, T_Container>&);
        void opera(const StructHash<T_Value, T_Container>&, StructHash<T_Value, T_Container>&, HashMethod);
        int maxSize;
        int length;
        std::shared_ptr<Node<DSBase<T_Value>>> table;
};

template<class T_Value, class T_Container>
void StructHash<T_Value, T_Container>::insert(const Node<T_Value> &n)
{
    int h = n.GetHash(this->maxSize);
    this->table.get()[h].insert(n);
}

template<class T_Value, class T_Container>
void StructHash<T_Value, T_Container>::remove(const Node<T_Value> &n)
{
    int h = n.GetHash(this->maxSize);
    this->table.get()[h].remove(n);
}

template<class T_Value, class T_Container>
const T_Value* const StructHash<T_Value, T_Container>::find(const Node<T_Value>& n) const
{
    int h = n.GetHash(this->maxSize);
    return this->table.get()[h].find(n);
}

template<class T_Value, class T_Container>
StructHash<T_Value, T_Container>& StructHash<T_Value, T_Container>::operator+(const StructHash<T_Value, T_Container>& sh)
{
    StructHash<T_Value, T_Container> third(*this);
    this->opera(third, sh, HashMethod::Insert);
    return third;
}

template<class T_Value, class T_Container>
StructHash<T_Value, T_Container>& StructHash<T_Value, T_Container>::operator-(const StructHash<T_Value, T_Container>& sh)
{
    StructHash<T_Value, T_Container> third(*this);
    this->opera(third, sh, HashMethod::Remove);
    return third;
}

template<class T_Value, class T_Container>
StructHash<T_Value, T_Container>& StructHash<T_Value, T_Container>::operator=(const StructHash<T_Value, T_Container>& sh)
{
    if (this == &sh){
        return *this;
    }
    copy(sh, *this);
}

template<class T_Value, class T_Container>
StructHash<T_Value, T_Container>& StructHash<T_Value, T_Container>::operator+=(const StructHash<T_Value, T_Container>& sh)
{
    this->opera(*this, sh, HashMethod::Insert);
    return *this;
}

template<class T_Value, class T_Container>
StructHash<T_Value, T_Container>& StructHash<T_Value, T_Container>::operator-=(const StructHash<T_Value, T_Container>& sh)
{
    this->opera(*this, sh, HashMethod::Remove);
    return *this;
}

template<class T_Value, class T_Container>
void StructHash<T_Value, T_Container>::resize(int s)
{
    StructHash<T_Value, T_Container> newSH(s);
    copy(*this, newSH);
    *this = newSH;
}

template<class T_Value, class T_Container>
void StructHash<T_Value, T_Container>::copy(const StructHash<T_Value, T_Container>& source, StructHash<T_Value, T_Container>& target)
{
    for (int i=0; i!=source.maxSize; i++){
        Node<T_Value>* pData = source.table.get()[i].pop();
        while(pData != nullptr){
            target.insert(*pData);
            pData = source.table.get()[i].pop();
        }
    }
}

template<class T_Value, class T_Container>
void StructHash<T_Value, T_Container>::opera(const StructHash<T_Value, T_Container>& source, StructHash<T_Value, T_Container>& target, HashMethod method)
{
    auto oldTable = std::shared_ptr<Node<DSBase<T_Value>>>(
        new Node<T_Container>[source.maxSize], 
        std::default_delete<Node<T_Container>[]>()
    );  

    for (int i=0; i!=source.maxSize; i++){
        if (!source.table.get()[i].isAlive())
            continue;

        Node<T_Value>* pData = source.table.get()[i].pop();
        while(pData != nullptr){
            if (method == HashMethod::Insert){
                target.insert(*pData);
            }else{
                target.remove(*pData);
            }
            pData = source.table.get()[i].pop();
            oldTable.get()[i].insert(*pData);
        }
    }
    source.table = oldTable;
}


} // ds namespace end
} // jk namespace end
#endif
