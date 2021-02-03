#ifndef BTREE_MMAP_H
#define BTREE_MMAP_H
#include "BPlus_Tree.h"
// #include "../!include/vector/vector.h"
// #include <vector>

using namespace std;

template <typename K, typename V>
struct MPair{
    K key;
    vector<V> value_list;

    //--------------------------------------------------------------------------------
    /*      MPair CTORs:
     *  must have these CTORs:
     *  - default CTOR / CTOR with a key and no value: this will create an empty vector
     *  - CTOR with a key AND a value: pushes the value into the value_list
     *  _ CTOR with a key and a vector of values (to replace value_list)
     */
    //--------------------------------------------------------------------------------
    MPair(){
        value_list = vector<V>();
    };
    MPair(const K& k){
        value_list = vector<V>();
        key = k;
    };
    MPair(const K& k, const V& v){
        key = k;
        value_list.push_back(v);
    };
    MPair(const K& k, const vector<V>& vlist){
        key = k;
        value_list = vlist;
    };
    void add_value(const V& v){
        value_list.push_back(v);
    }
    vector<V> Value_list(){
        return value_list;
    }
    //--------------------------------------------------------------------------------

    //You'll need to overlod << for your vector:
    friend ostream& operator <<(ostream& outs, const MPair<K, V>& print_me){
        outs<<print_me.key<<"--"<<print_me.value_list;
        return outs;
    };
    
    void operator =(int i){
        if(i ==0){
            while(value_list.size()!=0){
                value_list.pop_back();
            }
        }
    }
    void operator =(const MPair<K,V>& rhs){
        key = rhs.key;
        value_list = rhs.value_list;
    }
    friend bool operator ==(const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return lhs.key==rhs.key;
    };
    friend bool operator < (const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return lhs.key<rhs.key;
    };
    friend bool operator > (const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return lhs.key>rhs.key;
    };
    friend bool operator <= (const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return lhs.key<=rhs.key;
    };
    friend bool operator >= (const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return lhs.key>=rhs.key;
    };
    friend MPair<K, V> operator + (MPair<K, V>& lhs, MPair<K, V>& rhs){
        if(lhs.key == rhs.key){
            while(rhs.value_list.size()!=0){
                V item = rhs.value_list.pop_back();
                lhs.value_list.push_back(item);
            }
        }
    };
};
template <typename K, typename V>
class MMap{
    public:
        typedef BPlusTree<MPair<K, V> > map_base;

        MMap(){
            key_count = 0;
        };

    //  Capacity
        int size() const{
            return mmap.size();
        };
        bool empty() const{
            return mmap.empty();
        };

        typename map_base::Iterator begin(){
            return typename MMap<K, V>::map_base::Iterator(mmap.get_smallest());
        };

        typename map_base::Iterator end(){
            return typename MMap<K, V>::map_base::Iterator();
        };

        typename map_base::Iterator upper_bound(const K& key){
            auto it = mmap.upper_bound(MPair<K,V>(key));
            return it;
        };
        typename map_base::Iterator lower_bound(const K& key){
            return mmap.lower_bound(MPair<K,V>(key));
        };

    //  Element Access
        const vector<V>& operator[](const K& key) const{
            MPair<K,V> target =MPair<K,V>(key);
            MPair<K,V> p = mmap.get(target);
            vector<V>& temp = p.value_list;
            return temp;
        };
        vector<V>& operator[](const K& key){
            return mmap.get(key).value_list;
            // MPair<K,V> target =MPair<K,V>(key);
            // MPair<K,V> p = mmap.get(target);
            // vector<V>& temp = p.value_list;
            // return temp;
        };

    //  Modifiers
        void insert(const K& key, const V& value){
            MPair<K,V> p =MPair<K,V>(key,value);
            if(contains(key)){
                MPair<K,V>* temp = mmap.find(p);
                temp->add_value(value);

                MPair<K,V>* temp2 = mmap.find_low(p);
                if(temp!=temp2){
                    temp2->add_value(value);
                }
                
            }else{
                mmap.insert(p);
                key_count++;
            }
        };
        void erase(const K& key){
            MPair<K,V> p =MPair<K,V>(key);
            mmap.remove(p);
            key_count--;
        };
        void clear(){
            mmap.clear_tree();
        };

    //  Operations:
        bool contains(const K& key){
            MPair<K,V> p = MPair<K,V>(key);
            return mmap.contains(p);
        };
        vector<V> &get(const K& key){
            MPair<K,V> p = MPair<K,V>(key);
            return mmap.get(p);
        };

        int count(const K& key){
            return key_count;
        };



        bool is_valid();

        friend ostream& operator<<(ostream& outs, const MMap<K, V>& print_me){
            outs<<print_me.mmap;
            return outs;
        }

    private:
        int key_count;
        map_base mmap;
};



#endif //BTREE_MMAP_H