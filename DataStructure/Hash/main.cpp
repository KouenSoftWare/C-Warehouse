#include <iostream>
#include <map>

#include <time.h>
#include <stdlib.h>

#include "function_hash.h" 

using namespace std;

int main()
{
    cout << "Hello World" << endl;

    jk::ds::FunctionHash<int> fi;
    jk::ds::FunctionHash<int> fi2(jk::ds::FunctionHash<int>::FType::QuadraticFunction);
    map<int, int> mi;

    double x;
    srand((unsigned)time(NULL)); //srand(3)
    clock_t start, finish;

    start = clock();
    for (int i=0; i!=1000000; i++){
        x = 1 + rand() % 10000000;
        fi[x] = x;
    }
    finish = clock();
    cout << (double)(finish - start) / CLOCKS_PER_SEC << endl;
    start = clock();
    for (int i=0; i!=1000000; i++){
        x = 1 + rand() % 10000000;
        fi.find(x);
    }
    finish = clock();
    cout << (double)(finish - start) / CLOCKS_PER_SEC << endl;


    start = clock();
    for (int i=0; i!=1000000; i++){
        x = 1 + rand() % 10000000;
        fi2[x] = x;
    }
    finish = clock();
    cout << (double)(finish - start) / CLOCKS_PER_SEC << endl;
    start = clock();
    for (int i=0; i!=1000000; i++){
        x = 1 + rand() % 10000000;
        fi2.find(x);
    }
    finish = clock();
    cout << (double)(finish - start) / CLOCKS_PER_SEC << endl;

    start = clock();
    for (int i=0; i!=1000000; i++){
        x = 1 + rand() % 10000000;
        mi[x] = x;
    }
    finish = clock();
    cout << (double)(finish - start) / CLOCKS_PER_SEC << endl;
    start = clock();
    for (int i=0; i!=1000000; i++){
        x = 1 + rand() % 10000000;
        mi[x];
    }
    finish = clock();
    cout << (double)(finish - start) / CLOCKS_PER_SEC << endl;
    return 0;
}