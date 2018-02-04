#include <iostream>
#include <memory>

#include <time.h>
#include <stdlib.h>

#include "binary_tree.h"
#include "avl_tree.h"
#include "../Hash/function_hash.h"

using namespace std;
using jk::ds::BinaryTree;
using jk::ds::AVLTree;

int main()
{
    BinaryTree<int> bi;
    AVLTree<int> ai;
    jk::ds::FunctionHash<int> vi;

    const int NUMBER = 1000000;
    const int MAX = 1000000;

    double x;
    srand((unsigned)time(NULL)); //srand(3)
    clock_t start, finish;

    start = clock();
    for (int i=0; i!=NUMBER; i++){
        x = 1 + rand() % MAX;
        bi.jk::ds::DSBase<int>::insert(x, x);
    }
    finish = clock();
    cout << (double)(finish - start) / CLOCKS_PER_SEC << endl;

    start = clock();
    for (int i=0; i!=NUMBER; i++){
        x = 1 + rand() % MAX;
        bi.jk::ds::DSBase<int>::find(x);
    }
    finish = clock();
    cout << (double)(finish - start) / CLOCKS_PER_SEC << endl;

    start = clock();
    for (int i=0; i!=NUMBER; i++){
        x = 1 + rand() % MAX;
        ai.jk::ds::DSBase<int>::insert(x, x);
    }
    finish = clock();
    cout << (double)(finish - start) / CLOCKS_PER_SEC << endl;

    start = clock();
    for (int i=0; i!=NUMBER; i++){
        x = 1 + rand() % MAX;
        ai.jk::ds::DSBase<int>::find(x);
    }
    finish = clock();
    cout << (double)(finish - start) / CLOCKS_PER_SEC << endl;

    cout << "##" << ai.size() << endl;


    return 0;
}
