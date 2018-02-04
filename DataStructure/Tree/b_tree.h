#ifndef JK_DS_B_TREE_H  
#define JK_DS_B_TREE_H  

#include "../Common/base.h"

#include <memory>

namespace jk{
namespace ds{

using std::shared_ptr;

template<class T_Value, int Width=5>
class BTree:public DSBase<T_Value>{
    public:
        void insert(const Node<T_Value>&);
        void remove(const Node<T_Value>&);
        const T_Value* const find(const Node<T_Value>& n) const;

        template<class Key>
        T_Value& operator[](const Key& k);
    private:
};

}
}

#endif
