#include <iostream>
#include "BTable.h"
#include "../!include/vector/vector.h"
using namespace std;

// typedef vector<string> vectorstr;
int main(int argc, char *argv[])
{
    vector<string> inf;
    inf.push_back("first_name");
    inf.push_back("last_name");
    inf.push_back("Major");
    string s = "student";
    btable<string> t = btable<string>(s,inf);
    string firstname[]={"Joe","Dave","Jane","Mary","Jane","Sid","Samuel"};
    string lastname[]={"Blow","Smith","Doe","Stevenson","Torres","Albertson","Johnson"};
    string major[]={"CS","CS","Math","CS","CS","CS","CS"};

    for(int i=0;i<7;i++){
        vectorstr temp;
        temp.push_back(firstname[i]);
        temp.push_back(lastname[i]);
        temp.push_back(major[i]);
        t.insert_into(temp);
    }
    t.print_indices();
    vectorstr field;
    field.push_back("first_name");
    field.push_back("last_name");
    vectorstr cond;
    cond.push_back("first_name");
    cond.push_back("=");
    cond.push_back("Joe");
    cond.push_back("or");
    cond.push_back("last_name");
    cond.push_back("=");
    cond.push_back("Blow");
    cond.push_back("and");
    cond.push_back("Major");
    cond.push_back("=");
    cond.push_back("CS");
    btable<string>t2 = t.select(field,cond);
    t2.print_indices();
    t2.print();

    // t.print_indices();
    // btable<string> t2 = t.select(field,cond);
    // cout<<"btable 2"<<endl;
    // t2.print();
    
    cout<<"Done ";
    
    // cout <<endl<<endl<<endl<< "================================" << endl;
    // fstream f;
    // open_fileW(f, "record_list_two_d.bin");
    // string firstname[]={"Joe","Dave","Jane","Mary","Jane","Sid","Samuel"};
    // string lastname[]={"Blow","Smith","Doe","Stevenson","Torres","Albertson","Johnson"};
    // string major[]={"CS","CS","Math","CS","CS","CS","CS"};
    
    // vectorstr list[8];
    // for(int i = 0; i<7;i++){
    //     vectorstr temp;
    //     auto str = to_string(i);
    //     temp.push_back(str);
    //     temp.push_back(firstname[i]);
    //     temp.push_back(lastname[i]);
    //     temp.push_back(major[i]);
    //     list[i]=temp;
    // }
    // Record r;
    // long length;
    // for (int i = 0; i<7; i++){
    //     vectorstr v=list[i];
 
    //     r = Record(v);
    //     cout<<"here is r: "<<endl<<r<<endl;
    //     cout<<"--------------------"<<endl;
    //     length= r.write(f);
    // }
    // f.close();
    // open_fileRW(f, "record_list_two_d.bin" );
    // long l =r.read(f, 2);
    // cout<<"record 2: "<<endl<<r<<endl;
    // l = r.read(f, 3);
    // cout<<"record 3: "<<endl<<r<<endl;
    // cout<<l;
    // cout <<endl<<endl<<endl<< "================================" << endl;
    // return 0;
}