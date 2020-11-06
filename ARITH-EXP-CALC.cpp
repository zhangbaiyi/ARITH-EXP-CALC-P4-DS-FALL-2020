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

    //positive sign is before a left parenthesis.
    for(int i = 0;i<result.size();i++)
    {
        if(i-1>=0 && i+1!= result.size() && result[i+1].isOperator && result[i+1].op =='(' && result[i].isOperator && result[i].op =='+' && result[i-1].isOperator &&isOperator(result[i-1].op)){
            result[i].isOperator = false;
            result[i].op = '\0';
        }
    }


    //negative sign is before a left parenthesis.
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
//    cout << "请输入一个中缀表达式" << endl;
    cout << "Please enter '=' at the end of expresion" << endl;
//    cout << "请在结尾输入等号 = " << endl;
    cout << "Please note that this program can only calculate integer" << endl;
//    cout << "请注意本程序只计算整数，小数点会被视为Bad Input" << endl;
    cout << "The operators can be one of below:" << endl;
//    cout << "请注意本程序的运算符仅限以下几种" << endl;
    cout << "'+','-','*','/','%','^','(',')'" << endl;
    cout << R"(The Blankspace ends the process of input. For example, "2 +5/2" is read as "2".)" << endl;
//    cout << "空格会视为输入结束。例如，\"2 +5/2\" 会被读作 \"2\" " << endl;
    cout << "enter a 'q' can quit the program." << endl;
}

int main() {
    bool continueFlag = true;
    while (continueFlag) {
        menu();
        continueFlag = expInput();
        vector<item> processedInput;
        vector<item> postfixExp;
        preProcess(oriInput, processedInput);
        infix_to_postfix(processedInput, postfixExp);

        if (continueFlag) {
            cout << "The postfix expression is:" << endl;
//            cout << "后缀表达式为:" << endl;
            for (int i = 0; i < postfixExp.size(); i++) {
                if (postfixExp[i].isDigit)
                    cout << postfixExp[i].digit;
                if (postfixExp[i].isOperator)
                    cout << postfixExp[i].op;
            }

            double result = calcRes(postfixExp);
            cout << endl;
            cout << "The result is:" << endl;
//            cout << "最终结果为:" << endl;
            cout << result << endl;
        }
    }
    getchar();
    return 0;
}