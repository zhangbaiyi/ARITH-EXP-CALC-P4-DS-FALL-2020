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

void preProcess(string& exp, vector<item>& result) {

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
            it = it1 - 1;
            continue;
        }
    }

    //judging for negative numbers
    for (int i = 0; i < result.size(); i++) {
        //judging case when -1 is in the beginning.
        if (i == 0 && result[i].isOperator) {
            if (result[i].op == '-') {
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
        if (i + 2 < result.size() && isOperator(result[i].op) && result[i + 1].op == '-') {
            result[i + 2].digit = -result[i + 2].digit;
            result[i + 1].op = '\0';
            result[i + 1].isOperator = false;
        }
    }

    //judging for positive numbers  with extra '+'
    for (int i = 0; i < result.size(); i++) {
        //judging case when -1 is in the beginning.
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

        if (processedInput[i].op == '(') {
            st.Push(processedInput[i]);
        }

        if (processedInput[i].op == ')') {
            while (!st.isEmpty() && curTop.op != '(') {
                st.Pop(_popped);
                result.push_back(_popped);
                st.getTop(curTop);
            }
            //to pop left parenthesis
            st.Pop(_popped);
            st.getTop(curTop);
        }

        if (isOperator(processedInput[i].op))
        {
            if (st.isEmpty() || curTop.op == '(') {
                st.Push(processedInput[i]);
                st.getTop(curTop);
            }
            else{
                while (!st.isEmpty() && curTop.op != '(' && lessPrior(processedInput[i].op, curTop.op)) {
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
        if (isOperator(postfixExp[i].op)) {
            st.getTop(right);
            st.Pop(right);
            st.getTop(left);
            st.Pop(left);
            if (postfixExp[i].op == '+') {
                item tmp;
                tmp.isDigit = true;
                tmp.digit = left.digit + right.digit;
                st.Push(tmp);
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

        } else {
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
    if(!leftOpPrior && !rightOpPrior)
    {
        return false;
    }
    return (leftOpPrior == rightOpPrior) ;
}

int main()
{
    expInput();
    vector<item> processedInput;
    vector<item> postfixExp;
    preProcess(oriInput,processedInput);
    infix_to_postfix(processedInput, postfixExp);
    for(int i=0;i<postfixExp.size();i++)
    {
        if(postfixExp[i].isDigit)
            cout<<postfixExp[i].digit;
        if(postfixExp[i].isOperator)
            cout<<postfixExp[i].op;
    }
    double result=0.0;
    result = calcRes(postfixExp);
    cout<<endl<<result<<endl;
}