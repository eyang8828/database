#ifndef VECTOR_H
#define VECTOR_H

#include "array_function.h"
#include <iostream>

using namespace std;

template<class T>
class vector
{
public:
    vector(unsigned int _cap = 8);
    ~vector();
    vector(const vector& other);

    vector& operator =(const vector& other)
    {
        _capacity = other._capacity;
        _size = other._size;
        //avoid point to the same space!!!
        _item = new T [_capacity];
        copy_list(_item, other._item, _size);
        //return this object
        return *this;
    }
    //getting itme at nth index
    T& operator [](const int index){
        T* walker;
        walker = _item;
        walker = walker + index;
        return *walker;
    }
    const T& operator [](const int index) const{
        T* walker;
        walker = _item;
        walker = walker + index;
        return *walker;
    }
    friend ostream& operator <<(ostream& outs, const vector& v1){
        // a point to walk us through the vector
        T* walker;
        walker = v1._item;
        outs << "{ ";

        // loop through vector and output them one by one
        for (int i = 0; i < v1._size; i++){
            outs << *walker;
            outs << " ";
            walker++;
        }
        outs << "}";
        return outs;
    }
    void push_back(const T item);
    T pop_back();
    bool contain(const T& item);
    bool erase(const T& item);
    int size() const;
    int capacity() const;
    unsigned int index_of(const T& item);
private:
    T* _item;
    int _size;
    int _capacity;
    bool _empty();
};

template<class T>
// ctor that take in a int as capacity
vector<T>::vector(unsigned int new_cap){
    _item = NULL;
    _capacity = new_cap;
    _size = 0;
    _item = allocate<T>(_capacity);
}

template<class T>
vector<T>::~vector(){
    delete [] _item;
}

template<class T>
// copy ctor
vector<T>::vector(const vector& other){
    _capacity = other._capacity;
    _size = other._size;
    _item = new T [_capacity];
    copy_list(_item, other._item, _size);
}

template<class T>
// push in an item at the end
void vector<T>::push_back(const T item){
    T* walker;
    walker = _item;
    // call add_entry to add item in, and check whether
    // we need more space or not.
    _item = add_entry(walker, item, _size, _capacity);
}

template<class T>
//remove an item at the end
T vector<T>::pop_back(){
    T temp;
    _item = remove_last_entry(_item, _size, _capacity, temp);
    return temp;
}

template<class T>
// if the item is in the vector, reeturn true
bool vector<T>::contain(const T& item)
{
    T* walker = _item;
    // loop through vector
    for (int i = 0; i < _size; i++)
    {
        if(*walker == item)
        {
            return true;
        }
        walker++;
    }
    return false;
}

template<class T>
// remove item from vector, if success -> return true
bool vector<T>::erase(const T& item){
    if(contain(item)){
        _item = remove_entry(_item, item, _size, _capacity);
        return true;
    }
    else {
        // if fail -> return false
        return false;
    }
}

template<class T>
// check whether the vector is empty by checking size
bool vector<T>::_empty(){
    if (_size == 0){
        return true;
    }
    else{
        return false;
    }
}

//size() and capacity() need to be const
template<class T>
int vector<T>::size()const{
    return _size;
}

template <class T>
int vector<T>::capacity()const{
    return _capacity;
}

template<class T>
unsigned int vector<T>::index_of(const T& item){
    for (unsigned int i = 0; i < _size; i++){
        if(_item[i] == item){
            return i;
        }
    }
}

#endif // VECTOR_H