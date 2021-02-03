#ifndef TOKEN_H
#define TOKEN_H
#include <iostream>

using namespace std;


class Token{
    public:
     Token();
     Token(string str, int type);
     friend ostream& operator <<(ostream& outs, const Token& t){
        cout << t._token;
        return outs;
     };
     int type();
     string type_string();
     string token_str();
    private:
     int _type;
     string _token;
};
//default ctor
Token::Token(){
    _type = 0;
    _token = "";
}

Token::Token(string str, int type){
    _type = type;
    _token = str;
}
string Token::token_str(){
    return _token;
}
int Token::type(){
    return _type;
}
string Token::type_string(){

    //switch statement that return their type as string.
    switch (_type){
    case 1:
        return "ALPHA";
        break;
    case 2:
        return "PUNCTUATION";
        break;
    case 3:
        return "SPACE";
        break;
    case 4:
        return "NUMBER";
        break;
    case 6:
        return "NUMBER";
        break;
    default:
        return "UNKNOWN";
    }
}

#endif // TOKEN_H