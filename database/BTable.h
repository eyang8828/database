#ifndef BTABLE_H
#define BTABLE_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include "record.h"
#include "../queue/queue.h"
#include "../B+Tree/mmap.h"
#include "../B+Tree/map.h"
using namespace std;

static long serial = 1;


template<class T>
class btable{
    public:
        btable(string filename, vectorstr fields);
        btable(string filename);
        ~btable() {}

        void insert_into(vectorstr v);
        void set_fields(vectorstr fields);
        vectorstr get_fields();
        void write_info();
        void print(ostream& outs = cout) const;
        void print_indices(ostream& outs = cout) const ;

        btable<T> select(vectorstr field_vector, vectorstr condition);
        btable<T> select_all();
        //bool empty() {};
        friend ostream& operator<<(ostream& outs, const btable<T>& print_me){
            print_me.print(outs);
            return outs;
        }
    private:
        string txt = "_fields.txt";
        string tbl = ".tbl";
        string field_name;
        string binary_name;
        vector< MMap<T, int> > indices;
        vectorstr field_names;
        Map<T, int> field_map;
        int last_record;
};

template <typename T>
btable<T>::btable(string filename, vectorstr fields) {
    fstream _f;
    // set file names
    field_name = filename + txt ;
    binary_name = filename + tbl ;

    // create binary file
    open_fileW(_f, binary_name.c_str());
    _f.close();

    set_fields(fields);

    write_info();
    last_record = 0;
}

template <typename T>
btable<T>::btable(string filename) {
    fstream _f;
    // create binary file
    field_name = filename + txt ;
    binary_name = filename + tbl ;

    _f.open(field_name);
    string a;
    while(!_f.eof()){
        _f>>a;
        field_names.push_back(a);
    }
    _f.close();

    open_fileRW(_f, binary_name.c_str());
    Record r(field_names);
    int index = 0;
    int end_no = r.read(_f,index)/5000;

    while(end_no > 0) {
        vectorstr v = r.get_fields();
        for(int i =0;i<v.size();i++){
            indices[i].insert(v[i],index);
        }
        index ++ ;
        end_no = r.read(_f, index);
    }

    last_record++;
    _f.close();

    set_fields(get_fields());
}

template <typename T>
void btable<T>::insert_into(vectorstr v) {
    //write the data into the indices according to the recno
    fstream _f;
    open_fileRW(_f, binary_name.c_str());
    Record  r(v);
    int no = r.write(_f)/5000;
    _f.close();
    for(int i = 0 ; i < v.size() ; i ++ ) {
        indices[i].insert(v[i],no);
    }
    last_record++;
}

template <typename T>
void btable<T>::set_fields(vectorstr f) {
    // set up field_map
    for(int i = 0 ; i < f.size(); i ++){
        field_map.insert(f[i],i);
    }

    field_names = f;
}

template <typename T>
void btable<T>::write_info() {
    // write the field name into txt file
    ofstream _ff = ofstream(field_name);
    for(int i = 0 ; i < field_names.size(); i ++) {
        _ff << field_names[i] << " ";
    }
    _ff.close();
}

template <typename T>
vectorstr btable<T>::get_fields() {
    vectorstr v;
    ifstream f;
    f.open(field_name);
    while(f){
        string s;
        f >> s;
        v.push_back(s);
    }
    f.close();
    return v;
}

template <typename T>
btable<T> btable<T>::select(vectorstr field_vector, vectorstr condition) {
    fstream _f;
    //adding serial number to the file
    string new_name = binary_name+"_"+to_string(serial);
    serial++;
    btable<T> t(new_name, field_vector);

    vector<int> v;
    
    const char* oper  = condition[1].c_str();
    Queue<int> q;
    int index = field_map[condition[0]];
    switch (oper[0]){
        case '=':
        {
            vector<int> temp =indices[index][condition[2]];
            for(int i = 0; i < temp.size();i++){
                q.push(temp[i]);
            }
            break;
        }
        case '<':
        {
            T s = condition[2];
            auto up = indices[index].upper_bound(s);
            
            if(!up.is_null()&&oper[1]=='='&&indices[index].contains(s)){
                up++;up++;
            }
            if(up.is_null()){
                for(auto it = indices[index].begin();it!=indices[index].end();it++){
                    v = (*it).Value_list();
                    for(int i = 0 ; i < v.size();i++){
                        q.push(v[i]);
                    }
                }
            }else{
                for(auto it = indices[index].begin();it!=up;it++){
                    v = (*it).Value_list();
                    for(int i = 0 ; i < v.size();i++){
                        q.push(v[i]);
                    }
                }
            }
            break;
        }
        case '>':
        {
            
            T s = condition[2];

            auto up = indices[index].upper_bound(s);
            if(!up.is_null()){
                up++;
                if(oper[1]!='='&&indices[index].contains(s)){
                    up++;
                }

                for(;up!=indices[index].end();up++){
                    v = (*up).Value_list();
                    for(int i = 0 ; i < v.size();i++){
                        q.push(v[i]);
                    }
                }
            }
            break;
        }
        default:
            break;
    }
    while(!q.empty()){
        v.push_back(q.pop());
    }

    //if there are more condition piece will be >=1
    int piece = condition.size()/4;
    for(int i = 1;i<=piece;i++){
        const char* logic = condition[i*4-1].c_str();
        const char* oper2  = condition[i*4+1].c_str();
        // cout<<logic<<"----"<<oper2<<endl;
        int index2 = field_map[condition[i*4]];
        switch (oper2[0]){
            case '=':
            { 
                vector<int> temp =indices[index2][condition[i*4+2]];
                for(int i = 0; i < temp.size();i++){
                    q.push(temp[i]);
                }
                break;
            }
            case '<':
            {
                T s = condition[i*4+2];
                auto up = indices[index2].upper_bound(s);
                
                if(!up.is_null()&&oper[1]=='='&&indices[index2].contains(s)){
                    up++;up++;
                }
                if(up.is_null()){
                    for(auto it = indices[index2].begin();it!=indices[index2].end();it++){
                        v = (*it).Value_list();
                        for(int i = 0 ; i < v.size();i++){
                            q.push(v[i]);
                        }
                    }
                }else{
                    for(auto it = indices[index2].begin();it!=up;it++){
                        v = (*it).Value_list();
                        for(int i = 0 ; i < v.size();++i){
                            q.push(v[i]);
                        }
                    }
                }
                break;
                //=======================

                // T s = condition[i*4+2];

                // auto up = indices[index2].upper_bound(s);

                // if(oper[1]=='='&&indices[index2].contains(s)){
                //     up++;up++;
                // }

                // for(auto it = indices[index2].begin();*it<*up;it++){
                //     v = (*it).Value_list();
                //     for(int i = 0 ; i < v.size();i++){
                //         q.push(v[i]);
                //     }
                // }
                
                // break;
            }
            case '>':
            {
                
                T s = condition[2];

                auto up = indices[index2].upper_bound(s);

                up++;
                if(oper[1]!='='&&indices[index2].contains(s)){
                    up++;
                }
                for(;up!=indices[index2].end();up++){
                    v = (*up).Value_list();
                    for(int i = 0 ; i < v.size();i++){
                        q.push(v[i]);
                    }
                }
                break;
            }
            default:
                break;
        }

        switch (logic[0])
        {
            case 'a':
            {   

                while(!q.empty()){
                    int i = q.pop();
                    if(!v.contain(i)){
                        v.erase(i);
                    }
                }
                break;
            }
            case 'o':
            {
                while(!q.empty()){
                    int i = q.pop();
                    if(!v.contain(i)){
                        v.push_back(i);
                    }
                }
                break;
            }
            
            default:
                break;
        }
    }
    //open the btable
    open_fileRW(_f, binary_name.c_str());
    vector<int> check_dup;
    for(int z = 0;z<v.size();z++) {
        Record r;
        r.read(_f,v[z]);
        
        vectorstr _f = r.get_fields();


        vectorstr temp2;
        // insert field value for field in vector
        for(int i = 0 ; i < field_vector.size(); i ++ ) {
            int j = field_map[field_vector[i]];
            temp2.push_back(_f[j]);
        }
        if(!check_dup.contain(v[z]))
            t.insert_into(temp2);
        check_dup.push_back(v[z]);
    }
    _f.close();
    return t;
}

template <typename T>
btable<T> btable<T>::select_all() {
    fstream _f;
    string new_name = binary_name+"_"+to_string(serial);
    serial++;
    Record r;
    btable<T> t(new_name, field_names);
    open_fileRW(_f, binary_name.c_str());
    int index = 0;
    int end_no = r.read(_f, index)/5000;

    while(end_no > 0) {
        vectorstr v = r.get_fields();
        t.insert_into(v);
        index ++ ;
        end_no = r.read(_f, index);
    }
    _f.close();
    return t;
}

template <typename T>
void btable<T>::print(ostream& outs) const {
    fstream _f;
    cout<<string(80,'=')<<endl;
    cout<<left;
    for(int i = 0 ; i < field_names.size();i++){
        cout<<setw(18)<<field_names[i];
    }
    cout<<endl;

    open_fileRW(_f, binary_name.c_str());
    Record r;
    int index = 0;
    int end_no = r.read(_f, index)/5000;

    while(end_no > 0) {
        vectorstr v = r.get_fields();
        cout<<'['<<index+1<<"] ";
        for(int i = 0; i < v.size();i++){
            cout<<setw(18)<<v[i];
        }
        cout<<endl;
        index++;
        end_no = r.read(_f, index);
    }
    _f.close();
}

template <typename T>
void btable<T>::print_indices(ostream &outs) const {
    for(int i = 0 ; i < field_names.size(); i ++) {
        outs << indices[i] <<endl;
    }

}

#endif //BTABLE_H