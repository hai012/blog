#include<iostream>
#include<vector>

using namespace std;
vector<int> vec;

int main(int argc,char **argv)
{
    for(;;) {
        cout << "wait for input"<<endl;
        int s;
        if(cin >> s)
            vec.push_back(s);
        else
            break;
    }

    int count=100;
    for (vector<int>::iterator it = vec.begin(); it != vec.end();) {
        if (*it>0) {
            //it = vec.erase(it);
            //it = vec.insert(it,T);
            //it = vec.insert(it,n,T); n是要插入的T的个数。
            /*不能使用vec.push_back和vec.pop_back方法，
              可以使用 vec.insert(vec.end(),T) 和
              vec.erase(vec.end(),T) 来替代
              vector在非尾部增删元素时比较慢。
            */
        } else {
            it++;
        }
    }

        for(auto element : vec)
            cout << element << ' ';
        cout << endl;
    return 0;
}