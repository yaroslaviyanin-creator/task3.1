/*
config.h - Модуль объявления.
Янин Ярослав Иванович
Группа МК-101
*/

#pragma once

#define IN_FILE     "in.txt"        // входной файл
#define OUT_FILE    "out.txt"       // выходной файл
#define N           10              // размер блока для считывания символов из файла

#define SMALL_BLOCK_HEIGHT	2       // N1 по условию (количество строк)  
#define SMALL_BLOCK_WIDTH	3       // M1 по условию (количество столбцов)
#define LARGE_BLOCK_HEIGHT	3       // N2 по условию (количество строк)
#define LARGE_BLOCK_WIDTH	4       // M2 по условию (количество столбцов)


#define GEN_MAX_LEN_STR     67      // максимальная длина генерируемой строки
#define GEN_COUNT_STR       123     // количество генерируемых строк

typedef char SmallBlock[SMALL_BLOCK_HEIGHT][SMALL_BLOCK_WIDTH];
typedef SmallBlock LargeBlock[LARGE_BLOCK_HEIGHT][LARGE_BLOCK_WIDTH];