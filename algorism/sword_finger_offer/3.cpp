#include<iostream>
#include<list>
using namespace std;

list<int> info;

int main(int argc,char *argv[])
{
	int num;
	while(cin>>num) {
		info.push_back(num);
	}
	
	for(;;) {
		if(info.empty()) {
			break;
		} else {
			int select=info.front();
			for(list<int>::iterator it = ++info.begin();it!=info.end(); ++it){
				if(*it == select) {
					cout<<select<<endl;
					return 0;
				}
			}
			info.pop_front();
		}
	}
	


	//cout<<"not found"<<endl;

	return 0;
}