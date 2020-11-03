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

vector<item> preProcess(string& exp){
//    auto it = exp.begin();

    int itemIndex=0;
    vector<item> result;


    for(auto it = exp.begin();it!=exp.end();it++)
    {


        if((!isOperator(*it)) || (*it!='(') || (*it!=')') )
        {
            cout<<"Bad Input"<<endl;
            exit(1);

        }

        if(!isNum(*it))
        {
            cout<<"Bad Input"<<endl;
            exit(1);
        }

        if(*it == ' ')
        {
            continue;
        }

        if(isOperator(*it)){
            item tmp;
            tmp.isOperator = true;
            tmp.op = *it;
            result.push_back(tmp);
        }

        if(isNum(*it)){
            int numDigits = 0;
            string toNum;
            for(auto it1 = it; isNum(*it1); it1++) {
                toNum.push_back(*it1);
            }
            strtof (toNum, &(*it1));



            item tmp;
            tmp.isOperator = true;
            tmp.op = *it;
        }

    }
}

void infix_to_postfix(string& postFixExp) {
    arithStack<char> st;

    int i = 0;
    char curTop = '\0';
    char _popped = '\0';
    while (oriInput[i] != '='  ) {

        st.getTop(curTop);

        //If operand is found, add to postfix-exp
        if (isNum(oriInput[i])) {
            postFixExp.push_back(oriInput[i]);
        }

        if (oriInput[i] == '(') {
            st.Push(oriInput[i]);
        }

        if (oriInput[i] == ')') {
            while (!st.isEmpty() && curTop != '(') {
                st.Pop(_popped);
                postFixExp.push_back(_popped);
                st.getTop(curTop);

            }
            char leftParenthesis;
            st.Pop(leftParenthesis);
            st.getTop(curTop);

        }

        if (isOperator(oriInput[i])) {
            st.getTop(curTop);
            if (st.isEmpty() || curTop == '(') {
                st.Push(oriInput[i]);
                st.getTop(curTop);

            }
            else{
                while (!st.isEmpty() && curTop != '(' && lessPrior(oriInput[i], curTop)) {
                    st.Push(oriInput[i]);
                    st.getTop(curTop);
                    postFixExp.push_back(curTop);
                }

                st.Push(oriInput[i]);
                st.getTop(curTop);

            }
        }
        i++;
    }
    while(!st.isEmpty())
    {
        st.Pop(_popped);
        postFixExp.push_back(_popped);
    }

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
    return (leftOpPrior < rightOpPrior) ;
}

bool equalPrior(char leftOp, char rightOp) {
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
    return (leftOpPrior = rightOpPrior) ;
}

int main(void){

    string postFixExp;
    expInput();
    preProcess(postFixExp);
    infix_to_postfix(postFixExp);
    cout<<postFixExp.c_str()<<endl;


}