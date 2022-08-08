#include<iostream>
#include<vector>
using namespace std;

vector<vector<int>> info;

int main(int argc,char *argv[])
{
	int n,m;
	for (int i=0;i<n;++i){
		vector<int> line;
		for(int j=0;j<m;++j){
			int num;
			cin>>num;
			line.push_back(num);
		}	
		info.push_back(line);
	}
	
	//Binary search
	for(;;) {

	}
	


	//cout<<"not found"<<endl;

	return 0;
}