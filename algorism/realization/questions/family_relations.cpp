#include<iostream>
#include<list>
#include<queue>


using namespace std;



struct btNode {
    int value;
    btNode* left;
    btNode* right;
};

int p1;
int p2;
struct btNode *p1_node=NULL;
struct btNode *p2_node=NULL;

void create_tree_bfs(struct btNode * root,  list<pair<int,int>> & info)
{
    queue<struct btNode *> q;
    q.push(root);
    struct btNode * cur;

    while(!q.empty()) {
        cur = q.front();
        q.pop();

        cur->left = NULL;
        for(auto it=info.begin();it!=info.end();++it) {
            if( cur->value==(*it).first ) {
                cur->left = new struct btNode;
                cur->left->value = (*it).second;
                info.erase(it);
                q.push(cur->left);
                break;
            }
        }
            
        cur->right = NULL;
        for(auto it=info.begin();it!=info.end();++it) {
            if( cur->value==(*it).first ) {
                cur->right = new struct btNode;
                cur->right->value = (*it).second;
                info.erase(it);
                q.push(cur->right);
                break;
            }
        }

        if(cur->value == p1)
            p1_node = cur;
        if(cur->value == p2)
            p2_node = cur;
    }
}

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

        cout<<cur->value<<' ';

        if(cur->left)
            q.push(cur->left);
        if(cur->right)
            q.push(cur->right);
    }
}

int get_depth(struct btNode * node)
{
    if(node==NULL) {
        return 0;
    } else {
        int left_depth = get_depth(node->left);
        int right_depth = get_depth(node->right);
        return  ((left_depth > right_depth)? left_depth  : right_depth )  + 1 ;
    }
}

bool is_a_contains_b(struct btNode * a,struct btNode * b)
{
    if(a==NULL)
        return false;
    if(a->left) {
        if(a->left == b || is_a_contains_b(a->left, b)) {
            return true;
        }
    }
    if(a->right) {
        if(a->right == b || is_a_contains_b(a->right, b)) {
            return true;
        }
    }
    return false;
}

int main(int argc, char **argv)
{
    int count;

    if(!(cin >> count)) exit(-1);
    if(!(cin >> p1)) exit(-1);
    if(!(cin >> p2)) exit(-1);

    --count;
    list<pair<int,int>> info;
    for(int i=0;i<count;++i) {
        int parent;
        if(!(cin >> parent)) exit(-1);
        int child;
        if(!(cin >> child)) exit(-1);
        info.push_back(make_pair(parent,child));
    }


    //for(auto p:info)
    //    cout<<p.first<<' '<<p.second<<endl;

    struct btNode * root = new struct btNode;
    root->value = 0;
    create_tree_bfs(root, info);

    //level_raversal(root);

    int p1_depth = get_depth(p1_node);
    int p2_depth = get_depth(p2_node);


    if(p1_depth == p2_depth) {
        cout<<"2 0"<<endl;
    } else if(p1_depth > p2_depth) {
        if(is_a_contains_b(p1_node,p2_node)) {
            cout<<  p1_depth-p2_depth  << ' ' << p1_depth-p2_depth <<endl;
        } else {
            cout<<  p1_depth-p2_depth + 2 << ' ' << p1_depth-p2_depth <<endl;
        }
    } else {
        if(is_a_contains_b(p2_node,p1_node)) {
            cout<<  p2_depth-p1_depth << ' ' << p1_depth-p2_depth <<endl;
        } else {
            cout<<  p2_depth-p1_depth + 2 << ' ' << p1_depth-p2_depth <<endl;
        }
    }
    return 0;
}