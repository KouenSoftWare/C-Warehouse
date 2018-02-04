#ifndef JK_DS_TREE_H
#define JK_DS_TREE_H


#include "../Common/base.h"

#include <memory>
#include <ostream>
#include <string>

namespace jk{
namespace ds{

template<class T_Value>    
struct Leaf{
    Node<T_Value> data;
    std::shared_ptr<Leaf> left;
    std::shared_ptr<Leaf> right;
    int height;

    Leaf(const Node<T_Value>& n):data(n), left(), right(), height(0){}
};

}
}

#endif
