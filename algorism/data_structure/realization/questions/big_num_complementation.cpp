#include<iostream>
#include<vector>
//#include<algorithm>



using namespace std;


ssize_t base;
ssize_t divisor;
ssize_t dividend;


ssize_t exp_func(ssize_t x, ssize_t y) {
    if(y==0)
        return 1;
    else
        return x * exp_func(x, y-1);
}

int main(int argc,char **argv)
{

    //cout << "wait for input"<<endl;


    if(!(cin >> base))
        return -1;

    if(!(cin >> divisor))
        return -1;
    
    string base_str;
    if(!(cin >> base_str))
        return -1;

    dividend = 0;
    for(int i=0; i<base_str.size(); ++i) {
        ssize_t num = (ssize_t) (base_str[i]-'0');
        if( num<0 ) {
            dividend = 0;
            break;
        } else if( num <= 9 ) {
            dividend += num * exp_func(base,i);
            continue;
        }

        ssize_t alpha = (ssize_t) (base_str[i]-'a');
        if( alpha < 0) {
            dividend = 0;
            break;
        } else if( alpha <= 25 ) {
            dividend += (alpha + 10) * exp_func(base,i);
            continue;
        }

        dividend = 0;
        break;
    }


    cout << dividend%divisor << endl;
    return 0;
}