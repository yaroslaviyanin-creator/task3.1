/*
main.c - главный модуль программы.
Янин Ярослав Иванович
Группа МК-101
*/

#include "lib_main.h"
#include <stdio.h>
#include <stdlib.h>


//**********************************************************************************
//                             Основная программа
//**********************************************************************************

int main(int argc, char* argv[]) {

    if ((argc > 0) && (strcmp(argv[1], "-?") == 0)) print_help();
    if (argc != 6) {
        fprintf(stderr, "Error: Invalid arguments.\n");
        print_help();
    }

    //************************************************************************
    //                Инициализация структуры входных аргументов
    //************************************************************************

    TArg arg_prog;          // структура входных аргументов
    char* sym_error;        // указатель на символ, в котором произошла ошибка при преобразоывнии строки в число

    arg_prog.input_file = argv[1];
    arg_prog.output_file = argv[2];
    arg_prog.type_proc = argv[3];
    arg_prog.count_str = 0;
    arg_prog.max_size_str = 0;
    arg_prog.type_cmp = 1;

    if (strcmp(arg_prog.type_proc, "test") == 0) {                  // arg_prog.type_proc = "test" - генерация файла строк

        arg_prog.count_str = strtol(argv[4], &sym_error, 10);    // arg_prog.max_size_str -       максимальный размер генерируемой строки

        if (sym_error == argv[4]) {     // Проверка, были ли вообще цифры ("Ошибка: В строке нет цифр."))
            fprintf(stderr, "Error: There are no numbers in the string.  argv[4] = %s    <count_str> = <number>\n", argv[4]); return 0;
        }
        else if (*sym_error != '\0') {  // "Ошибка: после числа есть лишние символы."
            fprintf(stderr, "Error: There are extra character after the number: \"%c\".    <count_str> = <number>\n", *sym_error); return 0;
        }
        if ((arg_prog.count_str == 0) || (argv[4][0] == '-')) {  // "Ошибка: Некорректное значение аргумента <count_str>. Должно быть > 0."
            fprintf(stderr, "Error: Incorrect argument value <count_str>. Must be > 0.\n"); return 0;
        }

        arg_prog.max_size_str = strtol(argv[5], &sym_error, 10);       // arg_prog.count_str - количество генерируемых строк

        if (sym_error == argv[5]) {     // Проверка, были ли вообще цифры ("Ошибка: В строке нет цифр."))
            fprintf(stderr, "Error: There are no numbers in the string.  argv[5] = %s    <max_size_str> = <number>\n", argv[5]); return 0;
        }
        else if (*sym_error != '\0') {  // "Ошибка: после числа есть лишние символы."
            fprintf(stderr, "Error: There are extra character after the number: \"%c\".    <max_size_str> = <number>\n", *sym_error); return 0;
        }
        if ((arg_prog.max_size_str < 1) || (argv[5][0] == '-')) {  // "Ошибка: Некорректное значение аргумента <max_size_str>. Должно быть > 1."
            fprintf(stderr, "Error: Incorrect argument value <max_size_str>. Must be > 0.\n"); return 0;
        }
    }
    else if (strcmp(arg_prog.type_proc, "sort") == 0) {   // arg_prog.type_proc = "sort" - сортировка строк

        arg_prog.type_sort = argv[4];                // arg_prog.type_sort = "slow" - сортировка пузырьком
        // arg_prog.type_sort = "fast" - быстрая сортировка
        if ((strcmp(arg_prog.type_sort, "slow") != 0) && (strcmp(arg_prog.type_sort, "fast") != 0)) {
            fprintf(stderr, "Error: Incorrect argument value <type_sort>. Must be \"slow\" or \"fast\".\n"); return 0;
        }


        arg_prog.type_cmp = strtol(argv[5], &sym_error, 10);        // arg_prog.type_cmp - тип сравнения строк при сортировке
        // 1 - сортировка в лексикографическом порядке с учётом регистра
        // 2 - сортировка в лексикографическом порядке буз учёта регистра
        // 3 - сортировка по длине строки, а при равенстве — лексикографическая c учетом регистра
        // 4 - лексикографическая сортировка строк с конца

        if (sym_error == argv[5]) {     // Проверка, были ли вообще цифры ("Ошибка: В строке нет цифр."))
            fprintf(stderr, "Error: There are no numbers in the string.  argv[5] = %s    <type_cmp> = <number>\n", argv[5]); return 0;
        }
        else if (*sym_error != '\0') {  // "Ошибка: после числа есть лишние символы."
            fprintf(stderr, "Error: There are extra character after the number: \"%c\".    <type_cmp> = <number>\n", *sym_error); return 0;
        }
        if ((arg_prog.type_cmp < 1) || (arg_prog.type_cmp > 4)) {  // "Ошибка: Некорректное значение аргумента <type_cmp>. Должно быть между 1 и 4."
            fprintf(stderr, "Error: Incorrect argument value <type_cmp>. It should be between 1 and 4.\n"); return 0;
        }
    }
    else { fprintf(stderr, "Error: Incorrect argument value <type_proc>. Must be \"test\" or \"sort\".\n"); return 0; }


    //************************************************************************
    //                              Основная функция
    //************************************************************************

    int result = process_file(arg_prog);
    if (result != 0) {
        return result;
    }


    return 0;
}

