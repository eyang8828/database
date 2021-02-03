#ifndef BPlus_Tree
#define BPlus_Tree

using namespace std;

#include "BPlus_Tree_Array_Function.h"
#include <iomanip>
#include <cassert>
#include "../stack/stack.h"



template <class T>
class BPlusTree{
    public:
        class Iterator{
            public:
                friend class BPlusTree;
                Iterator(BPlusTree<T>* _it=nullptr, int _key_ptr = 0):it(_it), key_ptr(_key_ptr){}

                T operator *(){
                    assert(key_ptr<=it->data_count);
                    return it->data[key_ptr];
                }


                Iterator operator++(int un_used){
                    if(key_ptr<it->data_count-1){
                        key_ptr++;
                    }else{
                        key_ptr=0;
                        it = it->next_node;
                    }
                    return Iterator(it,key_ptr);
                }

                Iterator operator++(){
                    if(key_ptr<it->data_count-1){
                        key_ptr++;
                    }else{
                        key_ptr=0;
                        it = it->next_node;
                        it->print_tree();
                    }
                    return Iterator(it,key_ptr);
                }
                friend bool operator ==(const Iterator& lhs, const Iterator& rhs){
                    return (lhs.it == rhs.it&&lhs.key_ptr==rhs.key_ptr);
                }

                friend bool operator !=(const Iterator& lhs, const Iterator& rhs){
                    return (lhs.it != rhs.it||lhs.key_ptr!=rhs.key_ptr);
                }
                void print_Iterator(){
                    it->print_tree();
                }
                BPlusTree<T>* btree(){
                    return it;
                }
                bool is_null(){return !it;}
            private:
                BPlusTree<T>* it;
                int key_ptr;
        };
        BPlusTree(bool dups = false);
        //big three:
        BPlusTree(const BPlusTree<T>& other);
        ~BPlusTree();
        BPlusTree<T>& operator =(const BPlusTree<T>& RHS);

        void insert(const T& entry);                //insert entry into the tree
        bool remove(const T& entry);                //remove entry from the tree

        void clear_tree();                          //clear this object (delete all nodes etc.)
        void copy_tree(const BPlusTree<T>& other);
        void copy_tree(const BPlusTree<T>& other, Stack<BPlusTree<T>* >& tree_ptr);  //copy other into this object

        bool contains(const T& entry);              //true if entry can be found in the array
        T& get(const T& entry);                     //return a reference to entry in the tree
        T* find(const T& entry);                    //return a pointer to this key. NULL if not there.
        T* find_low(const T& entry);
        int size() const;                           //count the number of elements in the tree
        bool empty() const;                         //true if the tree is empty

        void print_tree(int level = 0, ostream &outs=cout) const; //print a readable version of the tree
        void print_node(int level = 0, ostream &outs=cout) const;
        friend ostream& operator<<(ostream& outs, const BPlusTree<T>& print_me){
            print_me.print_tree(0, outs);
            return outs;
        };
        
        //Iterator operation
        Iterator begin(){
            return Iterator(get_smallest());
        };

        Iterator end(){
            return Iterator();
        };

        Iterator upper_bound(const T &entry);
        Iterator lower_bound(const T &entry);
        BPlusTree* get_smallest()const;                 //return a pointer to the leftmost child

        bool is_valid(){
            for(int i = 0; i , i<data_count;i++){
                if(data[i]>data[i+1])
                    return false;
            }
            if(is_leaf()){
                return true;
            }
        }
    private:
        static const int MINIMUM = 1;
        static const int MAXIMUM = 2 * MINIMUM;

        bool dups_ok;                                   //true if duplicate keys may be inserted
        int data_count;                                 //number of data elements
        T data[MAXIMUM + 1];                            //holds the keys
        int child_count;                                //number of children
        BPlusTree* subset[MAXIMUM + 2];                 //children
        BPlusTree* next_node;                           //pointer to the next node

        bool is_leaf() const {return child_count==0;}   //true if this is a leaf node
        T* find_ptr(const T& entry);                    //return a pointer to this key. NULL if not there.

        //insert element functions
        void loose_insert(const T& entry);              //allows MAXIMUM+1 data elements in the root
        void fix_excess(int i);                         //fix excess of data elements in child i

        //remove element functions:
        bool loose_remove(const T& entry);              //allows MINIMUM-1 data elements in the root
        void fix_shortage(int i);                       //fix shortage of data elements in child i

        void remove_biggest(T& entry);                  //remove the biggest child of this tree->entry
        void transfer_left(int i);                      //transfer one element LEFT from child i
        void transfer_right(int i);                     //transfer one element RIGHT from child i
        BPlusTree* merge_with_next_subset(int i);       //merge subset i with subset i+1
};


template<class T>
typename BPlusTree<T>::Iterator BPlusTree<T>::upper_bound(const T &entry){
    int i = first_ge(data,data_count,entry);
    bool found = (i<=data_count && data[i] == entry);
    Iterator it;
    if(is_leaf()){
        if(this->next_node==nullptr){
            return Iterator();
        }
        it = Iterator(this,i);
        if(!found){
            it++;
        }
        return it;
    }else{
        return subset[i]->upper_bound(entry);
    }
}

template<class T>
typename BPlusTree<T>::Iterator BPlusTree<T>::lower_bound(const T &entry){
    int i = first_ge(data,data_count,entry);
    bool found = (i<=data_count && data[i-1] == entry);
    Iterator it;
    if(is_leaf()){
        it = Iterator(this,i);
        if(it == begin()){
            return begin();
        }
        if(found){
            it++;
        }
        it++;
        return it;
    }else{
        return subset[i]->lower_bound(entry);
    }
}

template<class T> 
BPlusTree<T>::BPlusTree(bool dups){
    data_count = 0;
    child_count = 0;
    dups_ok = dups;
    for(int i = 0; i < MAXIMUM+2; i++){
        subset[i] = nullptr;
    }
    next_node = nullptr;   
}

template<class T>
BPlusTree<T>::~BPlusTree(){
    clear_tree();
}

template<class T>
BPlusTree<T>::BPlusTree(const BPlusTree<T> &other){
    copy_tree(other);
}

template<class T>
BPlusTree<T> &BPlusTree<T>::operator=(const BPlusTree<T> &RHS){
    copy_tree(RHS);
    return *this;
}

template<class T>
void BPlusTree<T>::copy_tree(const BPlusTree<T> &other){
    Stack<BPlusTree<T>*> tree_ptr;
    if(!other.empty()){
        copy_tree(other,tree_ptr);
    }
    
}

template<class T>
void BPlusTree<T>::copy_tree(const BPlusTree<T> &other, Stack<BPlusTree<T>*>& tree_ptr){
    if(other.data_count == 0){
        return;
    }
    copy_array(data,other.data,data_count,other.data_count);
    for(int i = 0; i < other.child_count;i++){
        subset[i] = new BPlusTree<T>;
        subset[i]->copy_tree(*other.subset[i],tree_ptr);
        if(subset[i]->is_leaf()){
            if(!tree_ptr.empty()){
                BPlusTree<T>* temp = tree_ptr.pop();
                temp->next_node=subset[i];
            }
            tree_ptr.push(subset[i]);
        }
    }
    child_count= other.child_count;

}

template<class T>
void BPlusTree<T>::print_tree(int level , std::ostream &outs) const{
    if(is_leaf()){
        for(int i = data_count-1;i>=0;i--){
            outs<<string(level*4,' ')<<"["<<data[i]<<"]"<<endl;
        }
    }else{
        for(int i =child_count-1;i>0;i--){
            subset[i]->print_tree(level+1);
            outs<<string(level*4,' ')<<"["<<data[i-1]<<"]"<<endl;
        }
        subset[0]->print_tree(level+1);
        BPlusTree<T> * temp = next_node;
        while(temp) {
            outs << "[" ;
            print_array(temp->data, temp->data_count);
            outs << "] "<<temp->data_count<<"|" << temp->child_count << " -> ";
            temp = temp->next_node;
        }
    }

}

template<class T>
void BPlusTree<T>::print_node(int level,ostream &outs) const{
    if(!is_leaf()) {        
        subset[0]->print_node(level, outs);    
    } else {        
        outs << "[" ;        
        print_array(data, data_count);        
        outs << "] "<<data_count<<"|" << child_count << " -> ";        
        BPlusTree<T> * temp = next_node;
        while(temp) {            
            outs << "[" ;            
            print_array(temp->data, temp->data_count);            
            outs << "] "<<temp->data_count<<"|" << temp->child_count << " -> ";            
            temp = temp->next_node;        
        }    
    }
}

template<class T>
int BPlusTree<T>::size() const{
    return data_count;
}


template<class T>
bool BPlusTree<T>::empty() const{
    return data_count == 0;
}

template<class T>
void BPlusTree<T>::fix_excess(int i){
    if(!(i<child_count)){
        return void();
    }
    BPlusTree<T>* new_child = new BPlusTree<T>();
    insert_item(subset,i+1,child_count,new_child );
    split(subset[i]->data,subset[i]->data_count, subset[i+1]->data,subset[i+1]->data_count);
    split(subset[i]->subset,subset[i]->child_count, subset[i+1]->subset,subset[i+1]->child_count);

    T item;
    detach_item(subset[i]->data,subset[i]->data_count,item);
    ordered_insert(data,data_count,item);

    if(subset[i]->is_leaf()){
        insert_item(subset[i+1]->data,0,subset[i+1]->data_count,item);
        subset[i+1]->next_node = subset[i]->next_node;
        subset[i]->next_node = subset[i+1];
    }
    

}

template<class T>
void BPlusTree<T>::loose_insert(const T &entry){
    int i  = first_ge(data,data_count,entry);
    bool found = (i<data_count && data[i] == entry);
    if(found){
        if(is_leaf()){
            return;
        }else{
            subset[i+1]->loose_insert(entry);
            if(subset[i+1]->data_count>MAXIMUM)
                fix_excess(i+1);
        }
    }else{
        if(is_leaf()){
            insert_item(data,i,data_count,entry);
        }else{
            subset[i]->loose_insert(entry);
            if(subset[i]->data_count>MAXIMUM){
                fix_excess(i);
            }
        }        
    }
}

template<class T>
void BPlusTree<T>::insert(const T &entry){
    loose_insert(entry);
    if(data_count>MAXIMUM){
        BPlusTree<T>* new_tree = new BPlusTree<T>();
        copy_array(new_tree->data,data,new_tree->data_count,data_count);
        copy_array(new_tree->subset,subset,new_tree->child_count,child_count);
        data_count = 0;
        child_count =1;
        subset[0]=new_tree;
        fix_excess(0);
    }
}

template<class T>
bool BPlusTree<T>::remove(const T& entry){
    //Loose_remove the entry from this tree.
    //once you return from loose_remove, the root (this object) may have no data and only a single subset
    //now, the tree must shrink:
    //  point a temporary pointer (shrink_ptr) and point it to this root's only subset
    //  copy all the data and subsets of this subset into the root (through shrink_ptr)
    //  now, the root contains all the data and poiners of it's old child.
    //  now, simply delete shrink_ptr (blank out child), and the tree has shrunk by one level.
    //  Note, the root node of the tree will always be the same, it's the child node we delete
    if(!loose_remove(entry)){
        return false;
    }
    if(data_count<MINIMUM && !is_leaf()){
        BPlusTree<T>* shrink_ptr = subset[0];
        copy_array(data,subset[0]->data,data_count,subset[0]->data_count);
        copy_array(subset,subset[0]->subset,child_count,subset[0]->child_count);
        shrink_ptr->child_count = 0;
        delete shrink_ptr;
        shrink_ptr = nullptr;
    }
    return true;

}
template <class T>
bool BPlusTree<T>::loose_remove(const T& entry){
    int i  = first_ge(data,data_count,entry);
    bool found = (i<data_count && data[i] == entry);
    if(found&&is_leaf()){
        T target = entry;
        delete_item(data,i,data_count,target);
        return true;
    }else{   // not found
        if(found)
            i++;
        bool entered = subset[i]->loose_remove(entry);
        if(subset[i]->size()<MINIMUM){
            fix_shortage(i);
        }
        return entered;
    }
    return false;
     /* four cases:
          a. leaf && not found target: there is nothing to do
          b. leaf && found target: just remove the target
          c. not leaf and not found target: recursive call to loose_remove
          d. not leaf and found: replace target with largest child of subset[i]

             |   !found    |   found       |
       ------|-------------|---------------|-------
       leaf  |  a: nothing | b: delete     |
             |     to do   |    target     |
       ------|-------------|---------------|-------
       !leaf | c: loose_   | d: replace    |
             |    remove   |    w/ biggest |
       ------|-------------|---------------|-------


     */
}

template <class T>
void BPlusTree<T>::fix_shortage(int i){
    /*
     * fix shortage in suBPlusTree i:
     * if child i+1 has more than MINIMUM, transfer left
     * elif child i-1 has more than MINIMUM, transfer right
     * elif there is a right child, merge child i with next child
     * else merge child i with left child
     */

    if(i<child_count-1 && subset[i+1]->data_count>MINIMUM){
        transfer_left(i);
    }else if(i>0 && subset[i-1]->data_count>MINIMUM){
        transfer_right(i);
        if(i>0&&subset[i]->data[0]>data[i-1])
            data[i-1]= subset[i]->data[0];
    }else if (i>0 && subset[i-1]!= nullptr){
        merge_with_next_subset(i-1);
    }else{
        merge_with_next_subset(i);
    }
    
    
}

template <class T>
void BPlusTree<T>::remove_biggest(T& entry){
    if(!is_leaf()){
        subset[child_count-1]->remove_biggest(entry);
        if(subset[child_count-1]->data_count==0)
            fix_shortage(child_count-1);
    }else{
        detach_item(data,data_count,entry);
    }
    // Keep looking in the last suBPlusTree (recursive)
    //  until you get to a leaf.
    // Then, detach the last (biggest) data item
    //
    // after the recursive call, fix shortage.

}

template <class T>
BPlusTree<T>* BPlusTree<T>::merge_with_next_subset(int i){
    T item;
    delete_item(data,i,data_count,item);
    if(is_leaf())
        ordered_insert(subset[i]->data,subset[i]->data_count,item);
    merge(subset[i]->data,subset[i]->data_count,subset[i+1]->data,subset[i+1]->data_count);
    merge(subset[i]->subset,subset[i]->child_count,subset[i+1]->subset,subset[i+1]->child_count);

    BPlusTree* delete_this;
    delete_item(subset, i+1, child_count, delete_this);
    if(is_leaf())
        subset[i]->next_node= delete_this->next_node;
    delete delete_this;
    delete_this = nullptr;


    return subset[i];
}

template <class T>
void BPlusTree<T>::transfer_left(int i){
    T temp;
    delete_item(subset[i+1]->data,0,subset[i+1]->data_count,temp);
    ordered_insert(subset[i]->data,subset[i]->data_count,temp);
    data[i]= subset[i+1]->data[0];
}

template <class T>
void BPlusTree<T>::transfer_right(int i){
    T temp;
    detach_item(subset[i-1]->data,subset[i-1]->data_count,temp);
    ordered_insert(subset[i]->data,subset[i]->data_count,temp);
    data[i-1]= temp;
}

template<class T>
T* BPlusTree<T>::find_ptr(const T& entry){
    int i  = first_ge(data,data_count,entry);
    bool found = (i<data_count && data[i] == entry);
    if(is_leaf&&found){
        return &data[i];
    }
};

template<class T>
void BPlusTree<T>::clear_tree(){
    data_count = 0;
    for(int i = 0; i <child_count; i++){
        subset[i]->clear_tree();
        delete subset[i];
        subset[i]= NULL;
    }
    child_count = 0;
}


template<class T>
bool BPlusTree<T>::contains(const T &entry){
    int i = first_ge(data, data_count, entry);
    bool found = (i<data_count && data[i]==entry);
    if (found){
        return true;
    }else if (is_leaf()){
        return false;
    }
    return subset[i]->contains(entry);

}

template <class T>
T* BPlusTree<T>::find(const T& entry){
    int i = first_ge(data, data_count, entry);
    bool found = (i<data_count && data[i]==entry);
    if(found){
        return &data[i];
    }else if (is_leaf()){
        return nullptr;
    }
    return subset[i]->find(entry);
}

template <class T>
T* BPlusTree<T>::find_low(const T& entry){
    int i = first_ge(data, data_count, entry);
    bool found = (i<=data_count && data[i]==entry);
    if(found&&!is_leaf())
        i++;
    if(found&&is_leaf()){
        return &data[i];
    }else if (is_leaf()){
        return nullptr;
    }
    return subset[i]->find_low(entry);
}


template<class T>
T &BPlusTree<T>::get(const T &entry){
    //If entry is not in the tree, CRASH
    //assert(contains(entry));

    int i = first_ge(data, data_count, entry);
    bool found = (i<data_count && data[i]==entry);
    if (found)
        return data[i];
    if (is_leaf()){
        insert(entry);
        return get(entry);
    }
    return subset[i]->get(entry);
}

template<class T>
BPlusTree<T>*BPlusTree<T>::get_smallest()const{
    if(!subset[0]->is_leaf()){
        return subset[0]->get_smallest();
    }else{
        return subset[0];
    }
}



#endif  //BPlusTree_H