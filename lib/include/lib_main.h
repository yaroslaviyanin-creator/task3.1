/*
lib_main.h - заголовочный файл модуля lib_main.c.
Янин Ярослав Иванович
Группа МК-101
*/

#pragma once

#include <stdio.h>
#include <stdlib.h>

#define IN_FILE     "in.txt"        // входной файл
#define OUT_FILE    "out.txt"       // выходной файл
#define N           10              // размер блока для считывания символов из файла

#define DIV_MOD(X, Y) (((X) == 0) ? 0 : ((X)%(Y)))  // остаток


#define SMALL_BLOCK_HEIGHT	2       // N1 по условию (количество строк)  
#define SMALL_BLOCK_WIDTH	3       // M1 по условию (количество столбцов)
#define LARGE_BLOCK_HEIGHT	3       // N2 по условию (количество строк)
#define LARGE_BLOCK_WIDTH	4       // M2 по условию (количество столбцов)


#define GEN_MAX_LEN_STR     67      // максимальная длина генерируемой строки
#define GEN_COUNT_STR       123     // количество генерируемых строк

typedef char SmallBlock[SMALL_BLOCK_HEIGHT][SMALL_BLOCK_WIDTH];
typedef SmallBlock LargeBlock[LARGE_BLOCK_HEIGHT][LARGE_BLOCK_WIDTH];

//size_t count_str_LargeBlock = SMALL_BLOCK_HEIGHT * LARGE_BLOCK_HEIGHT;      // count_str_LargeBlock - количество строк в блоке
//size_t count_sym_LargeBlock = SMALL_BLOCK_WIDTH * LARGE_BLOCK_WIDTH;        // count_sym_LargeBlock - количество символов в блоке


// Структура элемента массива входных аргументов
typedef struct {
    char* input_file;       // входной файл
    char* output_file;      // выходной файл
    char* type_proc;        // "test" - генерация файла строк
    // "sort" - сортировка строк
    size_t max_size_str;    // максимальный размер генерируемой строки
    size_t count_str;       // количество генерируемых строк
    char* type_sort;        // "slow" - сортировка пузырьком
    // "fast" - быстрая сортировка
    int type_cmp;           // 1 - сортировка в лексикографическом порядке с учётом регистра
    // 2 - сортировка в лексикографическом порядке буз учёта регистра
    // 3 - сортировка по длине строки, а при равенстве — лексикографическая c учетом регистра
    // 4 - лексикографическая сортировка строк с конца
} TArg;

// Печать хелпа на экране
void print_help();

// Получение символа по строке и его индексу в строке
char get_sym_str(LargeBlock** strings, size_t vindex, size_t sym_index);

// Запись символа по строке и его индексу в строке
int put_sym_str(LargeBlock** strings, size_t vindex, size_t sym_index, char b);

//**********************************************************************************
//                        Функции вывод строк
//**********************************************************************************

// вывод строки на экран
void print_string(LargeBlock** strings, size_t vindex);

// вывод строки в файл
void print_string_file(FILE* out_file, LargeBlock** strings, size_t vindex);

// вывод строк на экран
void print_strings(LargeBlock** strings, size_t* mas_index, size_t count_str);

// вывод строк в файл
void print_strings_file(FILE* out_file, LargeBlock** strings, size_t* mas_index, size_t count_str);


//**********************************************************************************

// Возвращаем файловый указатель (курсор) в начало файла
int fseek_begin(FILE* in_file);

// Получение индекса конца строки
int index_end_str(LargeBlock** strings, size_t i1);

// Функция обмена двух элементов массива mas_index по индексам i1 и i2
void swap_mas_index(size_t* mas_index, size_t count_str, size_t i1, size_t i2);

//**********************************************************************************
//                             Функции сравнения
//**********************************************************************************

// Возвращаемое значение: 0 - перестановка не нужна, 1 - перестановка строк нужна.

// Сравнение строк в лексикографическом порядке с учётом регистра.
int cmp_str_alpha(LargeBlock** strings, size_t i1, size_t i2);

// Сравнение строк в лексикографическом порядке без учёта регистра. 
int cmp_str_alpha_tolower(LargeBlock** strings, size_t i1, size_t i2);

// Сравнение по длине строки, а при равенстве — лексикографическое c учетом регистра.
int cmp_str_length(LargeBlock** strings, size_t i1, size_t i2);

// Лексикографическая сортировка строк с конца
int cmp_str_end(LargeBlock** strings, size_t i1, size_t i2);

//**********************************************************************************
//                             Функция сортировки
//**********************************************************************************

// Сортировка пузырьком
void sort_strings(LargeBlock** strings, size_t* mas_index, size_t count_str, int (*cmp_str_func)(LargeBlock**, size_t, size_t));

// Вспомогательная функция для быстрой сортировки
// Нахождение индекса опорного элемента, по которому разделяем массив на две части: 
//  - слева все элементы меньше и равные опорному элементу; (определяются возвращаемым значением return 0 из функции сравнения двух строк)
//  - справа все элементы больше опорного элемента.
// min_index и max_index - границы сортируемой области массива strings
size_t support_func(LargeBlock** strings, size_t* mas_index, size_t count_str, size_t min_index, size_t max_index, int (*cmp_str_func)(LargeBlock**, size_t, size_t));

// Быстрая сортировка (через рекурсию)
void sort_strings_fast(LargeBlock** strings, size_t* mas_index, size_t count_str, size_t min_index, size_t max_index, int (*cmp_str_func)(LargeBlock**, size_t, size_t));

//************************************************************************
//                              Основная функция
//************************************************************************

int process_file(TArg arg_prog);