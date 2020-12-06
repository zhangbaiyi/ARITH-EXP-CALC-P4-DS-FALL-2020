//
// Created by Baiyi Zhang on 2020/10/30.
//

#ifndef ARITH_EXP_CALC_BUILD_H
#define ARITH_EXP_CALC_BUILD_H


#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <iostream>
#include <vector>
#include <math.h>


using namespace std;
#define EXP_MAX_SIZE 100 //预先设定一个字符串的最大存储值，以免出现溢出的错误。

string oriInput;//original input。最初的输入数据。用全局变量更方便其他函数调用。

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
    void Push(const T& toPush);//压栈函数
    bool Pop(T& toPop);//退栈函数，栈顶内容存储在toPop中。
    bool getTop(T& toStore);
    bool isEmpty();
    int getSize();
    void makeEmpty();
private:
    stackNode<T> *top;//这是栈元素的结点，作为私有数据成员。
};

typedef struct ITEM{//存储字符串翻译过来的数字或者符号。要用布尔值区分便于后续计算等函数计算。
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
