/*
lib_main.c - главный модуль библиотеки.
Янин Ярослав Иванович
Группа МК-101
*/

#include <stdlib.h>
#include <stdio.h>
#include "lib_main.h"
#include "config.h"

/*void GenerateRandomString(char* str, unsigned int max_size) {

unsigned int size = rand() % max_size;
// генерация одной случайной строки

}
*/

/*void GenerateRandomStrings() {

    for (;;) {
        char *p;
        GenerateRandomString (p, 10);
        }

}
*/

void PrintStrings(LargeBlock** strings) {


}


void SortStrings(LargeBlock** strings, void* compareStringsFunction) {



}


// Возвращаем файловый указатель (курсор) в начало файла
int fseek_begin(FILE* in_file) {
    if (fseek(in_file, 0, SEEK_SET) != 0) {   // SEEK_SET — константа, означающая смещение относительно начала файла на 0 байт
        fprintf(stderr, "Error: An error occurred when moving the file pointer to the beginning of the file.");
        return 1;
    }
    return 0;
}
