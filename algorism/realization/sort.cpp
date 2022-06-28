#include<iostream>
#include<vector>

using namespace std;
vector<int> info;


#define Select_mini_max_2


#ifdef Cpp_sort
#include<algorithm>
#endif

int main(int argc,char **argv)
{
    for(;;) {
        cout << "wait for input"<<endl;
        int s;
        if(cin >> s)
            info.push_back(s);
        else
            break;
    }

    for(auto element : info)
        cout << element << ' ';
    cout << endl;

#ifdef Exhaustion_exchange
//短线，遇到满足条件的都进行一次交换
    for(int i=0; i<info.size(); ++i) {
        for(int j=i+1; j<info.size(); ++j) {
            if( info[i] > info[j] ) {
                int tmp = info[i];
                info[i] = info[j];
                info[j] = tmp;
            }
        }
    }
#endif


#ifdef Sample_Select_sort
//长线，遇到满足条件的都记录一下，找到最极限的，在进行下一次外层循环前交换
    vector<int> result;
    while(info.size() > 0) {
        int tmp=info[0];
        vector<int>::iterator it_tmp = info.begin();
        for(vector<int>::iterator it = info.begin()+1; it!=info.end(); ++it) {
            if( *it < tmp ) {
                tmp = *it;
                it_tmp = it;
            }
        }
        result.push_back(tmp);
        info.erase(it_tmp);//vector在非尾部删除元素很慢，建议把info的类型改成list
    }
    info = result;
#endif

#ifdef Sample_Select_sort_2
//长线，遇到满足条件的都记录一下，找到最极限的，在进行下一次外层循环前交换
    for(vector<int>::iterator it1=info.begin(); it1!=info.end(); ++it1) {
        vector<int>::iterator it_tmp=it1;
        for(vector<int>::iterator it2 = it1+1; it2!=info.end(); ++it2) {
            if( *it2 < *it_tmp ) {
                it_tmp = it2;
            }
        }
        if(it_tmp != it1) {
            int tmp= *it_tmp;
            *it_tmp = *it1;
            *it1 = tmp;
        }
    }
#endif









#ifdef Bubbling
    int is_odered;
    for(int i=0; i<info.size(); ++i) {
        is_odered = 1;
        for(int j=info.size()-1; j>i; --j) {
            //cout << i << ' ' << j << ' ' <<endl;
            if( info[j-1] > info[j] ) {
                int tmp = info[j];
                info[j] = info[j-1];
                info[j-1] = tmp;
                is_odered = 0;
            }
        }
        if(is_odered)
            break;
    }
#endif








#ifdef Cpp_sort
    sort(info.begin(), info.end());
/*
sort(info.begin(), info.end());使用默认的升序排序方法，一般用于基本数据类型

如果需要 降序 或 自定义比较标准(例如对象之间进行比较)
sort(info.begin(), info.end(),compare);
bool compare(const &T a, const &T b)
如果返回 true 则a排在b之前
如果返回 false 则a排在b之后
bool compare(int a,int b)
{ 
    return a < b; //升序排列，如果改为return a>b，则为降序 
}
*/
#endif



    for(auto element : info)
        cout << element << ' ';
    cout << endl;
    return 0;
}