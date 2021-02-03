#ifndef TABLE_H
#define TABLE_H
#define MAX_COLUMNS 128
#define MAX_ROWS 30
#include <iostream>
#include <cstring>

using namespace std;;

//Fill all cells of the array with -1
void init_table(int _table[][MAX_COLUMNS]){
    for(int row = 0;row < MAX_ROWS;row++){
        for(int col = 0; col < MAX_COLUMNS;col ++){
            _table[row][col]= -1;
        }
    }
};

//Mark this state (row) with a 1 (success)
void mark_success(int _table[][MAX_COLUMNS], int state){
    _table[state][0]= 1;
};

//Mark this state (row) with a 0 (fail)
void mark_fail(int _table[][MAX_COLUMNS], int state){
    _table[state][0]= 0;
};

//true if state is a success state
bool is_success(int _table[][MAX_COLUMNS], int state){
    if(_table[state][0]==1){
        return true;
    }else{
        return false;
    }
};

//Mark a range of cells in the array. 
void mark_cells(int row, int _table[][MAX_COLUMNS], int from, int to, int state){
    for(int col = from; col < to; col++){
        _table[row][col] = state;
    }
};

//Mark columns represented by the string columns[] for this row
void mark_cells(int row, int _table[][MAX_COLUMNS], const char columns[], int state){
    for(int i = 0; i < strlen(columns); i++){
        _table[row][columns[i]] = state;
    }
};

//Mark this row and column
void mark_cell(int row, int _table[][MAX_COLUMNS], int column, int state){
    _table[row][column] = state;
};

//This can realistically be used on a small table
void print_table(int _table[][MAX_COLUMNS]){
    for (int row = 0; row < sizeof(*_table)/sizeof(*_table); row++){
        cout<<" | ";
        for (int col= 0; col < MAX_COLUMNS; col++){
            cout<<_table[row][col]<<" | ";
        }
        cout<<endl;
    }
};

//show string s and mark this position on the string:
//hello world   pos: 7
void show_string(char s[], int _pos){
    for(int i = 0; i <strlen(s);i++){
        cout<<s[i];
    }
    cout<<"pos: "<<_pos<<endl;
};
#endif // TABLE_H