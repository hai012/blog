




typedef struct {
    int num;
    ElementType *prior;
}ElementType;


typedef struct stack {
    ElementType *top;
    int depth;
}StackType;


void push(StackType *s, ElementType *e)
{
    e->prior = s->top;
    s -> top = e;
    ++s->depth;
}

ElementType * pop(StackType *s)
{
    //e->prior = NULL;
    s -> top = e->prior;
    --s->depth;
    return e;
}


void init_stack(StackType *s)
{
    s->top = NULL;
    s->depth = 0;
}






int seq[]={2,67,13,45,67,1,6,4};
#define NUM_COUNT sizeof(seq)


int (*catalan)[ 2 * NUM_COUNT ];
size_t catalanCount;



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