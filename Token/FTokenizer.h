/*
 * Author: Eddie Yang
 * Project: file tokenizer
 * Purpose: receive text input from file, and tokenize them,
 *          by using string tokenizer.
 * Notes: String tokenizer was implemented to not taking an
 *        unknown value for this project.
 */
#include "STokenizer.h"
#include <iostream>
#include <fstream>

using namespace std;

class FTokenizer{
    public:
        const int MAX_BLOCK = MAX_BUFFER;
        FTokenizer(char* fname);
        Token next_token();
        bool more();        //returns the current value of _more
        int pos();          //returns the value of _pos
        int block_pos();     //returns the value of _blockPos
        friend FTokenizer& operator >> (FTokenizer& f, Token& t){

            //if there is more in the file, keep getting more token.
            if(f.more()){
                t= f.next_token();
            }        
            return f;
        };
    private:
        bool get_new_block(); //gets the new block from the file
        ifstream _f;   //file being tokenized
        STokenizer _stk;     //The STokenizer object to tokenize current block
        int _pos;           //Current position in the file
        int _blockPos;      //Current position in the current block
        bool _more;         //false if last token of the last block
                            //  has been processed and now we are at
                            //  the end of the last block.
};

FTokenizer::FTokenizer(char *fname){

    //open the file
    _f.open(fname);

    //initializing variables.
    _more = true;
    _pos = 0;
    _blockPos = 0;

    // call get_new block before anything started, also check whether there are
    // characters in the file, if not _more will be set to null
    get_new_block();
}

bool FTokenizer::get_new_block(){
    char buffer[MAX_BLOCK+1];
    
    //read the file with length MAX_BLOCK in to char array buffer
    _f.read(buffer,MAX_BLOCK);

    //set the last char to NULL in order to prevent out of bound issue
    buffer[MAX_BLOCK]= NULL;

    //check whether the machine got anything, if not set _more to false
    // in order to stop the whole process
    if(_f.gcount()>0){
        _stk.set_string(buffer);
    }else{

        //close the file along with setting _more to false.
        _f.close();
        _more = false;
    }

    return _more;
}

Token FTokenizer::next_token(){
    Token t;

    //if stokenizer is not done yet, keep processing.
    if(!_stk.done()){
        _stk >>t;

    //if it's out of buffer, call get_new_block for more.
    }else if (get_new_block()){

        //process again once, more block is received.
        return next_token();
    }
    return t;
}

FTokenizer::block_pos(){
    return _blockPos;
}

FTokenizer::pos(){
    return _pos;
}

bool FTokenizer::more(){
    return _more;
}