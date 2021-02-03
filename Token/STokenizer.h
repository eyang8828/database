/*
 * Author: Eddie Yang
 * Project: String tokenizer
 * Purpose: make a function that can tokenize given string
 *        to little token that we can use to return their
 *        type and string inside the token.
 * Notes: Solution when the given string exceed MAX_BUFFER has
 *        not yet been implemented
 */
#ifndef STOKENIZER_H
#define STOKENIZER_H
#define MAX_COLUMNS 128
#define MAX_ROWS 30
#define MAX_BUFFER 200
#include "token.h"
#include "table.h"
#include <cstring>

using namespace std;

class STokenizer{
    public:
     STokenizer();
     STokenizer(char[]);
     bool done();            //true: there are no more tokens
                             // internal length = length

     //---------------
     //extract one token (very similar to the way cin >> works)
     friend STokenizer& operator >> (STokenizer& s, Token& t){
        string str ="";
        switch(s.get_token(0,str)){
            case 1:
                t= Token(str,1);
                break;
            case 2:
                t= Token(str,2);
                break;
            case 3:
                t= Token(str,3);
                break;
           case 4:
                t= Token(str,4);
                break;
            case 6:
                t= Token(str,6);
                break;
            default:
                // if the program encounter unknown we still need to move the pos forward
                str= s._buffer[s._pos];
                s._pos++;
                t = Token(str,0);
        }
        return s;
    };

     //set a new string as the input string
     void set_string(char str[]);

    private:
     //create table for all the tokens we will recognize
     //                      (e.g. doubles, words, etc.)
     void make_table(int _table[][MAX_COLUMNS]); 

     //extract the longest string that match
     //     one of the acceptable token types
     //     return the state of the token
     int get_token(int start_state, string& token);
     //---------------------------------
     char _buffer[MAX_BUFFER];       //input string
     int _pos;                       //current position in the string
     static int _table[MAX_ROWS][MAX_COLUMNS];
};

STokenizer::STokenizer(){
    _pos = 0;
    make_table(_table);
}

STokenizer::STokenizer(char str[]){
    _pos = 0;
    make_table(_table);
    strcpy(_buffer, str);
}
int STokenizer::get_token(int start_state,string &token){
    int point = _pos;
    int start_pos = _pos;
    bool success = false;
    int current_state = 0;
    //while function has not encounter fail state (-1)
    // and the point has not yet exceed _buffer
    while( start_state != -1 && point < strlen(_buffer)){
        
        //if we encountered something we can't reconigze,
        // skip
        if(_buffer[point]<0 || _buffer[point]>128){
            break;
        }
        //move the state according to the table we created
        start_state = _table[start_state][_buffer[point]];

        //if we encounter a success state
        if(_table[start_state][0] == 1){

            //mark current state as start state
            current_state = start_state;
            success = true;

            //move the point and mark _pos
            point++;
            _pos = point;
        
        //if we encounter fail state
        }else if (_table[start_state][0] == 0){
            //just move the pointer
            point++;
        }
    }
    //if we encountered a success state, add the string
    // into token according the _pos we marked
    if(success){
        for(start_pos;start_pos<_pos;start_pos++){
            token += _buffer[start_pos];
        }
    }
    //return current state
    return current_state;
}

void STokenizer::set_string(char *str){
    _pos=0;
    strcpy(_buffer, str);
}

bool STokenizer::done(){
    if(_pos==strlen(_buffer)){
        return true;
    }
    return false;
}

void STokenizer::make_table(int _table[MAX_ROWS][MAX_COLUMNS]){
    //initialize the table
    init_table(_table);
    
    //ALPHA
    mark_success(_table,1);
    mark_fail(_table,0);
    mark_cells(0,_table,'a','z',1);
    mark_cells(0,_table,'A','Z',1);
    mark_cells(1,_table,'a','z',1);
    mark_cells(1,_table,'A','Z',1);

    //puncuation
    mark_success(_table,2);
    mark_cells(0,_table,'!','/',2);
    mark_cells(0,_table,':','@',2);
    mark_cells(0,_table,'[','`',2);
    mark_cells(0,_table,'{','~',2);
    mark_cells(2,_table,'!','/',2);
    mark_cells(2,_table,':','@',2);
    mark_cells(2,_table,'[','`',2);
    mark_cells(2,_table,'{','~',2);

    //SPACE
    mark_success(_table,3);
    mark_cell(0,_table,' ',3);
    mark_cell(3,_table,' ',3);
    
    //NUMBER
    mark_success(_table,4);
    mark_fail(_table,5);
    mark_success(_table,6);
    mark_cells(0,_table,'0','9',4);
    mark_cells(4,_table,'0','9',4);
    mark_cell(0,_table,'9',4);
    mark_cell(4,_table,'9',4);

    //if the machine found a '.' after number 
    mark_cell(4,_table,'.',5);
    mark_cells(5,_table,'0','9',6);
    mark_cells(6,_table,'0','9',6);
}

//init table in header file
int STokenizer::_table[MAX_ROWS][MAX_COLUMNS];

#endif // STOKENIZER_H