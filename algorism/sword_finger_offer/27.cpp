#include<iostream>
#include<queue>
#include<unorderedmap>
using namespace std;

class Node {
public:
    int val;
    Node* next;
    Node* random;
    
    Node(int _val) {
        val = _val;
        next = NULL;
        random = NULL;
    }
};

Node* copyRandomList(Node* head) {
    if(head==NULL)
        return NULL;

}

int main(int argc,char *argv[])
{
	return 0;
}