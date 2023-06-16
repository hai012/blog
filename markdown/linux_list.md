







```

LIST_HEAD(list);
//struct list_head list;
//INIT_LIST_HEAD(&list);

struct node_t {
        int cnt;
        struct list_head entry;
};

static void test_for_each()
{

    struct list_head *lhp;
    struct node_t *nodep;

    struct node_t node[10];


    for(int i=0;i<10;++i)
    {
        node[i].cnt = i;
        list_add_tail(&node[i].entry,  &list);
    }
    
    
    list_for_each(lhp,&list) {//pos_lh   head
        nodep = container_of(lhp,struct node_t,entry);
        //nodep = list_entry(lhp,struct node_t,entry);
        //list_entry就是container_of的宏
        printk("%d\n",nodep->cnt);
    }
    list_for_each_entry(nodep,&list,entry) {//pos_node  head  member
        printk("%d\n",nodep->cnt);
    }
    list_for_each_entry_reverse(nodep,&list,entry) {
    	printk("%d\n",nodep->cnt);
    }
}



1.list_for_each版本传入的pos的类型是struct list_head *，如果需要得到嵌入struct list_head的类型的变量地址例如struct node_t则需要使用container_of/list_entry。list_for_each通过struct list_head中的next一路从head头开始往后遍历直到再次等于head，遍历整个链表.
2.list_for_each_continue，通过struct list_head中的next一路从pos的下一个节点开始往后遍历，直到再次等于head。遍历部分链表.
4.list_for_each_prev则与list_for_each相反通过prev往前遍历，遍历整个链表。
5.如果要在遍历的过程中删除节点，则应使用list_for_each_safe这种代safe的版本，同时新增一个临时变量n，其类型是struct list_head*用于遍历时记录当前要删除的pos节点的下一个节点。遍历整个链表。
6.list_for_each_rcu  RCU版本的list_for_each


list_for_each(pos, head) 
list_for_each_continue
list_for_each_prev
list_for_each_prev_safe
list_for_each_safe(pos, n, head)
list_for_each_rcu
list_for_each_entry
list_for_each_entry_continue
list_for_each_entry_continue_reverse
list_for_each_entry_from
list_for_each_entry_from_reverse
list_for_each_entry_reverse
list_for_each_entry_safe
list_for_each_entry_safe_continue
list_for_each_entry_safe_from
list_for_each_entry_safe_reverse




不带后缀
版本传入的pos的类型是struct list_head *，如果需要得到嵌入struct list_head的类型的变量地址例如struct node_t则需要使用container_of/list_entry。list_for_each通过struct list_head中的next一路从head头开始往后遍历直到再次等于head，遍历整个链表.

entry后缀
版本传入的pos的类型是struct node_t *，相当于list_for_each + container_of/list_entry的版本默认遍历整个链表.
list_for_each_entry(pos, head, member)，其中member是struct node_t 中内嵌struct list_head类型的成员名即entry


continue后缀
从当前pos节点的下一个节点开始遍历

from后缀
从当前节点开始遍历

prev后缀
改变默认的遍历方向，默认使用next往后遍历，加入prev后使用prev往前遍历

safe后缀
增加临时遍历n用于保存pos下一个节点的地址，当删除pos后才能通过n继续往下一个节点去遍历，如果要遍历过程中修改则必须用safe

rcu后缀
RCU版本





list_del
list_add_tail
list_entry/container_of


list_for_each_entry_continue_rcu
list_for_each_entry_from_rcu
list_for_each_entry_lockless
list_for_each_entry_rcu
list_for_each_entry_srcu
```





# RCU



https://zhuanlan.zhihu.com/p/460032594

```text
1.rcu_read_lock()
2.p1 = rcu_dereference(p)//先把p赋给p1，然后内存屏障
3.读取数据
4.rcu_read_unlock()



1.新申请一个节点并准备好数据
2.rcu_assign_pointer(old,new)//先内存屏障;然后把new赋给old
3.synchronize_rcu()





链表相关函数对RCU进行了封装：
rcu_read_lock();
list_for_each_entry_rcu(pos, head, member) {
    // do something with `pos`
}
rcu_read_unlock();


p = search_the_entry_to_update();//找到链表中一个要替换的节点
q = kmalloc(sizeof(*p), GFP_KERNEL);//新申请一个节点
*q = *p;//拷贝要替换的节点的数据到新的节点
q->field = new_value;//加入修改
list_replace_rcu(&p->list, &q->list);//进行替换
synchronize_rcu();//等待所有读者调用完rcu_read_unlock离开临界区
kfree(p);//释放老节点
```