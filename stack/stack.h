#include "../!include/LinkList/Linked_List_function.h"
#include <cassert>
#include <iostream>

template <class T>
class Stack{
public:
    Stack():_top(NULL){}

    ~Stack();
    Stack(const Stack<T>& other);
    Stack<T>& operator =(const Stack<T>& rhs);

    void push(T item);
    T pop();
    T top();
    bool empty();
    friend ostream& operator << (ostream& outs, const Stack& s){
        return print_list(s._top, outs);
    }
private:
    node<T>* _top;
};

template<class T> T Stack<T>::pop(){
    node<T> *temp;
    assert(_top != nullptr);
    temp = _top;
    _top = _top->_next;
    T item = temp->_item;
    delete_head(temp);
    return item;
}

template<class T> void Stack<T>::push(T item){
    node<T>* temp = new node<T>(item);
    temp->_next = _top;
    _top = temp;
}

template<class T> T Stack<T>::top(){
    T item = _top->_item;
    return item;
}

template<class T> bool Stack<T>::empty(){
    if(_top==NULL){
        return true;
    }
    return false;
}

template<class T> Stack<T> &Stack<T>::operator=(const Stack<T> &rhs){
    copy_list(_top, rhs);
}

template<class T> Stack<T>::~Stack(){
    print_list(_top);
}