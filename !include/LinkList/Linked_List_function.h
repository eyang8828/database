#ifndef LINKED_LIST_FUNCTION_H_
#define LINKED_LIST_FUNCTION_H_
#include <iostream>
#include <cassert>

using namespace std;
template <class T>
struct node{
    T _item;
    node<T>* _next;
    node<T>* _prev;
    node(T v){
        _item = v;
        _next = nullptr;
        _prev = nullptr;
    }
    node(){
        _next = nullptr;
        _prev = nullptr;
    }
    friend ostream& operator << (ostream& outs, const node<T>& print_me){
        outs<<"["<<print_me._item<<"]->";
        return outs;
    }
};

//initializes head to NULL
template <class T>
node<T>* init_head(node<T>* &head){
    head = nullptr;
    return head;
};

//deletes all the nodes in the list
template<class T>
void delete_all(node<T>*&head){
    node<T>* deletethis = nullptr;
    while(head != nullptr){
        deletethis = head;
        head = head->_next;
        delete deletethis;
    }
};

//true if head is NULL, false otherwise.
template <class T>
bool empty(const node<T>* head){
    if(head == nullptr){
        return true;
    }
    return false;
};

//makes a copy of the list, returns a pointer to the last node:
template <class T>
node<T>* copy_list(const node<T>* head, node<T>* & cpy){
    assert(head != nullptr);
    node<T> * current = head->_next->_prev;
    current->_next = head->_next;
    node<T> * tail;
    while(current!= nullptr){
        node<T>*temp = new node<T>(current->_item);
        temp->_next = cpy;
        cpy = temp;
        current = current->_next;
    }
    return tail;
};

//insert at the beginning of the list:
template <class T>
node<T>* insert_head(node<T>* &head, T item){
    node<T>* temp = new node<T>(item);
    if (head == nullptr){
        head = temp;
        return temp;
    }
   
    head->_prev = temp; temp->_item = item;
    temp->_next = head;
    head = temp;
    return temp;
};

//insert_after: if after is NULL, inserts at head
template <class T>
node<T>* insert_after(node<T>* &head, node<T>* after, const T& item){
    node<T>* in_node;
    if(after == nullptr){
        in_node =insert_head(head, item);
        return in_node;
    }
    if(after->_next == nullptr){
        in_node = new node<T>(item);
        in_node->_next = nullptr;
        in_node->_prev = after;
        after->_next = in_node;
        return in_node;
    }
    in_node= new node<T>(item);
    node<T>*before = after->_next;
    in_node->_next = before;
    in_node->_prev = after;
    before->_prev = in_node;
    after->_next = in_node;
    return in_node;
};

//delete the node at the head of the list, reuturn the item:
template <class T>
T delete_head(node<T>* &head){

    node<T>* deletethis = head;
    T item = head->_item;

    if(head->_next == nullptr){
        node<T>* temp = new node<T>();
        head = temp;
        delete deletethis;
        return item;
    }
    
    node<T>* temp = deletethis->_next;
    temp->_prev = nullptr;
    head = temp;
    delete deletethis;
    return item;
};

//delete the node at the head of the list, reuturn the item:
template <class T>
bool delete_node(node<T>* &head,const T& target){

    node<T>* deletethis= head;
    node<T>* prev;
    T item = head->_item;
    if(item == target){
        delete_head(head);
        return true;
    }

    while(item != target && deletethis!= nullptr){
        prev = deletethis;
        deletethis = deletethis->_next;
        item = deletethis->_item;
        if(item == target){

            if(deletethis->_next==nullptr){
                prev->_next = nullptr;
            }else{
                prev->_next = deletethis->_next;
                node<T>* temp = deletethis->_next;
                temp->_prev = prev;
            }
            delete deletethis;
            return true;
        }
    }
    return false;

};

//print the list and return outs
template<class T>
ostream& print_list(node<T>* head, ostream& outs=cout){
    // cout<<"H->";
    // for (node<T>* walker = head; walker!= nullptr; walker=walker->_next){
    //     cout<<*walker;
    // }
    // cout<<"|||";
    return outs;
};

template<class T>
node<T>* last_node(node<T>* head){
    while(head->_next!= nullptr){
        head = head->_next;
    }
    return head;
}

#endif // LINKED_LIST_FUNCTION_H_