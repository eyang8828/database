#include "../!include/LinkList/Linked_List_function.h"
#include <cassert>
#include <iostream>

template <class T>
class Queue{
public:
    Queue():_head(NULL), _tail(NULL){}

    ~Queue();
    Queue(const Queue<T>& other);
    Queue<T> &operator =(const Queue& rhs);

    int size();
    void push(T item);
    T pop();
    bool empty();
    T front();

    friend ostream& operator <<(ostream& outs, const Queue& q){
        return print_list(q._head,outs);
    }

private:
    int length = 0;
    node<T>* _head;
    node<T>* _tail;
};

//return the item store in head and remove it from the queue
template<class T> T Queue<T>::pop(){
    length--;
    node<T>* temp = _head;
    T item = _head->_item;
    _head = _head->_next;
    delete_head(temp);
    return item;
}

template<class T> void Queue<T>::push(T item){
    node<T> *temp = new node<T>(item);
    length++;
    //if it's empty insert at head
    if(_head == nullptr){
        _head = temp;
        _tail = temp;
    //other than that insert that the end of the queue
    }else{
        _tail = insert_after(_head,_tail,item);
    }
}
//if head is empty return true else return false
template<class T> bool Queue<T>::empty(){
    if(_head != nullptr){
        return false;
    }
    return true;
}

//return the item store in head without delete it
template<class T> T Queue<T>::front(){
    T item = _head->_item;
    return item;
}

template<class T> Queue<T> &Queue<T>::operator=(const Queue<T> &rhs){
    copy_list(_head,rhs);
}

template<class T> Queue<T>::~Queue(){
    delete_all(_head);
}

template<class T> Queue<T>::Queue(const Queue<T> &other){
    _head=other._head;
    _tail = other._tail;
    length = other.length;

}

template<class T> int Queue<T>::size(){
    return length;
}