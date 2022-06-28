#include<iostream>

#include<stack>
#include<queue>

using namespace std;


struct btNode {
	char value;//结点值
	btNode *left;//左子树指针
	btNode *right;//右子树指针
};


// preorder traversal 
// inorder traversing
// Postorder-traversal algorithm


void preorder_creat_btNode(btNode * &node)//创建二叉树
{
	char ch;
	ch = getchar();//从键盘输入一串字母,依次读取,直至换行键
	if (ch == '#')
		node = NULL;
	else {
		node = new btNode;
		node->value = ch;
		preorder_creat_btNode(node->left);
		preorder_creat_btNode(node->right);
	}
}

void preorder_traversal(btNode *node)
{
	if (node == NULL)
		return;
	else {
			cout << node->value;
			preorder_traversal(node->left);
			preorder_traversal(node->right);
	}
}
void inorder_traversal(btNode *node)
{
    if(node==NULL)
        return;
    else {
        inorder_traversal(node->left);
        cout<<node->value;
        inorder_traversal(node->right);
    }
}
void post_traversal(btNode *node)
{
    if(node==NULL)
        return;
    else {
        post_traversal(node->left);
        post_traversal(node->right);
        cout<<node->value;
    }
}




void preorder_traversal_2(btNode *root)
{
    if(root == NULL){
        return ;
    }
    btNode *cur = root;
    stack<btNode *> s;
    while(!s.empty() || cur!=NULL) {
        while(cur!=NULL) {
            cout<<cur->value;
            s.push(cur);
            cur = cur->left;
        }

        cur = s.top();
        s.pop();
        cur=cur->right;
    }
}
void inorder_traversal_2(btNode *root)
{
    if(root == NULL){
        return ;
    }
    btNode *cur = root;
    stack<btNode *> s;
    while(!s.empty() || cur!=NULL) {
        while(cur!=NULL) {
            s.push(cur);
            cur = cur->left;
        }
        
        cur=s.top();
        s.pop();

        cout<<cur->value;

        cur=cur->right;
    }
}

void post_traversal_2(btNode *root)
{
    if(root == NULL){
        return ;
    }
    btNode *cur = root;
    stack<btNode *> s;
    btNode *pre = root;
    while(!s.empty() || cur!=NULL) {
        while(cur!=NULL) {
            s.push(cur);
            cur = cur->left;
        }
        
        cur=s.top();
        if(cur->right==NULL || cur->right==pre) {
            s.pop();
            cout<<cur->value;
            pre=cur;
            cur=NULL;
        } else {
            cur=cur->right;
        }
    }
}

//breadth-first search, bfs, 广度优先遍历
void level_raversal(btNode *root)
{
    if(root == NULL){
        return ;
    }
    queue<btNode *> q;
    q.push(root);
    btNode *cur;
    while(!q.empty()) {
        cur = q.front();
        q.pop();

        cout<<cur->value;

        if(cur->left)
            q.push(cur->left);
        if(cur->right)
            q.push(cur->right);
    }
}

int node_count(btNode* node)
{
	if (node)
		return 1 + node_count(node->left) + node_count(node->right);
	else {
		return 0;
	}
}

int main(int argc, char **argv)
{
    btNode *root = NULL;
    preorder_creat_btNode(root);

    cout <<"pre:"<<endl;
    preorder_traversal(root);
    cout <<endl;
    preorder_traversal_2(root);
    cout <<endl;

    cout <<"in:"<<endl;
    inorder_traversal(root);
    cout <<endl;
    inorder_traversal_2(root);
    cout <<endl;

    cout <<"post:"<<endl;
    post_traversal(root);
    cout <<endl;
    post_traversal_2(root);
    cout <<endl;

    cout <<"level:"<<endl;
    level_raversal(root);
    cout <<endl;
    return 0;
}