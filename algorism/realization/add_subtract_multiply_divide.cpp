
#include <string>
#include <vector>
#include <iostream>
#include "MyStack.h"

using namespace std;

void split_ops_num(const char * char_oder, vector<string *> &buf)
{
    string input(char_oder);
    string *num, *ops;


    if(input[0] >= '0' && input[0] <= '9') {
        num = new string(1, input[0]);
        if(!num)
            exit(-1);
        buf.push_back(num);
    } else if(input[0]=='(') {
        ops = new string(1,input[0]);
        if(!ops)
            exit(-1);
        buf.push_back(ops);
    }else if(input[0]=='-'){
        input.insert(0, 1, '0');
        num = new string(1,'0');
        if(!num)
            exit(-1);
        buf.push_back(num);
    }else {
        exit(-1);
    }


    for(int i=1;input[i]!='\0';++i) {
        
        switch(input[i]) {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '.':
                if(input[i] == '.' && input[i-1] == '.')
                    exit(-1);
                if( (input[i-1] < '0' || input[i-1] > '9') && input[i-1] != '.') {
                    if( num->back()=='-' ) {

                    } else {
                        num = new string();
                        if(!num)
                            exit(-1);
                        buf.push_back(num);
                    }
                }
                num->append(1,input[i]);
                break;
            case '-':
                if(input[i-1]=='(' ) {
                    num = new string(1,'-');
                    if(!num)
                        exit(-1);
                    buf.push_back(num);
                    break;
                }
            case '+':
            case '*':
            case '/':
            case '(':
            case ')':
                ops = new string(1,input[i]);
                if(!ops)
                    exit(-1);
                buf.push_back(ops);
                break;
            default:
                exit(-1);
        }
    }
}

void to_suffix(const vector<string *> &input,vector<string *> &output)
{
    class MyStack<string *> my_stack;
    ElementType<string *> *e;

    for(int i=0; i < input.size(); ++i) {
        switch( (input[i])->back() ) {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                    output.push_back(input[i]);
                    break;
            case '+':
            case '-':
                    for(;;) {
                        e = my_stack.pop();
                        if(e == NULL )
                            break;
                        if((*(e->element))[0] == '(') {
                            my_stack.push(e);
                            break;
                        }
                        output.push_back(e->element);
                        delete e;
                    }
                    e = new ElementType<string *>(input[i]);
                    if(!e) {
                        printf("new ElementType<string *> failed in push\n");
                        exit(-1);
                    }
                    my_stack.push(e);
                    break;
            case '*':
            case '/':
                    for(;;) {
                        e = my_stack.pop();
                        if(e == NULL)
                            break;
                        if( (*(e->element))[0] == '(' || (*(e->element))[0] == '+' || (*(e->element))[0] == '-' ) {
                            my_stack.push(e);
                            break;
                        }
                        output.push_back(e->element);
                        delete e;
                    }
                    e = new ElementType<string *>(input[i]);
                    if(!e) {
                        printf("new ElementType<string *> failed in push\n");
                        exit(-1);
                    }
                    my_stack.push(e);
                    break;
            case '(':
                    e = new ElementType<string *>(input[i]);
                    if(!e) {
                        printf("new ElementType<string *> failed in push\n");
                        exit(-1);
                    }
                    my_stack.push(e);
                    break;
            case ')':
                    for(;;) {
                        e = my_stack.pop();
                        if(e == NULL)
                            exit(-1);//未找到栈中对应的'('
                        else if (  (*(e->element))[0] == '(' ) {
                            break;
                        }
                        output.push_back(e->element);
                        delete e;
                    }
                    break;
            default:
                    exit(-1);
        }
    }
    for(;;) {
        e = my_stack.pop();
        if(!e)
            break;
        output.push_back(e->element);
        delete e;
    }
}



double calc_with_suffix(const vector<string *> &buf) {
    class MyStack<double> my_stack;
    ElementType<double> *e1,*e2,*e3;
 
    for(int i=0; i<buf.size(); ++i) {

        /*cout <<"class MyStack<double> dump:";
        for(e1 = my_stack.top; e1 !=NULL; e1=e1->prior) {
            cout << e1->element << "   ";
        }
        cout <<'\n';*/

        switch( (buf[i])->back() ) {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                e1 = new ElementType<double>(stod( *(buf[i]) ));
                if(!e1)
                    exit(-1);
                my_stack.push(e1);
                break;
            case '+':
                e1 = my_stack.pop();
                if( e1 == NULL)
                    exit(-1);
                e2 = my_stack.pop();
                if( e2 == NULL)
                    exit(-1);
                e3 = new ElementType<double>((e2->element + e1->element));
                if(!e3)
                    exit(-1);
                my_stack.push(e3);
                break;
            case '-':
                e1 = my_stack.pop();
                if( e1 == NULL)
                    exit(-1);
                e2 = my_stack.pop();
                if( e2 == NULL)
                    exit(-1);
                e3 = new ElementType<double>((e2->element - e1->element));
                if(!e3)
                    exit(-1);
                my_stack.push(e3);
                break;
            case '*':
                e1 = my_stack.pop();
                if( e1 == NULL)
                    exit(-1);
                e2 = my_stack.pop();
                if( e2 == NULL)
                    exit(-1);
                e3 = new ElementType<double>((e2->element * e1->element));
                if(!e3)
                    exit(-1);
                my_stack.push(e3);
                break;
            case '/':
                e1 = my_stack.pop();
                if( e1 == NULL)
                    exit(-1);
                e2 = my_stack.pop();
                if( e2 == NULL)
                    exit(-1);
                e3 = new ElementType<double>((e2->element / e1->element));
                if(!e3)
                    exit(-1);
                my_stack.push(e3);
                break;
            default:
                exit(-1);
        }
    }
    e1 = my_stack.pop();
    if(e1 == NULL)
        exit(-1);
    return e1->element;
}

int main(int argc,char **argv)
{
    if(!argv[1]) {
        printf("usage: x*y  x/y  x+y x-y ()");
        exit(-1);
    }
    vector<string *> buf;
    vector<string *> buf2;

    split_ops_num(argv[1], buf);

    cout<<"split_ops_num:\n";
    for(string *ptr : buf)
        cout << *ptr << '\n';

    to_suffix(buf,buf2);
    cout<<"to_suffix:\n";
    for(string *ptr : buf2)
        cout << *ptr << '\n';


    cout<<"calc_with_suffix:\n"
        <<calc_with_suffix(buf2)
        <<'\n';

    return 0;
}


