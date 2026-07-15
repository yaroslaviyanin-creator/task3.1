/*
config.h - Модуль объявления.
Янин Ярослав Иванович
Группа МК-101
*/

#pragma once

#define IN_FILE     "in.txt"        // входной файл
#define OUT_FILE    "out.txt"       // выходной файл
#define N           10              // размер блока для считывания символов из файла

#define SMALL_BLOCK_WIDTH	1       // N1 по условию
#define SMALL_BLOCK_HEIGHT	3       // M1 по условию   
#define LARGE_BLOCK_WIDTH	3       // N2 по условию
#define LARGE_BLOCK_HEIGHT	2       // M2 по условию

#define GEN_MAX_LEN_STR     67      // максимальная длина генерируемой строки
#define GEN_COUNT_STR       123     // количество генерируемых строк

typedef char SmallBlock[SMALL_BLOCK_WIDTH][SMALL_BLOCK_HEIGHT];
typedef SmallBlock LargeBlock[LARGE_BLOCK_WIDTH][LARGE_BLOCK_HEIGHT];