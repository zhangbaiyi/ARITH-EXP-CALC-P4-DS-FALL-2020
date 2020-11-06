//
// Created by Baiyi Zhang on 2020/10/30.
//

#ifndef ARITH_EXP_CALC_P4_DS_FALL_2020_ARITH_EXP_CALC_H
#define ARITH_EXP_CALC_P4_DS_FALL_2020_ARITH_EXP_CALC_H


#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <iostream>
#include <vector>
#include <math.h>


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

typedef struct ITEM{
    bool isDigit = false;
    double digit = 0.0;
    bool isOperator = false;
    char op = '\0';
}item;

template<class T>
arithStack<T>::arithStack() {
    top= NULL;
}

template<class T>
arithStack<T>::~arithStack() {
    makeEmpty();
}

void menu();
bool expInput();
void preProcess(string& exp, vector<item>& result);
void infix_to_postfix(vector<item>& processedInput, vector<item>& result);
double calcRes(vector<item>& postfixExp);

bool isNum(char toJudge);
bool isOperator(char toJudge);
bool lessPrior(char leftOp, char rightOp);
bool equalPrior(char leftOp, char rightOp);





#endif //ARITH_EXP_CALC_P4_DS_FALL_2020_ARITH_EXP_CALC_H
