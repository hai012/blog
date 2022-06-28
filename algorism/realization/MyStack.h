
template <typename T>
class ElementType {
    public:
        T element;
        ElementType<T> *prior;
        ElementType() = default;
        ElementType(T e) : element(e) {};
};

template <typename T> class MyStack {

    public:
        ElementType<T> *top;
        int depth;
        MyStack() {
            top = NULL;
            depth = 0;
        };
        void push(ElementType<T> *e)
        {
            e->prior = top;
            top = e;
            ++depth;
        };
        ElementType<T> * pop()
        {
            //e->prior = NULL;
            ElementType<T> *e = top;
            if(e) {
                top = top->prior;
                --depth;
                return e;
            } else {
                return NULL;
            }
        }
        ElementType<T> * getTop()
        {
            return top;
        }
        void delTop()
        {
            //e->prior = NULL;
            if(top) {
                top = top->prior;
                --depth;
            }
        }
};