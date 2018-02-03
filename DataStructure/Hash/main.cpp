#include <iostream>
#include <vector>
#include <string>
#include <map>

#include <time.h>
#include <stdlib.h>

#include "function_hash.h" 

using namespace std;


string GetStr()
{
    int length = 1+rand() % 20;
    string ret;
    for (int i=0; i<=length; i++){
        ret.push_back(char(rand()%57+65));
    }
    return ret;
}

int main()
{
    cout << "Hello World" << endl;

    jk::ds::FunctionHash<int> fi;
    jk::ds::FunctionHash<int> fi2(jk::ds::FunctionHash<int>::FType::QuadraticFunction);
    jk::ds::FunctionHash<string> fs;

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

    vector<string> vs;
    for (int i=0; i!=1000000; i++){
        vs.push_back(GetStr());
    }

    cout << "测试字符串开始" << endl;
    start = clock();
    for (int i=0; i!=1000000; i++){
        fs[vs[i]] = i;
    }
    finish = clock();
    cout << (double)(finish - start) / CLOCKS_PER_SEC << endl;

    start = clock();
    for (int i=0; i!=1000000; i++){
        fs[vs[i]];
    }
    finish = clock();
    cout << (double)(finish - start) / CLOCKS_PER_SEC << endl;

    return 0;
}
