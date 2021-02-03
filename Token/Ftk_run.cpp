#include "FTokenizer.h"
#include <iostream>
#include <iomanip>
using namespace std;
int main(){
    Token t;
    FTokenizer ftk("solitude.txt");
    ftk>>t;
    int token_count = 0;
    while (ftk.more()){
        if (t.type_string()=="ALPHA" ){
            token_count++;
            cout<<setw(10)<<token_count
                <<setw(3)<<left<<":"<<setw(25)<<left<<t.token_str()
                <<t.type_string()<<endl;
        }
        ftk>>t;
    }
    cout<<"=========="<<endl;
    cout<<"Tokens Found: "<<token_count<<endl;
    cout<<"=========="<<endl;

}