// Created by Baiyi Zhang on 2020/10/30.
//



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
struct stackNode {
    T data;
    stackNode<T>* next;
};

template <class T>
class arithStack {
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
    stackNode<T>* top;
};

typedef struct ITEM {
    bool isDigit = false;
    double digit = 0.0;
    bool isOperator = false;
    char op = '\0';
}item;

template<class T>
arithStack<T>::arithStack() {
    top = NULL;
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


template<class T>
void arithStack<T>::Push(const T &toPush) {

    stackNode<T>* newNode= (stackNode<T>*) malloc(sizeof(stackNode<T>));
    newNode -> data = toPush;
    newNode -> next = top;
    top = newNode;


}

template<class T>
bool arithStack<T>::Pop(T &toPop) {
    if(isEmpty()==true)
    {
        return false;
    }
    else {
        stackNode<T> *current = top;
        top = top->next;
        toPop = current->data;
        delete current;
        return true;
    }
}

template<class T>
bool arithStack<T>::getTop(T &toStore) {
    if(isEmpty()==true){
        return false;
    }
    else{
        toStore = top->data;
        return true;
    }
}

template<class T>
bool arithStack<T>::isEmpty() {

    if(top==NULL)
    {
        return true;
    }
    else{
        return false;
    }
}

template<class T>
int arithStack<T>::getSize() {
    stackNode<T>* current = top;
    int count = 0;
    while(current!=NULL)
    {
        current=  current->next;
        count++;
    }
    return count;
}

template<class T>
void arithStack<T>::makeEmpty() {

    stackNode<T>* current;
    while(top!=NULL) {
        current = top;
        top = top->next;
        delete current;
    }
}

bool expInput(){

    char* toInput = (char*) malloc(sizeof(char));
    cin>>toInput;
    if (strcmp("q",toInput) == 0)
    {
        return false;
    }
    oriInput = toInput;
    return true;
}

void preProcess(string& exp, vector<item>& result) {//省略内容均为退出代码exit(1)

    //排除非法字符
    for (auto it = exp.begin(); it != exp.end(); it++) {
        
        if (!isOperator(*it) && (!isNum(*it)) && (*it != '(') && (*it != ')') && (*it != '=')) {

            cout << "Bad Input" << endl;
            exit(1);

        }

        if (*it == '.') {
            cout << "Bad Input: decimal point ('.') is not supported" << endl;
            exit(1);
        }

        if (it == (exp.end() - 1) && *it != '=') {
            cout << "Bad Input" << endl;
            exit(1);
        }

    }
    //检查括号是否关闭。左括号则isParentOpen++；右括号反之。检查是否为0.
    int isParentOpen = 0;
    for (auto it = exp.begin(); it != exp.end(); it++) {
        if (*it == '(') {
            isParentOpen++;
        }
        if (*it == ')') {
            isParentOpen--;
        }
    }
    if (isParentOpen) {
        cout << "Bad Input" << endl;
        exit(1);
    }
    //出现连续的三个符号则为非法。为了便于正负号的运算此处判断标准为3个。后续会详细说明
    for (int i = 0; i < exp.size()-2;i++) {
        if (isOperator(exp[i]) && isOperator(exp[i+1]) && isOperator(exp[i+2]))
        {
            cout << "Bad Input" << endl;
            exit(1);
        }
    }

    //转换为项目是ITEM的函数
    for (auto it = exp.begin(); it != exp.end(); it++) {
        if (isOperator(*it) || (*it == '(') || (*it == ')')) {
            item tmp;
            tmp.isOperator = true;
            tmp.op = *it;
            result.push_back(tmp);
        }

        if (isNum(*it)) {
            string toNum;
            auto it1 = exp.begin();
            for (it1 = it; isNum(*it1) && it1 != exp.end(); it1++) {
                toNum.push_back(*it1);
            }
            item tmp;
            tmp.isDigit = true;
            tmp.digit = (double) stoi(toNum);
            result.push_back(tmp);
            it = it1 - 1;//大串的iterator转移到跳过数字区的第一个字符。此处-1是因为外围for循环需要+1.
            continue;
        }
    }

    //judging for negative numbers
    for (int i = 0; i < result.size(); i++) {
        //judging case when -1 is in the beginning.
        if (i == 0 && result[i].isOperator) {
            if (result[i].op == '-'&&result[i+1].op!='(') {
                result[i + 1].digit = -result[i + 1].digit;
                result[i].op = '\0';
                result[i].isOperator = false;
            }
        }
        //judging "exp like (-1*2)"
        if (i + 2 < result.size() && result[i].isOperator && result[i].op == '(' && result[i + 1].isOperator &&
            result[i + 1].op == '-') {
            result[i + 2].digit = -result[i + 2].digit;
            result[i + 1].op = '\0';
            result[i + 1].isOperator = false;
        }
        //judging cases like "2*-1+2"
        if (i + 2 < result.size() && isOperator(result[i].op) && result[i + 1].op == '-'&&result[i+2].op!='(') {
            result[i + 2].digit = -result[i + 2].digit;
            result[i + 1].op = '\0';
            result[i + 1].isOperator = false;
        }
    }

    //judging for positive numbers  with extra '+'
    for (int i = 0; i < result.size(); i++) {
        //judging case when in the beginning.
        if (i == 0 && result[i].isOperator) {
            if (result[i].op == '+') {
                result[i].op = '\0';
                result[i].isOperator = false;
            }
        }
        //judging "exp like (+1*2)"
        if (i + 2 < result.size() && result[i].isOperator && result[i].op == '(' && result[i + 1].isOperator &&
            result[i + 1].op == '+') {
            result[i + 1].op = '\0';
            result[i + 1].isOperator = false;
        }
        //judging cases like "2*+1+2"
        if (i + 2 < result.size() && isOperator(result[i].op) && result[i + 1].op == '+') {
            result[i + 1].op = '\0';
            result[i + 1].isOperator = false;
        }
    }

//    int equalPriorIndex = 0;
    int curFarEqualPriorIndex = 0;
    item leftParent;
    leftParent.isOperator = true;
    leftParent.op = '(';
    item rightParent;
    rightParent.isOperator = true;
    rightParent.op = ')';

    for (int i = 0; i < result.size(); i++) {
        //如果连续出现同级的运算符，加括号
        if (i + 2 < result.size() && result[i + 2].op != '(' && result[i + 2].op != ')' &&
            isOperator(result[i].op) && isOperator(result[i + 2].op) &&
            equalPrior(result[i].op, result[i + 2].op))
        {
            curFarEqualPriorIndex = i;
            for (int ii = i; ii < result.size(); ii = ii + 2) {

                if (equalPrior(result[i].op, result[ii].op) && isOperator(result[i].op) && isOperator(result[ii].op)) {
                    curFarEqualPriorIndex = ii;
                }
            }
            auto j = result.begin() + i;
            result.insert(j + 1, leftParent);
            result.insert(result.begin() + curFarEqualPriorIndex + 3, rightParent);

            int flipFlag = 0;
            //如果为负号和除号，里面的同级运算符加括号时需要变号。
            if (result[i].op == '-' || result[i].op == '/') {
                flipFlag = 1;
            }

            for (j = result.begin() + i + 2; j <= result.begin() + curFarEqualPriorIndex + 1; j++) {
                int flipped = 0;
                if (j->isOperator && flipFlag && !flipped) {
                    if (j->op == '-') {
                        j->op = '+';
                        flipped = 1;
                    }
                }
                if (j->isOperator && flipFlag && !flipped) {
                    if (j->op == '/') {
                        j->op = '*';
                        flipped = 1;
                    }
                }
                if (j->isOperator && flipFlag && !flipped) {
                    if (j->op == '+') {
                        j->op = '-';
                        flipped = 1;
                    }
                }
                if (j->isOperator && flipFlag && !flipped) {
                    if (j->op == '*') {
                        j->op = '/';
                        flipped = 1;
                    }
                }
            }
        }
    }

    //2(3-7)= case.
    for(int i = 0;i<result.size();i++)
    {
        if(i!= 0 && result[i].isOperator && result[i].op =='(' && result[i-1].isDigit ){
            item tmp;
            tmp.isOperator = true;
            tmp.op = '*';
            result.insert(result.begin()+i,tmp);
        }
    }

    //positive sign is before a left parenthesis.此时直接去符号
    for(int i = 0;i<result.size();i++)
    {
        if(i-1>=0 && i+1!= result.size() && result[i+1].isOperator && result[i+1].op =='(' && result[i].isOperator && result[i].op =='+' && result[i-1].isOperator &&isOperator(result[i-1].op)){
            result[i].isOperator = false;
            result[i].op = '\0';
        }
    }


    //negative sign is before a left parenthesis.在符号前面加一个0再加外围括号. 变成(0-()).
    for(int i = 0;i<result.size();i++)
    {
        if(i ==1 && result[i].op == '(' && result[i-1].op == '-')
        {
            item tmp;
            tmp.isDigit = true;
            tmp.digit = 0;
            result.insert(result.begin(),tmp);
            result.insert(result.begin(),leftParent);
            bool jumpFlag = 0;
            for(int j=i;j<result.size()&&!jumpFlag;j++)
            {
                if(result[j].op == ')')
                {
                    result.insert(result.begin()+j+1,rightParent);
                    jumpFlag = 1;
                }
            }
        }
        if(i-2>=0 && result[i].op == '(' && result[i-1].op == '-' && isOperator(result[i-2].op))
        {
            item tmp;
            tmp.isDigit = true;
            tmp.digit = 0;
            result.insert(result.begin()+i-1,tmp);
            result.insert(result.begin()+i-1,leftParent);
            bool jumpFlag = 0;
            for(int j=i;j<result.size()&&!jumpFlag;j++)
            {
                if(result[j].op == ')')
                {
                    result.insert(result.begin()+j+1,rightParent);
                    jumpFlag = 1;
                }
            }
        }

    }

    for (int i = 0; i < result.size()-1; i++)//判断连续的符号。在这一步出现再连续符号是因为源数据有非法的多符号连续输入。
    {
        if (result[i].isOperator&&result[i+1].isOperator)
        {
            if (result[i].op == '(' || result[i + 1].op == '(' || result[i].op == ')' || result[i + 1].op == ')')
            {
                continue;
            }
            else {
                cout << "Bad Input" << endl;
                exit(1);
            }
        }
    }

    item equalSign;
    equalSign.isOperator = true;
    equalSign.op = '=';
    result.push_back(equalSign);
}

void infix_to_postfix(vector<item>& processedInput, vector<item>& result) {

    arithStack<item> st;

    item curTop ;
    item _popped ;
    for (int i=0 ; i<processedInput.size(); i++) {

        st.getTop(curTop);

        if (!processedInput[i].isDigit && !processedInput[i].isOperator) {
            continue;
        }

        //If operand is found, add to postfix-exp
        if (processedInput[i].isDigit) {
            result.push_back(processedInput[i]);
        }

        if (processedInput[i].op == '(')//遇到左括号进栈
        {
            st.Push(processedInput[i]);
        }

        if (processedInput[i].op == ')') {
            while (!st.isEmpty() && curTop.op != '(') {//一直输出栈内运算符直至遇到左括号
                st.Pop(_popped);
                result.push_back(_popped);
                st.getTop(curTop);//每出一次栈就更新一下栈顶元素
            }
            //to pop left parenthesis
            st.Pop(_popped);//左括号出栈
            st.getTop(curTop);
        }

        if (isOperator(processedInput[i].op))
        {
            if (st.isEmpty() || curTop.op == '(') {
                st.Push(processedInput[i]);
                st.getTop(curTop);
            }
            else{
                while (!st.isEmpty() && curTop.op != '(' && lessPrior(processedInput[i].op, curTop.op)) {//辅助函数lessPrior判断优先级大小
                    st.Pop(_popped);
                    result.push_back(_popped);
                    st.getTop(curTop);
                }
                while (!st.isEmpty() && curTop.op != '(' && processedInput[i].op == '%' && (curTop.op =='*'||curTop.op =='/')) {
                    st.Pop(_popped);
                    result.push_back(_popped);
                    st.getTop(curTop);
                }
                st.Push(processedInput[i]);
                st.getTop(curTop);
            }
        }
    }
    while(!st.isEmpty())
    {
        st.Pop(_popped);
        result.push_back(_popped);
    }

}

double calcRes(vector<item>& postfixExp) {
    arithStack<item> st;
    item result;
    item right;
    item left;
    for (int i = 0; i < postfixExp.size(); i++) {
        if (!postfixExp[i].isOperator && !postfixExp[i].isDigit) {
            continue;
        }
        if (isOperator(postfixExp[i].op)) {//运算符取栈顶两位元素进行运算
            st.getTop(right);
            st.Pop(right);
            st.getTop(left);
            st.Pop(left);
            if (postfixExp[i].op == '+') {
                item tmp;
                tmp.isDigit = true;
                tmp.digit = left.digit + right.digit;
                st.Push(tmp);//运算结果进栈
            }
            if (postfixExp[i].op == '-') {
                item tmp;
                tmp.isDigit = true;
                tmp.digit = left.digit - right.digit;
                st.Push(tmp);
            }
            if (postfixExp[i].op == '*') {
                item tmp;
                tmp.isDigit = true;
                tmp.digit = left.digit * right.digit;
                st.Push(tmp);
            }
            if (postfixExp[i].op == '/') {
                item tmp;
                tmp.isDigit = true;
                tmp.digit = left.digit / right.digit;
                st.Push(tmp);
            }
            if (postfixExp[i].op == '%') {
                item tmp;
                tmp.isDigit = true;
                tmp.digit = (int) left.digit % (int) right.digit;
                st.Push(tmp);
            }
            if (postfixExp[i].op == '^') {
                item tmp;
                tmp.isDigit = true;
                tmp.digit = pow(left.digit, right.digit);
                st.Push(tmp);
            }

        } else {//数字进栈
            st.Push(postfixExp[i]);
        }
    }
    st.getTop(result);
    return result.digit;
}

bool isNum(char toJudge){
    if(toJudge>='0'&&toJudge<='9')
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool isOperator(char toJudge)
{
    if(toJudge=='+'||toJudge=='-'||toJudge=='*'||toJudge=='/'||toJudge=='^'||toJudge=='%')
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool lessPrior(char leftOp, char rightOp) {
    int leftOpPrior = 0;
    int rightOpPrior = 0;
    switch (leftOp) {

        case '^':
            leftOpPrior = 3 ;
            break;
        case '*':
        case '/':
        case '%':
            leftOpPrior = 2 ;
            break;
        case '+':
        case '-':
            leftOpPrior = 1 ;
            break;
    }
    switch (rightOp) {

        case '^':
            rightOpPrior = 3 ;
            break;
        case '*':
        case '/':
        case '%':
            rightOpPrior = 2 ;
            break;
        case '+':
        case '-':
            rightOpPrior = 1 ;
            break;
    }
    return (leftOpPrior < rightOpPrior) ;
}

bool equalPrior(char leftOp, char rightOp) {
    int leftOpPrior = 0;
    int rightOpPrior = 0;
    switch (leftOp) {

        case '^':
            leftOpPrior = 3 ;
            break;
        case '*':
        case '/':
//        case '%':
            leftOpPrior = 2 ;
            break;
        case '+':
        case '-':
            leftOpPrior = 1 ;
            break;
    }
    switch (rightOp) {

        case '^':
            rightOpPrior = 3 ;
            break;
        case '*':
        case '/':
//        case '%':
            rightOpPrior = 2 ;
            break;
        case '+':
        case '-':
            rightOpPrior = 1 ;
            break;
    }
    if(!leftOpPrior && !rightOpPrior)
    {
        return false;
    }
    return (leftOpPrior == rightOpPrior) ;
}

void menu() {
    cout << "Please enter a infix expression." << endl;
    cout << "请输入一个中缀表达式" << endl;
    cout << "Please enter '=' at the end of expresion" << endl;
    cout << "请在结尾输入等号 = " << endl;
    cout << "Please note that this program can only calculate integer" << endl;
    cout << "请注意本程序只计算整数，小数点会被视为Bad Input" << endl;
    cout << "The operators can be one of below:" << endl;
    cout << "请注意本程序的运算符仅限以下几种" << endl;
    cout << "'+','-','*','/','%','^','(',')'" << endl;
    cout << R"(The Blankspace ends the process of input. For example, "2 +5/2" is read as "2".)" << endl;
    cout << "空格会视为输入结束。例如，\"2 +5/2\" 会被读作 \"2\" " << endl;
    cout << "enter a 'q' can quit the program." << endl;
}

int main() {
    bool continueFlag = true;
    while (continueFlag) {
        menu();
        continueFlag = expInput();//调用expInput()来进行输入
        vector<item> processedInput;//这两个装item的向量分别为预处理后的表达式和转化为后缀的表达式。
        vector<item> postfixExp;
        preProcess(oriInput, processedInput);
        infix_to_postfix(processedInput, postfixExp);//利用这两个函数进行处理。
        if (continueFlag) {
            cout << "The postfix expression is:" << endl;
            cout << "后缀表达式为:" << endl;
            for (int i = 0; i < postfixExp.size(); i++) {
                if (postfixExp[i].isDigit)
                    cout << postfixExp[i].digit;
                if (postfixExp[i].isOperator)
                    cout << postfixExp[i].op;
            }
            //展示后缀表达式。

            double result = calcRes(postfixExp);
            //计算后缀表达式。
            cout << endl;
            cout << "The result is:" << endl;
            cout << "最终结果为:" << endl;
            cout << result << endl;
        }
    }
    getchar();
    return 0;
}