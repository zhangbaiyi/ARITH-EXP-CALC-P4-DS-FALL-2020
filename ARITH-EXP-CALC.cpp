#include "ARITH-EXP-CALC.h"

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

void expInput(){

    char* toInput = (char*) malloc(sizeof(char));
    cin>>toInput;
    oriInput = toInput;
}

void toBackExp(string& backEXP) {
    arithStack<char> toBackWorkStack;

    int i = 0;
    char curTop = '\0';
    char _popped = '\0';
    while (oriInput[i] != '=') {

        toBackWorkStack.getTop(curTop);

        //If operand is found, add to postfix-exp
        if (isNum(oriInput[i])) {
            backEXP.push_back(oriInput[i]);
        }

        if (oriInput[i] == '(') {
            toBackWorkStack.Push(oriInput[i]);
        }

        if (oriInput[i] == ')') {
            while (!toBackWorkStack.isEmpty() && curTop != '(') {
                toBackWorkStack.Pop(_popped);
                backEXP.push_back(_popped);
            }
            char leftParenthesis;
            toBackWorkStack.Pop(leftParenthesis);
        }

        if (isOperator(oriInput[i])) {
            toBackWorkStack.getTop(curTop);
            if (toBackWorkStack.isEmpty() || curTop == '(') {
                toBackWorkStack.Push(oriInput[i]);
            }
            else{
                while (!toBackWorkStack.isEmpty() && curTop != '(' && lessPrior(oriInput[i], curTop)) {
                    toBackWorkStack.Push(oriInput[i]);
                    backEXP.push_back(curTop);
                }
                toBackWorkStack.Push(oriInput[i]);
            }
        }
        i++;
    }
    while(!toBackWorkStack.isEmpty())
    {
        toBackWorkStack.Pop(_popped);
        backEXP.push_back(_popped);
    }

}








//    toBackWorkStack.Push('#');
//    char curTop = '\0';
//    char toPop = '\0';
//    int i=0;
//    while(!toBackWorkStack.isEmpty() && oriInput[i] != '=')
//    {
//        if( isNum(oriInput[i]) )
//        {
//            backEXP.push_back(oriInput[i]);
//            i++;
//        }
//        else {
//            toBackWorkStack.getTop(curTop);
//            //tmp is the top operator in the stack
//            if(inStackPriority(curTop) < outStackPriority(oriInput[i]))
//            {
//                toBackWorkStack.Push(oriInput[i]);
//                i++;
//                //then : jumps to the big for-loop
//            }
//            else if(inStackPriority(curTop) > outStackPriority(oriInput[i]))
//            {
//                toBackWorkStack.Pop(toPop);
//                backEXP.push_back(toPop);
//                i++;
//            }
//            else if(inStackPriority(curTop) == outStackPriority(oriInput[i]))
//            {
//                toBackWorkStack.Pop(toPop);
//                if(toPop = '(')
//                {
//                    i++;
//                }
//            }
//        }
//    }
//}

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
    int leftOpPrior;
    int rightOpPrior;
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
    return (leftOpPrior<=rightOpPrior) ;
}
int inStackPriority(char& ch) {
    switch (ch) {
        case '#':
            return 0;
        case '(':
            return 1;
        case '^':
            return 7;
        case '*':
        case '/':
        case '%':
            return 5;
        case '+':
        case '-':
            return 3;
        case ')':
            return 8;

    }
}

int outStackPriority(char& ch) {
    switch (ch) {
        case '#':
            return 0;
        case '(':
            return 8;
        case '^':
            return 6;
        case '*':
        case '/':
        case '%':
            return 4;
        case '+':
        case '-':
            return 2;
        case ')':
            return 1;

    }
}
int main(void){

    string backEXP;
    expInput();
    toBackExp(backEXP);
    cout<<backEXP.c_str()<<endl;


}