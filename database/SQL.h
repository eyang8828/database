#include "parser.h"
#include "BTable.h"

using namespace std;


void run_batch(){
    cout<<"Starting SQL program"<<endl;
    int number = 1;
    string s;
    ifstream inf;
    inf.open("_!select-1.txt");

    
    while(!inf.eof()){
        getline(inf,s);
        
        if(s.length()!=0 &&s[0] != '/'){
            cout<<'['<<number<<']';
            number++;
            
            Parser p = Parser(s.c_str());
    
            string table_name= p.parse_tree()["table"][0];
            
            MMap<string,string>ptree = p.parse_tree();
            
            const char* action = ptree["command"][0].c_str();

            cout<<s<<endl;
            if(action[0]=='m'){
                btable<string> t = btable<string>(table_name,ptree["field"]);
                t.print();
            }else if(action[0]=='s'){
                btable<string> t = btable<string>(table_name);
                vectorstr fields;
                if(ptree["field"][0]=="*"){
                    // cout<<"selecting all"<<endl;
                    if(ptree["condition"].size()==0){
                        btable<string> t2 = t.select_all();
                        t2.print();
                    }else{
                        cout<<t.get_fields()<<endl;
                        cout<<ptree["condition"]<<endl;
                        cout<<t.get_fields()<<ptree["condition"]<<endl;
                        btable<string> t2 = t.select(t.get_fields(),ptree["condition"]);
                        cout<<"selected"<<endl;
                        t2.print();
                    }
                    
                }else{
                    for(int i =0;i<ptree["field"].size();i++){
                        fields.push_back(ptree["field"][i]);
                    }
                    btable<string> t2 = t.select(fields,ptree["condition"]);
                    
                    t2.print();
                }
            }else{
                
                btable<string> t = btable<string>(table_name);
                vectorstr v;
                
                
                for(int i = 0; i < ptree["info"].size();i++){
                    v.push_back(ptree["info"][i]);
                }
                
                t.insert_into(v);
                t.print();
            }
            
            cout<<endl<<endl<<endl;
        }else{
            cout<<s<<endl;
        }
    }
    
    cout<<"SQL::done"<<endl;
    inf.close();   
}