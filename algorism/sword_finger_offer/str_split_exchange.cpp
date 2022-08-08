#include<iostream>
#include<string>
using namespace std;

char input[]= {"s 2r2 3233 fsaf"};



void reserse(char *str)
{
	char *next = strtok(str," ");//first  strtok is str
	if(next) {
		next_reserse(NULL);
		cout<<next<<' ';
	}
}


int main(int argc,char *argv[])
{
	start_reserse(input);
	cout<<endl;
	return 0;
}




