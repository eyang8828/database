#ifndef LIST_H
#define LIST_H
#include "../../!include/LinkList/Linked_List_function.h"
#include <iostream>
using namespace std;



template <class T>
class List
{
public:
    //Iterated List
    class Iterator{
    public:
        friend class List;
        Iterator(){
            _ptr = NULL;
        }

        Iterator(node<T>* p):_ptr(p){}

        operator bool();

        T& operator *(){
            return _ptr->_item;
        }
        T* operator ->()
        {
            T* temp = &_ptr->_item;
            return temp;
        }

        bool is_null(){
            if (_ptr == NULL){
                return true;
            }
            return false;
        }

        friend bool operator !=(const Iterator& left, const Iterator& right)
        {
            if (left._ptr != right._ptr)
            {
                return true;
            }
            return false;
        }
        friend bool operator ==(const Iterator& left, const Iterator& right)
        {
            if (left._ptr == right._ptr)
            {
                return true;
            }
            return false;
        }

        Iterator& operator++()
        {
            _ptr = _ptr->_next;
            return *this;
        }

        friend Iterator operator++(Iterator& it, int unused)
        {
            Iterator temp(it);
            ++it;
            return temp;
        }
        //use the prev address in the node
        Iterator& operator--()
        {
            _ptr = _ptr->_prev;
            return *this;
        }
        friend Iterator operator--(Iterator& it, int unused)
        {
            Iterator temp(it);
            --it;
            return temp;
        }
    private:
        node<T>* _ptr;
    };

//=====================================================================

    List();
    ~List();

    //calling insert head from List function
    Iterator Inserthead(T i){
        return InsertHead(i);
    }
    //calling insert after
    Iterator Insertafter(T i, Iterator n1){
        return InsertAfter(i, n1._ptr);
    }
    Iterator Insertbefore(T i, Iterator n1){
        return InsertBefore(i, n1._ptr);
    }
    //delete the top node and return the item
    T Deletehead(){
        return DeleteHead();
    }

    bool Deletenode(T target){
        return DeleteNode(target);
    }
    void Print() const;
    bool Empty() const
    {
        return is_empty();
    }
    Iterator Begin() const
    {
        return begin();
    }
    Iterator End() const
    {
        return NULL;
    }
    Iterator Lastnode() const
    {
        return end();
    }
    friend ostream& operator <<(ostream& outs, const List<T>& l)
    {
        print_list(l.head_ptr);
        return outs;
    }
private:
    node<T>* head_ptr;
    node<T>* InsertHead(T item);
    node<T>* InsertAfter(T item, node<T>* n1);
    node<T>* InsertBefore(T item, node<T>* n1);
    T DeleteHead();
    bool DeleteNode(T target);
    node<T>* Search(const T& key);
    node<T>* Prev(node<T>* n1);
    node<T>* begin() const;
    bool is_empty() const;
    node<T>* end() const;
};


template<class T>
List<T>::List(){
    head_ptr = NULL;
}

template<class T>
List<T>::~List<T>(){
    delete_all(head_ptr);
}


template <class T>
node<T>* List<T>::InsertHead(T item){
    return insert_head(head_ptr, item);
}

template <class T>
node<T>* List<T>::InsertAfter(T i, node<T>* n1)
{
    return insert_after(head_ptr,n1, i);
}

template <class T>
node<T>* List<T>::InsertBefore(T item, node<T>* n1){
    return insert_before(head_ptr, n1, item);
}

template <class T>
T List<T>::DeleteHead(){
    return delete_head(head_ptr);
}

template <class T>
bool List<T>::DeleteNode(T target){
    return delete_node(head_ptr, target);
}

template<class T>
void List<T>::Print() const{
    print_list(head_ptr);
}

template<class T>
node<T>* List<T>::begin() const{
    return head_ptr;
}

template<class T>
bool List<T>::is_empty() const{
    return (head_ptr==nullptr);
}

template<class T>
node<T>* List<T>::end() const{
    return last_node(head_ptr);
}
 
#endif // LIST_H