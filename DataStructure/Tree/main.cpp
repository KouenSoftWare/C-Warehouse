#include <iostream>
#include <memory>

#include <time.h>
#include <stdlib.h>

#include "binary_tree.h"
#include "avl_tree.h"
#include "hash_tree.h"
#include "b_tree.h"
#include "../Hash/function_hash.h"

using namespace std;
using jk::ds::BinaryTree;
using jk::ds::HashTree;
using jk::ds::AVLTree;
using jk::ds::BTree;

const int NUMBER = 100000;
const int MAX = 100000;

template<class T>
void Test(jk::ds::DSBase<T>* container, string name){
    clock_t start, finish;
    int x;

    start = clock();
    for (int i=0; i!=NUMBER; i++){
        x = 1 + rand() % MAX;
        container->insert(x, x);
    }

    finish = clock();
    cout << name << "的插入时间:" <<  (double)(finish - start) / CLOCKS_PER_SEC << endl;

    start = clock();
    for (int i=0; i!=NUMBER; i++){
        x = 1 + rand() % MAX;
        container->find(x);
    }
    finish = clock();
    cout << name << "的搜索时间:" <<  (double)(finish - start) / CLOCKS_PER_SEC << endl;
    cout << endl;
}

int main()
{
    BinaryTree<int> bi;
    AVLTree<int> ai;
    BTree<int, 10> bbi;
    HashTree<int> hi;
    jk::ds::FunctionHash<int> vi;
    srand((unsigned)time(NULL)); //srand(3)

    cout << "开始测试" << endl;
    Test(&vi, "散列");
    Test(&bi, "二叉树");
    Test(&ai, "平衡树");
    Test(&bbi, "B树");
    Test(&hi, "Hash树");

    return 0;
}
