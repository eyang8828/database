#ifndef BPLUS_TREE_ARRAY_FUNCTION_H
#define BPLUS_TREE_ARRAY_FUNCTION_H
#include <iostream>
#include <cassert>
#include <iomanip>
#include "../!include/vector/vector.h"
// #include <vector>

using namespace std;


template <class T>
T maximal(const T& a, const T& b){                      //return the larger of the two items
    assert(a!=b);
    if(a>b){
        return a;
    }else{
        return b;
    }

};

template <class T>
void Swap(T& a, T& b){                                  //swap the two items
    T temp = a;
    a = b;
    b = temp;
};

template <class T>
int index_of_maximal(T data[ ], int n){                 //return index of the largest item in data
    T max = data[0];
    int index;
    for(int i = 1 ; i < n ; i++){
        if(max < data[i]){
            max = data[i];
            index = i;
        }
    }
    return index+1;
};


template <class T>
int first_ge(const T data[ ], int n, const T& entry){
    //loop through the array to find the first 
    //  item that is >= the target and return
    //  its index
    for(int i = 0; i < n; i++){

        if(data[i]>= entry){
            return i;
        }
    }
    //everything in the array is less than target
    //  return the size of the array
    return n;
};   //return the first element in data that is
                                                        //not less than entry

template <class T>
void attach_item(T data[ ], int& n, const T& entry){    //append entry to the right of data
    data[n]= entry;
    n++;    
};

template <class T>
void insert_item(T data[ ], int i, int& n, T entry){    //insert entry at index i in data
    if(n!=0){
        for(int j = n; j>i; j--){
            data[j]=data[j-1];
        }
    }
    n++;
    data[i]= entry;
};

template <class T>
void detach_item(T data[ ], int& n, T& entry){          //remove the last element in data and place
                                                        //it in entry
    //since the array is sorted we check if index to see whether if it's empty
    entry = data[n-1];
    n--;
};

template <class T>
void delete_item(T data[ ], int i, int& n, T& entry){   //delete item at index i and place it in entry
    assert(i<=n && i>=0);
    entry = data[i];
    for(int j = i; j < n-1; j++){
        data[j]= data[j+1];
    }
    n--;
};

template <class T>
void merge(T data1[ ], int& n1, T data2[ ], int& n2){   //append data2 to the right of data1
    //since the array is in ascending order index of maximal will return the last item's index
    for(int i = 0;i<n2;i++){
        data1[n1+i]= data2[i];
        data2[i] = 0;
    }
    n1+=n2;
    n2 = 0;
};

template <class T>
void split(T data1[ ], int& n1, T data2[ ], int& n2){   //move n/2 elements from the right of data1
                                                        //and move to data2
    n2=n1/2;
    n1=n1-n2;
    for(int i = 0; i<n2;i++){
        data2[i] = data1[n1+i];
    }
};

template <class T>
void copy_array(T dest[], const T src[],
                int& dest_size, int src_size){              //copy src[] into dest[]
    dest_size = src_size;
    for(int i = 0 ; i < src_size;i++){
        dest[i]= src[i];
    }
};

template <class T>
void print_array(const T data[], int n, int pos = -1){  //print array data
    for(int i = 0 ; i < n ; i++){
        cout<<"["<<data[i]<<"] ";
    }
};

template <class T>
bool is_gt(const T data[], int n, const T& item){       //item > all data[i]
    bool flag = true;

    for(int i = 0 ; i < n ; i++){
        if(data[i]>=item){
            flag = false;
        }
    }

    return flag;
};

template <class T>
bool is_le(const T data[], int n, const T& item){       //item <= all data[i]
    return !is_gt(data,n,item);
};
template <class T>
void ordered_insert(T data[ ], int& n, T entry){        //insert entry into the sorted array
                                                        //data with length n
    int i = first_ge(data,n,entry);
    insert_item(data,i,n,entry);
};

//-------------- Vector Extra operators: ---------------------

template <typename T>
ostream& operator <<(ostream& outs, const vector<T>& list){ //print vector list
    outs<<list;
    return outs;
};

template <typename T>
vector<T>& operator +=(vector<T>& list, const T& addme){ //list.push_back addme
    list.push_back(addme);
    return list;
};


#endif //BPLUS_TREE_ARRAY_FUNCTION_H