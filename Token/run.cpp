#include <iostream>
#include "STokenizer.h"
#include <iomanip>

using namespace std;

int main(){
    char s[] = "select last from student where last = lastname and country = america";
    STokenizer stk(s);
    Token t;
    //The all too familiar golden while loop:

    while(!stk.done()){
        t = Token();
        stk>>t;
        //process token here...
        cout<<setw(10)<<t.type_string()<<setw(10)<<t<<endl;
    }
}