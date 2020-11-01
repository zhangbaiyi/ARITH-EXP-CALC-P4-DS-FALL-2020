//
// Created by Baiyi Zhang on 2020/10/30.
//

#ifndef ARITH_EXP_CALC_P4_DS_FALL_2020_ARITH_EXP_CALC_H
#define ARITH_EXP_CALC_P4_DS_FALL_2020_ARITH_EXP_CALC_H


#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <iostream>
using namespace std;
#define EXP_MAX_SIZE 100

string oriInput;

template <class T>
struct stackNode{
    T data;
    stackNode<T>* next;
};

template <class T>
class arithStack{
public:
    arithStack();
    ~arithStack();
    void Push(const T& toPush);
    bool Pop(T& toPop);
    bool getTop(T& toStore);
    bool isEmpty();
    int getSize();
    void makeEmpty();
private:
    stackNode<T> *top;
};

template<class T>
arithStack<T>::arithStack() {
    top= NULL;
}

template<class T>
arithStack<T>::~arithStack() {
    makeEmpty();
}

void expInput();
void calcExp(string& backEXP);
bool isNum(char toJudge);
int inStackPriority(char& ch);
int outStackPriority(char& ch);




#endif //ARITH_EXP_CALC_P4_DS_FALL_2020_ARITH_EXP_CALC_H
