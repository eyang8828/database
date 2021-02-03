#ifndef MAP_H
#define MAP_H
#include "BPlus_Tree.h"

using namespace std;

template<class K, class V>
struct Pair{
    
    K key;
    V value;
    Pair(){};
    Pair(const K& k){
        key = k;
    }
    Pair(const K& k, const V& v){
        key= k;
        value = v;
    };
    friend ostream& operator <<(ostream& outs, const Pair<K, V>& print_me){
        outs<<print_me.key<<"--"<<print_me.value;
        return outs;
    };

    void operator =(int i){
        if(i ==0){
            value = i;
        }
    }
    void operator =(const Pair<K,V>& rhs){
        key = rhs.key;
        value = rhs.value;
    }
    friend bool operator ==(const Pair<K, V>& lhs, const Pair<K, V>& rhs){
        return lhs.key==rhs.key;
    };
    friend bool operator < (const Pair<K, V>& lhs, const Pair<K, V>& rhs){
        return lhs.key<rhs.key;
    };
    friend bool operator > (const Pair<K, V>& lhs, const Pair<K, V>& rhs){
        return lhs.key>rhs.key;
    };
    friend bool operator <= (const Pair<K, V>& lhs, const Pair<K, V>& rhs){
        return lhs.key<=rhs.key;
    };
    friend bool operator >= (const Pair<K, V>& lhs, const Pair<K, V>& rhs){
        return lhs.key>=rhs.key;
    };
};

template<class K,class V>
class Map{
    public:
        typedef BPlusTree<Pair<K, V> > map_base;

        Map(){
            key_count= 0;
        };

    //  Capacity
        int size() const{
            return _map.size();
        };
        bool empty() const{
            return key_count==0;
        };

    //  Element Access
        V& operator[](const K& key){
            Pair<K,V> target =Pair<K,V>(key);
            Pair<K,V> p =  _map.get(target);
            V& temp = p.value;
            return temp;
        };
        V& at(const K& key){
            // Pair<K,V> p =Pair<K,V>(key);
            return _map.get(key);
        };


    //  Modifiers
        void insert(const K& k, const V& v){
            Pair<K,V> p = Pair<K,V>(k,v);
            _map.insert(p);
            key_count++;
        };
        void erase(const K& key){
            Pair<K,V> p =Pair<K,V>(key);
            _map.remove(p);
            key_count--;
        };
        void clear(){
            _map.clear_tree();
            key_count=0;
        };
        V get(const K& key){
            Pair<K,V> p =Pair<K,V>(key);
            return _map.get(p);
        };

    //  Operations:
        bool contains(const Pair<K, V>& target){
            bool flag = _map.contains(target);
            return flag;
        };

        // bool is_valid(){return _map.is_valid();}
        friend ostream& operator<<(ostream& outs, const Map<K, V>& print_me){
            outs<<print_me._map<<endl;
            return outs;
        }
    private:
        int key_count;
        BPlusTree<Pair<K, V> > _map;
};





#endif //MAP_H