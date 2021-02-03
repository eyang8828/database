#ifndef ARRAY_FUNCTION_H
#define ARRAY_FUNCTION_H
#include<iostream>
using namespace std;

template <class T>
T* allocate (int);

template<class T>
void copy_list (T*, T*, int);

template <class T>
void print_list (T*, int);

template <class T>
T* deallocate (T*, int, int&);

template <class T>
T* append (T*, int&,const T);

template <class T>
T* search_entry(T*, const T&, int);

template<class T>
T* add_entry(T*, const T, int&, int&);

template <class T>
T* re_allocate(T*, int &);

template <class T>
void shift_left(T*, int);

template<class T>
T* remove_last_entry(T*, int&, int&, T&);


#include <iostream>

using namespace std;

template <class T>
T* allocate (int capacity){
    // getting more space for the vector
    return new T[capacity];
}

template<class T>
void copy_list (T* to, T* from, int size){
    for (int i = 0; i < size; i++){
        // copying vector
        *to = *from;
        to++;
        from++;
    }
}

template<class T>
T* remove_last_entry(T* list, int& size, int& capacity, T& item){
    //point the pointer the the last item in the vector and delete it
    // if the size is too low for the capacity, shrink the capacity.
    T* walker;
    walker = list;
    walker = walker + size - 1;
    item = *walker;
    size = size - 1;
    if (size == (capacity / 4) && capacity > 4)
    {
        list = deallocate(list, size, capacity);
    }
    return list;
}

template<class T>
T* add_entry(T* list, const T new_list, int& size, int& capacity){
    // check whether we need more space for the vector or not
    if (size == capacity){
        list = re_allocate(list, capacity);
    }
    append(list, size, new_list);
    // return the pointer that point to the first item
    return list;
}

template <class T>
// add the item to the end of the vector and add the size by 1 
T* append (T* list, int& size,const T new_list){
    list = list + size;
    *list = new_list;
    size++;
    return list;
}

template <class T>
// create a new vector that 2* more storage, and delete the first
// vector once we copy the item to the new vector
T* re_allocate(T* items, int &capacity){
    T* temp;
    int size = capacity;
    capacity = capacity * 2;
    temp = allocate<T>(capacity);
    copy_list (temp, items, size);
    delete [] items;
    return temp;
}

template <class T>
void print_list (T* item, int size){
    for (int i = 0; i < size; i++)
    {
        cout << " " << *item;
        item++;
    }
    cout << endl;
}

template <class T>
void print (T* item, int size){
    for (int i = 0; i < size; i++){
        cout << " "<< *item;
        item++;
    }
}


template <class T>
//walk through the vector to find target, if not found
// return nullptr;
T* search_entry(T* list, const T& target, int size){
    T* walker = list;
    while(walker - list < size){
        if (*walker == target)
            return walker;
        walker++;
    }
    return nullptr;
}

template <class T>
//create a new vector that has 1/2 size of the old vector
// and delete the old one after we copy over the items
T* deallocate (T* delete_this, int size, int& capacity){
    T* temp;
    capacity = capacity / 2;
    temp = allocate<T>(capacity);
    copy_list (temp, delete_this, size);
    delete [] delete_this;
    return temp;
}


template <class T>
void shift_left(T* list, int size){
    //create a pointer points to the next box
    T* walker = list + 1;
    for (int i = 0; i < size; i++){
        //start from the address of the matching number, shift every thing left one unit
        *list = *walker;
        list++;
        //increase the pointer until the end
        walker++;
    }
}

template <class T>
T* remove_entry (T* list, const T& delete_me, int& size, int& capacity){
    T* walker;
    //deallocate if size is one forth of the capacity and capacity is larger than 4
    if (size == (capacity / 4) && capacity > 4){
        list = deallocate(list, size, capacity);
    }
    //find the matching number
    walker = search_entry(list, delete_me, size);
    //delete the number found
    shift_left(walker, size);
    size--;
    return list;
}


#endif // ARRAY_FUNCTION_H