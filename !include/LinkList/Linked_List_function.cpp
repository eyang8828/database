#include "Linked_List_function.h"
using namespace std;


int main(){
    node<int>* n1 = new node<int>(10);
    insert_head(n1,20);
    insert_head(n1,30);
    insert_head(n1,40);
    print_list(n1);
    node<int>*n2 = last_node(n1);
    cout<<endl<<*n2<<endl;
    print_list(n1)<<endl;
    delete_node(n1,40);
    print_list(n1);
}




















// template<class T> node<T> *init_head(node<T> *&head){
//     head = nullptr;
// }

// template<class T> void delete_all(node<T> *&head){
//     assert(head != nullptr);
//     node<T>* temp = nullptr;
//     while(head != nullptr){
//         temp = head->_next;
//         delete head;
//     }
// }

// template<class T> bool empty(const node<T> *head){
//     if(head == nullptr){
//         return true;
//     }
//     return false;
// }

// template<class T> node<T> *copy_list(const node<T> *head, node<T> *&cpy){
//     assert(head != nullptr);
//     for(node<T>* walker = head; walker!= nullptr; walker = walker->_next){
//         T item = walker->_item
//     }
// }

// template<class T> node<T> *insert_head(node<T> *&head, T item){
//     node* temp = new node(item);
//     if (head){
//         head->_prev = temp;
//     }
//     temp->_next = head;
//     head = temp;
// }

// template<class T> node<T> *insert_after(node<T> *&head, node<T> *after, const T &item){
//     if(after == nullptr){
//         insert_head(head, item);
//     }
// }

// template<class T> T delete_head(node<T> *&head){
//     assert(head != nullptr);
//     node<T>* deletethis = *head;
//     node<T>* temp = deletethis->_next;
//     temp->_prev = nullptr;
//     head = temp;
//     delete deletethis;
//     return temp;
// }

// template<class T> std::ostream &print_list(const node<T> *head, std::ostream &outs = cout){
//     cout<<"H->";
//     for (node<T>* walker = head; walker!= nullptr; walker=walker->next){
//         cout<<*walker;
//     }
//     cout<<"|||";
// }