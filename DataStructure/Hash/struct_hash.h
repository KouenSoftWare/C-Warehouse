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
        StructHash()
            :maxSize(cInitHashSize), length(0) {
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

        void insert(std::string, const T_Value&);
        void insert(int, const T_Value&);

        void remove(std::string);
        void remove(int);

        int rehash(int) const;

        const T_Value& find(std::string) const;
        const T_Value& find(int) const;

        T_Value& find(std::string);
        T_Value& find(int);

        const T_Value& operator[](std::string) const;
        const T_Value& operator[](int) const;

        T_Value& operator[](std::string);
        T_Value& operator[](int);

        StructHash<T_Value, T_Container> operator+(const StructHash<T_Value, T_Container>&);
        StructHash<T_Value, T_Container> operator-(const StructHash<T_Value, T_Container>&);

        StructHash<T_Value, T_Container>& operator=(const StructHash<T_Value, T_Container>&);
        StructHash<T_Value, T_Container>& operator+=(const StructHash<T_Value, T_Container>&);
        StructHash<T_Value, T_Container>& operator-=(const StructHash<T_Value, T_Container>&);

        void resize(int);
        void clear(){ for (auto i:table) i.value.clear(); }
        int size() { return length; }
    private:
        int maxSize;
        int length;
        std::shared_ptr<Node<DSBase<T_Value>>> table;
};

template<class T_Value, class T_Container>
void StructHash<T_Value, T_Container>::resize(int s)
{
    auto newTable = std::shared_ptr<Node<DSBase<T_Value>>>(
        new Node<T_Container>[s], 
        std::default_delete<Node<T_Container>[]>()
    );  

    for (int i=0; i!=maxSize; i++){
        Node<T_Value>* pData = table.get()[i].pop();
        while(pData != nullptr){
             
        }
    }
    this->maxSize = s;
}

} // ds namespace end
} // jk namespace end
#endif
