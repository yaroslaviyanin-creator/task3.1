/*
main.c - главный модуль программы.
Янин Ярослав Иванович
Группа МК-101
*/

#include <stdlib.h>
#include <stdio.h>
#include "config.h"




int main(int argc, char* argv[]) {


    FILE* in_file = fopen(IN_FILE, "rb");
    if (in_file == NULL) {
        fprintf(stderr, "Error: Cannot open input file %s\n", IN_FILE);
        return 1;
    }

    FILE* out_file = fopen(OUT_FILE, "wb");
    if (out_file == NULL) {
        fprintf(stderr, "Error: Cannot create output file %s\n", OUT_FILE);
        fclose(in_file);
        return 1;
    }

    printf("Files opened successfully.\n");


    char buf[N];                    // buf - считываемый блок (массив) символов из файла 
    size_t b_read;                  // b_read - количество считанных символов в блоке
    char b;                         // b - текущий символ
    size_t i, j;                    // i, j - переменные цикла
    size_t count_str = 0;           // count_str - количество строк в файле



    //**********************************************************************************************//
    //                                                                                              // 
    //                   Выделяем память под шестимерный массив strings.                            //
    //                                                                                              // 
    //**********************************************************************************************//

    // Считаем количество строк во входном файле - первый проход файла
    //================================================================
    while ((b_read = fread(buf, 1, N, in_file)) > 0) {
        for (i = 0; i < b_read; i++) {
            b = buf[i];                        // b - текущий (рассматриваемый) символ из считанного блока 
            printf("%c", b);
            if (b == 0x0A) count_str++;        // 0x0D - символ возврата каретки
            // 0x0A - символ перевода строки
            // Признак конца строки:
            // в Unix-системах — один символ 0x0A;
            // в Windows — пара символов 0x0D и 0x0A.
        }
    }
    // Если при окончании входного файла последний символ не 0x0A, т.е. есть последняя строка без завершающих символов конца строки, то
    if ((b != 0x0A)) count_str++;              // добавляем в счетчик строк +1

    printf("\ncount_str = %zd\n", count_str);

    size_t count_LargeBlock;    // count_LargeBlock -  количество групп LargeBlock для хранения количества строк
    count_LargeBlock = count_str / (SMALL_BLOCK_WIDTH * LARGE_BLOCK_WIDTH);
    if (count_str % (SMALL_BLOCK_WIDTH * LARGE_BLOCK_WIDTH) > 0) count_LargeBlock++; // Если есть хвостик из строк, то добавляем ещё один блок
    printf("\ncount_LargeBlock = %zd\n", count_LargeBlock);

    // Выделяем память под одномерный массив указателей (под count_LargeBlock указателей)
    LargeBlock** strings = (LargeBlock**)malloc(count_LargeBlock * sizeof(LargeBlock*));

    if (strings == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for array strings.");
        return 1;
    }



    // GenerateRandomStrings ();

    // PrintStrings (strings);
    // SortStrings (strings, 0);
    // PrintStrings (strings);

    fclose(in_file);
    fclose(out_file);
    return 0;
}

