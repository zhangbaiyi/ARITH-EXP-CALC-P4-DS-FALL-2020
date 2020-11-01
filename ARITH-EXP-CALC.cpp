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


int main(void){

    arithStack<int> stack;
    stack.Push(1);
    int yes;
    stack.Push(2);
    stack.Push(3);
    cout<<stack.getSize()<<endl;
    int store;
    stack.getTop(store);
    stack.Pop(yes);
    cout<<yes<<endl;
    cout<<store<<endl;
    cout<<"This is a moyu contribution."<<endl;


}