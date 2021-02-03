#ifndef PARSER_H
#define PARSER_H
#include "../Token/STokenizer.h"
#include "../b+tree/mmap.h"
#include "../b+tree/map.h"
#include "BTable.h"


using namespace std;

class Parser{
    public:
        Parser(const char* s);

        void set_string();
        int get_column(Token t);

        bool get_parse_tree();
        void build_keyword_list();
        void initial_table();

        MMap<string,string>& parse_tree()
        {
            return ptree;
        }
        bool fail()
        {
            return false;
        }
    private:
        MMap<string,string> ptree;
        Map<string,int> keywords;

        int _table[MAX_COLUMNS][MAX_COLUMNS];
        bool flag=false;
        //input_queue hold input
        Queue<Token> input;
        enum keys{ZERO,SELECT,FROM,WHERE,MAKE,INSERT,TO,AND,OR};

        char _buffer[MAX_BUFFER];       //input string

};


Parser::Parser(const char* s){
    strcpy(_buffer,s);
    // cout<<"1"<<endl;
    initial_table();
        // cout<<"1"<<endl;

    build_keyword_list();
        // cout<<"1"<<endl;

    
    set_string();
    //    cout<<"1"<<endl;


}

void Parser::set_string()
{

    STokenizer stk(_buffer);
    Token t;
    while(!stk.done()){
        t = Token();
        stk>>t;
        if(t.type()==1||t.type()==2||t.type()==4){//means it is string and operator
            input.push(t);
        }
    }
    // cout<<"getting ptree\n";
    get_parse_tree();
    // cout<<"get it"<<endl;

}

int Parser::get_column(Token t){
    if(keywords.contains(t.token_str())){
        return  keywords[t.token_str()];
    }else {
        if(t.type()==2){
            return 10;
        }
        
        if(t.type()==4){
            
            return 11;
        }
        return 6;
    }

}

bool Parser::get_parse_tree(){
    int state=0;
    while(!input.empty() && state!=-1){
        Token t=input.pop();
        state=_table[state][get_column(t)];
        // cout<<state<<"-----------"<<t<<endl;
        switch (state) {
            case 1:
            {
                
                ptree["command"].push_back(t.token_str());
                break;
            }
            case 11:
            {
                ptree["command"].push_back(t.token_str());
                break;
            }
            case 16:
            {
                ptree["command"].push_back(t.token_str());
                break;
            }
            case 2:
            {
                ptree["field"]+=t.token_str();
                break;
            }
            case 13:
            {
                ptree["table"]+=t.token_str();
                break;
            }
            case 15:
            {
                ptree["field"]+=t.token_str();
                break;
            }
            case 18:
            {
                ptree["table"]+=t.token_str();
                break;
            }
            case 3:
            {
                ptree["command"]+=t.token_str();
                break;
            }
            case 4:
            {
                ptree["table"]+=t.token_str();
                break;
            }
            case 5:
            {
                if(t.token_str()=="and"||t.token_str()=="or")
                    ptree["condition"]+=t.token_str();
                else
                    ptree["command"]+=t.token_str();
                break;
            }
            case 12:
            {
                break;
            }
            case 14:
            {
                break;
            }
            case 17:
            {
                break;
            }
            case 19:
            {
                break;
            }
            case 20:
            {
                if(t.type()==1||t.type()==4){
                    ptree["info"]+=t.token_str();
                }
                break;
            }
            case 22:
            {
                if(t.type()==1){
                    string s= t.token_str();
                    t = input.pop();
                    if(t.type()==1){
                        s= s+' '+t.token_str();
                    }
                    ptree["info"]+= s;
                }else{
                    ptree["info"]+= t.token_str();
                }
            }
            default:
            {
                if(t.token_str()!="\"")
                    ptree["condition"]+=t.token_str();
                break;
            }
        }

    }
    if(state == -1){
        ptree.clear();
        return false;
    }
    return true;
}

void Parser::build_keyword_list()
{
    keywords.insert("select",SELECT);
    keywords.insert("from",FROM);
    keywords.insert("where",WHERE);
    keywords.insert("make",MAKE);
    keywords.insert("insert",INSERT);
    keywords.insert("and",AND);
    keywords.insert("or",OR);

}


void Parser::initial_table()
{
    //mark success state
    init_table(_table);
    mark_fail(_table,0);
    mark_fail(_table,1);
    mark_fail(_table,2);
    mark_fail(_table,3);
    mark_success(_table,4);

    //select all
    mark_cell(1,_table,10,2);
    //command
    mark_cell(0,_table,1,1);

    //MAKE
    mark_cell(0,_table,4,11);
    mark_cell(11,_table,6,12);
    mark_cell(12,_table,6,13); // table name
    mark_cell(13,_table,6,14);
    mark_cell(14,_table,6,15); // field
    mark_cell(15,_table,10,14); //punc
    //INSERT
    mark_cell(0,_table,5,16);
    mark_cell(16,_table,6,17); //to
    mark_cell(17,_table,6,18); //table name
    mark_cell(18,_table,6,19); //values
    mark_cell(19,_table,6,20);
    mark_cell(19,_table,10,21);
    mark_cell(19,_table,11,20);
    mark_cell(20,_table,6,20);
    mark_cell(20,_table,10,20);
    mark_cell(20,_table,11,20);
    mark_cell(21,_table,6,22);
    mark_cell(21,_table,11,22);
    mark_cell(22,_table,10,19);
    mark_cell(22,_table,6,22);
    mark_cell(22,_table,11,22);




    //field
    mark_cell(2,_table,2,3);

    //where
    mark_cell(4,_table,3,5);


    //sym
    mark_cell(1,_table,6,2); // field
    mark_cell(3,_table,6,4); // table
    mark_cell(5,_table,6,6); // cond 1-1
    mark_cell(6,_table,10,7); // operator
    mark_cell(7,_table,6,8); // cond 1-2
    mark_cell(7,_table,11,8); //number
    mark_cell(7,_table,10,9);
    mark_cell(9,_table,6,23);
    mark_cell(23,_table,6,23);
    mark_cell(23,_table,10,8);
    mark_cell(8,_table,10,10);
    mark_cell(10,_table,6,8);
    mark_success(_table,8);
    mark_cell(8,_table,AND,5); //AND / OR
    mark_cell(8,_table,OR,5);

    // for(int i=0;i<5;i++)
    // {
    //     for(int j=0;j<6;j++)
    //     {
    //         cout<<_table[i][j]<<"|";
    //     }
    //     cout<<endl;
    // }


}


#endif // PARSER_H