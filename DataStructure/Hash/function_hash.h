#ifndef JK_DS_FUNCTION_HASH_H  
#define JK_DS_FUNCTION_HASH_H

#include "../Common/base.h"
#include "hash.h"

#include <string>
#include <memory>

namespace jk{
namespace ds{

    
template<class VALUE>
class FunctionHash:public DSBase<VALUE>{
    public:
        enum FType{LinerFunction, QuadraticFunction};

        ~FunctionHash() { }

        FunctionHash()
            :maxSize(cInitHashSize), length(0), analyMethod(FType::LinerFunction){
                table = std::shared_ptr<Node<VALUE>>(new Node<VALUE>[maxSize], std::default_delete<Node<VALUE>[]>());  
            } 

        FunctionHash(FType f)
            :maxSize(cInitHashSize), length(0), analyMethod(f){
                table = std::shared_ptr<Node<VALUE>>(new Node<VALUE>[maxSize], std::default_delete<Node<VALUE>[]>());  
            }

        FunctionHash(const FunctionHash<VALUE>& fh)
            :maxSize(fh.maxSize), analyMethod(fh.analyMethod), length(fh.length){
                table = std::shared_ptr<Node<VALUE>>(new Node<VALUE>[maxSize], std::default_delete<Node<VALUE>[]>());  
                for (int i=0; i!=maxSize;i++)
                    table.get()[i] = fh.table.get()[i];
            }

        void insert(const Node<VALUE>&);
        void remove(const Node<VALUE>&);

        int rehash(int) const;

        const VALUE* const find(const Node<VALUE>&) const;

        const VALUE& operator[](std::string) const;
        const VALUE& operator[](int) const;

        VALUE& operator[](std::string);
        VALUE& operator[](int);

        FunctionHash<VALUE> operator+(const FunctionHash<VALUE>&);
        FunctionHash<VALUE> operator-(const FunctionHash<VALUE>&);

        FunctionHash<VALUE>& operator=(const FunctionHash<VALUE>&);
        FunctionHash<VALUE>& operator+=(const FunctionHash<VALUE>&);
        FunctionHash<VALUE>& operator-=(const FunctionHash<VALUE>&);

        int size();
        void resize(int);
        void clear();
    private:
        void insert(const Node<VALUE>& n, int h)
        {
            if (length/maxSize > cHashChangeRate)
                resize(maxSize*2); 
            
            while (table.get()[h].status == Node<VALUE>::NodeStatus::Active)
                h = rehash(h); 

            table.get()[h] = n;
        }

        void opera(FunctionHash<VALUE>&, const FunctionHash<VALUE>&, HashMethod);

        FType analyMethod;
        int maxSize;
        int length;
        std::shared_ptr<Node<VALUE>> table;
};

template<class VALUE>
void FunctionHash<VALUE>::insert(const Node<VALUE>& n)
{
    int h = n.GetHash(this->maxSize);
    insert(n, h);
}

template<class VALUE>
void FunctionHash<VALUE>::remove(const Node<VALUE>& n)
{
    int h = n.GetHash(this->maxSize);
    while (table.get()[h] == n)
        h = rehash(h);
    table.get()[h].del();
}

template<class VALUE>
int FunctionHash<VALUE>::rehash(int h) const
{
    switch (analyMethod){
        case FType::LinerFunction:
            return h+1;
        case FType::QuadraticFunction:
            return h*h;
        default:
            throw HashRehashError();
    }
}

template<class VALUE>
const VALUE* const FunctionHash<VALUE>::find(const Node<VALUE>& n) const
{
    int h = n.GetHash(this->maxSize); 

    while (table.get()[h].isAlive()){
        if (table.get()[h] == n)
            break;
        h = this->rehash(h);
    }

    return table.get()[h].val();
}

template<class VALUE>
const VALUE& FunctionHash<VALUE>::operator[](std::string key) const
{
    return this->find(key);
}

template<class VALUE>
const VALUE& FunctionHash<VALUE>::operator[](int key) const
{
    return this->find(key);
}

template<class VALUE>
VALUE& FunctionHash<VALUE>::operator[](std::string key)
{
    return *const_cast<VALUE*>(((const FunctionHash<VALUE> *)this)->find(key));
}

template<class VALUE>
VALUE& FunctionHash<VALUE>::operator[](int key)
{
    return *const_cast<VALUE*>(((const FunctionHash<VALUE> *)this)->find(key));
}

template<class VALUE>
FunctionHash<VALUE>& FunctionHash<VALUE>::operator=(const FunctionHash<VALUE>& r)
{
    if (this == &r){
        return *this;
    }
    table = std::shared_ptr<Node<VALUE>>(new Node<VALUE>[r.maxSize], std::default_delete<Node<VALUE>[]>());  
    for(int i=0;i!=r.maxSize;i++){
        table.get()[i] = r.table.get()[i];
    }
    length = r.length;
    analyMethod = r.analyMethod;
    maxSize = r.maxSize;
    return *this;
}

template<class VALUE>
void FunctionHash<VALUE>::opera(FunctionHash<VALUE>& f1, const FunctionHash<VALUE>& f2, HashMethod sign)
{
    for (int i=0; i!=f2.maxSize; i++){
        if (f2.table.get()[i].status != Node<VALUE>::NodeStatus::Active)
            continue;
        if (f2.table.get()[i].keyType == Node<VALUE>::KeyType::String)
            if (sign == HashMethod::Insert)
                f1.insert(f2.table.get()[i].sKey, f2.table.get()[i].value);
            else
                f1.remove(f2.table.get()[i].sKey);
        else
            if (sign == HashMethod::Insert)
                f1.insert(f2.table.get()[i].iKey, f2.table.get()[i].value);
            else
                f1.remove(f2.table.get()[i].iKey);
    }
}

template<class VALUE>
FunctionHash<VALUE> FunctionHash<VALUE>::operator+(const FunctionHash<VALUE>& r)
{
    FunctionHash<VALUE> third(*this);
    this->opera(third, r, HashMethod::Insert);
    return third;
}

template<class VALUE>
FunctionHash<VALUE> FunctionHash<VALUE>::operator-(const FunctionHash<VALUE>& r)
{
    FunctionHash<VALUE> third(*this);
    this->opera(third, r, HashMethod::Remove);
    return third;
}

template<class VALUE>
FunctionHash<VALUE>& FunctionHash<VALUE>::operator+=(const FunctionHash<VALUE>& r)
{
    this->opera(*this, r, HashMethod::Insert);
    return *this;
}

template<class VALUE>
FunctionHash<VALUE>& FunctionHash<VALUE>::operator-=(const FunctionHash<VALUE>& r)
{
    this->opera(*this, r, HashMethod::Remove);
    return *this;
}

template<class VALUE>
int FunctionHash<VALUE>::size()
{
    return this->length;
}

template<class VALUE>
void FunctionHash<VALUE>::resize(int s)
{
    auto newTable = table = std::shared_ptr<Node<VALUE>>(new Node<VALUE>[s], std::default_delete<Node<VALUE>[]>());  

    int newPos;
    for (int i=0; i!= this->maxSize; i++){
        newPos = this->table.get()[i].GetHash(s);
        newTable.get()[newPos] = this->table.get()[i];
    }

    this->table = newTable;
    this->maxSize = s;
}

template<class VALUE>
void FunctionHash<VALUE>::clear()
{
    for (int i=0; i!=maxSize;i++){
        this->table.get()[i].del();
    }
}

} // ds namespace end
} // jk namespace end

#endif