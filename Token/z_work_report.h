#ifndef Z_WORK_REPORT_TEMPLATE_H
#define Z_WORK_REPORT_TEMPLATE_H
/*
    Features:
        -Not Implemented: The function that will determine whether the token
                ended at the middle of a word, therefore we don't cut some
                word into half.
        -Implemented: basic feature of file tokenizer that allowed us to get
                more token from stokenizer, and check whether there are more
                text input in the file, if not exit the function.
        -Partly implemented: not that I know of

    Reflections:
        in order for string tokenizer to reusable, we had to set _pos back
        to 0 when we are passing new string into the function, I can't figure
        out why my file tokenizer are not processing at the beginning, after
        doing some checking I realize the problem took place in string
        tokenizer. 

*/

#endif // Z_WORK_REPORT_TEMPLATE_H