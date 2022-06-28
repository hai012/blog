









int seq[]={2,67,13,45,67,1,6,4};
#define NUM_COUNT sizeof(seq)


int (*catalan)[ 2 * NUM_COUNT ];
size_t catalanCount;


#if 0
size_t calcFactorial(size_t n)
{
    size_t product = 1;
    for(size_t i = 2; i < n; ++i) {
        product = product * i;
    }
    return product;
}
size_t getCatalanCount(size_t pair_count)
{
    return calcFactorial(2*pair_count) / ( calcFactorial(pair_count)^2  * (pair_count+1) );
}
#else
size_t getCatalanCount(size_t pairs)
{
    if(pairs==1)
        return 1;
    else {
        return func(pairs-1)*(4*pairs-2)/(pairs+1);
    }
}
#endif

https://blog.csdn.net/iteye_4389/article/details/82453266?spm=1001.2101.3001.6661.1&utm_medium=distribute.pc_relevant_t0.none-task-blog-2%7Edefault%7ECTRLIST%7Edefault-1-82453266-blog-5705641.pc_relevant_aa&depth_1-utm_source=distribute.pc_relevant_t0.none-task-blog-2%7Edefault%7ECTRLIST%7Edefault-1-82453266-blog-5705641.pc_relevant_aa&utm_relevant_index=1
https://blog.csdn.net/gstc110/article/details/7583655?spm=1001.2101.3001.6650.3&utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7Edefault-3-7583655-blog-47379789.pc_relevant_default&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7Edefault-3-7583655-blog-47379789.pc_relevant_default&utm_relevant_index=6


void func(vector<char>kind,int count[],int n)
{
    /*if(kind.size = 2 * n) {
        vector<char>::iterator iter;
        for(iter=kind.begin();iter!=kind.end();iter++) {
            cout<<(*iter)<<" ";
        }
        cout<<endl;
    }*/

    if(count[0]>=1) {
        kind.push_back('(');
        count[0]--;
        func(kind,count,n);
        count[0]++;
        kind.pop_back();
    }

    if((count[1]>=1) && (count[1]>count[0]))
    {
        kind.push_back(')');
        count[1]--;
        func(kind,count,n);
        count[1]++;
        kind.pop_back();
    }
}





int main(int argc,char **argv)
{




    return 0;
}










#if 0
int main(int argc,char **argv)
{
    StackType *s = malloc(sizeif(StackType));
    if(!s) exit(-1);
    init_stack(s);



    count = 2 * NUM_COUNT;
    for(int i=0;i<count;++i) {//出栈操作次数加上入栈操作次数

        e->num = seq[i];
        if() {
            ElementType* e = (ElementType *)malloc(sizeif(ElementType));
            if(!e) exit(-1);
            push(s,e);
        }
        else { 
            ElementType* e = pop(s);
            printf("%d ", e->num);
            free(e);
        }
    }

    return 0;
}
#endif