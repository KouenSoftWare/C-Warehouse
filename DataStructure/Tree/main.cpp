#include <iostream>
#include <memory>

#include <time.h>
#include <stdlib.h>

#include "binary_tree.h"
#include "../Hash/function_hash.h"

using namespace std;
using jk::ds::BinaryTree;

int main()
{
    BinaryTree<int> bi;
    jk::ds::FunctionHash<int> vi;

    double x;
    srand((unsigned)time(NULL)); //srand(3)
    clock_t start, finish;

    start = clock();
    for (int i=0; i!=1000000; i++){
        x = 1 + rand() % 1000000;
        bi.jk::ds::DSBase<int>::insert(x, x);
    }
    finish = clock();
    cout << (double)(finish - start) / CLOCKS_PER_SEC << endl;

    start = clock();
    for (int i=0; i!=1000000; i++){
        x = 1 + rand() % 1000000;
        bi.jk::ds::DSBase<int>::find(x);
    }
    finish = clock();
    cout << (double)(finish - start) / CLOCKS_PER_SEC << endl;

    cout << bi.size() << endl;

    return 0;
}
