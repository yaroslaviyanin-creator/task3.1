/*
lib_main.c - главный модуль библиотеки.
Янин Ярослав Иванович
Группа МК-101
*/

#include "lib_main.h"
#include <malloc.h>

// Печать хелпа на экране
void print_help() {
    fprintf(stderr, "Arguments: <input_file> <output_file> <type_proc> <param1> <param2>\n\n");
    fprintf(stderr, "           <type_proc> = { test | sort }\n\n");
    fprintf(stderr, "           <input_file> <output_file> test <max_size_str> <count_str>\n\n");
    fprintf(stderr, "           <input_file> <output_file> sort <type_sort> <type_cmp>\n\n");
    fprintf(stderr, "           <type_sort> = { slow | fast }\n");
    fprintf(stderr, "           <type_cmp> = { 1 | 2 | 3 | 4 }\n");
    fprintf(stderr, "                          1 -  cmp_str_alpha\n");
    fprintf(stderr, "                          2 -  cmp_str_alpha_tolower\n");
    fprintf(stderr, "                          3 -  cmp_str_length\n");
    fprintf(stderr, "                          4 -  cmp_str_end\n\n");
    fprintf(stderr, "Example:   in.txt out.txt test 64 10\n");
    fprintf(stderr, "Example:   in.txt out.txt sort slow 1\n");
    fprintf(stderr, "Example:   in.txt out.txt sort fast 3\n");
    exit(1);
}


int process_file(const char* in_path, const char* out_path, const char* search, const char* replace) {



}