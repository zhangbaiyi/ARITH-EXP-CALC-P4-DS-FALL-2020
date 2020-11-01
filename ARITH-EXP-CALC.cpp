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

void toBackExp(string& backEXP){
    arithStack<char> toBackWorkStack;
    toBackWorkStack.Push('#');
    char curTop = '\0';
    char toPop = '\0';
    for(int i = 0; toBackWorkStack.isEmpty()==false && oriInput[i] != '=' && i < oriInput.size() ; i++)
    {
        if( isNum(oriInput[i]) )
        {
            backEXP.push_back(oriInput[i]);
        }
        else {
            toBackWorkStack.getTop(curTop);//tmp is the top operator in the stack
            if(inStackPriority(curTop)<outStackPriority(oriInput[i]))
            {
                toBackWorkStack.Push(oriInput[i]);
                //then : jumps to the big for-loop
            }
            else if(inStackPriority(curTop)>outStackPriority(oriInput[i]))
            {
                toBackWorkStack.Pop(toPop);
                backEXP.push_back(toPop);
            }
            else
            {
                toBackWorkStack.Pop(toPop);
                if(toPop = '(')
                {
                    continue;
                }
            }
        }
    }
}

bool isNum(char toJudge){
    if(toJudge>=48&&toJudge<=57)
    {
        return true;
    }
    else
    {
        return false;
    }
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



}