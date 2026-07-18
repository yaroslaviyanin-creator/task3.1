/*
lib_main.c - главный модуль библиотеки.
Янин Ярослав Иванович
Группа МК-101
*/

#include "lib_main.h"
#include <malloc.h>
#include <ctype.h>


size_t count_str_LargeBlock = SMALL_BLOCK_HEIGHT * LARGE_BLOCK_HEIGHT;      // count_str_LargeBlock - количество строк в блоке
size_t count_sym_LargeBlock = SMALL_BLOCK_WIDTH * LARGE_BLOCK_WIDTH;        // count_sym_LargeBlock - количество символов в блоке

// Печать хелпа на экране
void print_help() {
    fprintf(stderr, "\nArguments: <input_file> <output_file> <type_proc> <param1> <param2>\n\n");
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


// Генерация одной случайной строки в файл
void generate_random_string_to_file(FILE* in_file, size_t max_size_str) {

    // Используем буквы английского алфавита и цифры для наглядности
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    int charset_size = sizeof(charset) - 1;         // Отрезаем символ конца строки '\0'
    int rand_index;                                 // произвольный индекс

    // Вычисляем случайную длину от 0 до max_size_str
    size_t cur_len_str = rand() % (max_size_str + 1);

    char b;                                         // Сгенерированный символ

    for (int i = 0; i < cur_len_str; i++) {
        rand_index = rand() % charset_size;
        b = charset[rand_index];
        fwrite(&b, 1, 1, in_file);                  // запись символа в файл
    }
    // вывод признака конца строки в файл 
    b = 0x0D;   fwrite(&b, 1, 1, in_file);    // \r          
    b = 0x0A;   fwrite(&b, 1, 1, in_file);    // \n
}


// Генерация случайных строк в файл
void generate_random_strings_to_file(char* input_file, size_t count_str, size_t max_size_str) {

    FILE* in_file = fopen(input_file, "wb");
    if (in_file == NULL) { fprintf(stderr, "Error: Cannot open input file for test %s.\n", input_file); exit(1); }

    printf("File for test opened successfully.\n");

    for (int i = 0; i < count_str; i++) {
        generate_random_string_to_file(in_file, max_size_str);
    }

    fclose(in_file);
}


// Получение символа по строке и его индексу в строке
char get_sym_str(LargeBlock** strings, size_t vindex, size_t sym_index) {
    size_t n_lb = 0, n2 = 0, m2 = 0, n1 = 0, m1 = 0;        // текущие индексы для вложенных массивов
    size_t strings_i, ostatok_str;                          // переменные для расчета индексов строки
    size_t ostatok_sym;                                     // переменная для расчета индексов символа
    char b;

    // индексы для строки

    strings_i = vindex / count_str_LargeBlock;              // сколько больших блоков входит по высоте в строках
    ostatok_str = DIV_MOD(vindex, count_str_LargeBlock);    // остаток

    n2 = ostatok_str / SMALL_BLOCK_HEIGHT;                  // сколько маленьких блоков входит по высоте в строках
    n1 = DIV_MOD(ostatok_str, SMALL_BLOCK_HEIGHT);          // остаток

    // индексы для символа

    n_lb = sym_index / count_sym_LargeBlock;                 // сколько больших блоков входит по ширине до символа
    ostatok_sym = DIV_MOD(sym_index, count_sym_LargeBlock);  // остаток

    m2 = ostatok_sym / SMALL_BLOCK_WIDTH;                    // сколько маленьких блоков входит по ширине до символа
    m1 = DIV_MOD(ostatok_sym, SMALL_BLOCK_WIDTH);            // остаток

    b = strings[strings_i][n_lb][n2][m2][n1][m1];   // текущий символ в строке

    return b;
}

// Запись символа по строке и его индексу в строке
int put_sym_str(LargeBlock** strings, size_t vindex, size_t sym_index, char b) {
    size_t n_lb = 0, n2 = 0, m2 = 0, n1 = 0, m1 = 0;        // текущие индексы для вложенных массивов
    size_t strings_i, ostatok_str;                          // переменные для расчета индексов строки
    size_t ostatok_sym;                                     // переменная для расчета индексов символа

    // индексы для строки

    strings_i = vindex / count_str_LargeBlock;              // сколько больших блоков входит по высоте в строках
    ostatok_str = DIV_MOD(vindex, count_str_LargeBlock);    // остаток

    n2 = ostatok_str / SMALL_BLOCK_HEIGHT;                  // сколько маленьких блоков входит по высоте в строках
    n1 = DIV_MOD(ostatok_str, SMALL_BLOCK_HEIGHT);          // остаток

    // индексы для символа

    n_lb = sym_index / count_sym_LargeBlock;                 // сколько больших блоков входит по ширине до символа
    ostatok_sym = DIV_MOD(sym_index, count_sym_LargeBlock);  // остаток

    m2 = ostatok_sym / SMALL_BLOCK_WIDTH;                    // сколько маленьких блоков входит по ширине до символа
    m1 = DIV_MOD(ostatok_sym, SMALL_BLOCK_WIDTH);            // остаток

    strings[strings_i][n_lb][n2][m2][n1][m1] = b;            // сохранение текущего символа в строке

    return 1;
}


//**********************************************************************************
//                        Функции вывод строк
//**********************************************************************************

// вывод строки на экран
void print_string(LargeBlock** strings, size_t vindex) {
    size_t i = 0;                   // переменная цикла
    char b;

    while ((b = get_sym_str(strings, vindex, i)) != '\0') {
        printf("%c", b);            // выводим символ на экран
        i++;
    }
    printf("\n");                   // вывод признака конца строки на экран
}

// вывод строки в файл
void print_string_file(FILE* out_file, LargeBlock** strings, size_t vindex) {
    size_t i = 0;                   // переменная цикла
    char b;

    while ((b = get_sym_str(strings, vindex, i)) != '\0') {
        fwrite(&b, 1, 1, out_file);       // запись символа в файл
        i++;
    }
    // вывод признака конца строки в файл
    b = 0x0D;   fwrite(&b, 1, 1, out_file);   // \r      
    b = 0x0A;   fwrite(&b, 1, 1, out_file);   // \n
}


// вывод строк на экран
void print_strings(LargeBlock** strings, size_t* mas_index, size_t count_str) {
    printf("=========================================\n");
    for (int i = 0; i < count_str; i++)
        print_string(strings, mas_index[i]);
    printf("=========================================\n");
}

// вывод строк в файл
void print_strings_file(FILE* out_file, LargeBlock** strings, size_t* mas_index, size_t count_str) {
    for (int i = 0; i < count_str; i++)
        print_string_file(out_file, strings, mas_index[i]);
}

// Возвращаем файловый указатель (курсор) в начало файла
int fseek_begin(FILE* in_file) {
    if (fseek(in_file, 0, SEEK_SET) != 0) {   // SEEK_SET — константа, означающая смещение относительно начала файла на 0 байт
        fprintf(stderr, "Error: An error occurred when moving the file pointer to the beginning of the file.");
        return 1;
    }
    return 0;
}

// Получение индекса конца строки
int index_end_str(LargeBlock** strings, size_t i1) {
    int i = 0;
    while (get_sym_str(strings, i1, i) != '\0') i++;
    return i;
}

// Функция обмена двух элементов массива mas_index по индексам i1 и i2
void swap_mas_index(size_t* mas_index, size_t count_str, size_t i1, size_t i2) {
    size_t tmp;                 // tmp - переменная для обмена
    tmp = mas_index[i1];
    mas_index[i1] = mas_index[i2];
    mas_index[i2] = tmp;
}


//**********************************************************************************
//                             Функции сравнения
//**********************************************************************************

// Возвращаемое значение: 0 - перестановка не нужна, 1 - перестановка строк нужна.

// Сравнение строк в лексикографическом порядке с учётом регистра.
int cmp_str_alpha(LargeBlock** strings, size_t i1, size_t i2) {

    size_t cmp_i = 0;       // номера сравниваемых индексов в строках
    char a, b;              // сравниваемые символы двух строк: a для i1, b для i2

    while (1) {
        a = get_sym_str(strings, i1, cmp_i);
        b = get_sym_str(strings, i2, cmp_i);

        // Сравниваем коды символов (включая '\0')
        if (a > b) return 1;    // i1 больше i2, нужна перестановка
        if (a < b) return 0;    // i1 меньше i2, перестановка не нужна

        // Символы a и b равны
        // Если при этом a == '\0', значит обе строки закончились одновременно.
        if (a == '\0') return 0;    // строки полностью идентичны

        cmp_i++;    // символы равны, идем дальше
    }
}


// Сравнение строк в лексикографическом порядке без учёта регистра. 
int cmp_str_alpha_tolower(LargeBlock** strings, size_t i1, size_t i2) {    // i1, i2 - индексы сравниваемых строк

    size_t cmp_i = 0;       // номера сравниваемых индексов в строках
    char a, b;              // сравниваемые символы двух строк: a для i1, b для i2

    while (1) {
        a = tolower(get_sym_str(strings, i1, cmp_i));
        b = tolower(get_sym_str(strings, i2, cmp_i));

        // Сравниваем коды символов (включая '\0')
        if (a > b) return 1;    // i1 больше i2, нужна перестановка
        if (a < b) return 0;    // i1 меньше i2, перестановка не нужна

        // Символы a и b равны
        // Если при этом a == '\0', значит обе строки закончились одновременно.
        if (a == '\0') return 0;    // строки полностью идентичны

        cmp_i++;    // символы равны, идем дальше
    }
}


// Сравнение по длине строки, а при равенстве — лексикографическое c учетом регистра.
int cmp_str_length(LargeBlock** strings, size_t i1, size_t i2) {    // i1, i2 - индексы сравниваемых строк

    size_t len_i1 = index_end_str(strings, i1);       // длины строк 
    size_t len_i2 = index_end_str(strings, i2);

    if (len_i1 < len_i2) return 0;          // i1 строка < i2 строки. Перестановка не нужна.
    else if (len_i1 > len_i2) return 1;     // i1 строка > i2 строки. Перестановка нужна.
    // строки равны по длине, делаем лексикографическое сравнение строк 
    else return cmp_str_alpha(strings, i1, i2);
}


// Лексикографическая сортировка строк с конца
int cmp_str_end(LargeBlock** strings, size_t i1, size_t i2) {    // i1, i2 - индексы сравниваемых строк

    size_t cmp_i1 = index_end_str(strings, i1) - 1;       // номера сравниваемых индексов в строках с конца 
    size_t cmp_i2 = index_end_str(strings, i2) - 1;

    char a, b;                      // сравниваемые символы двух строк: a для i1, b для i2

    while (1) {
        if ((cmp_i1 == -1) && (cmp_i2 == -1)) return 0;    // Закончились обе строки одновременно, значит они равны, перестановка не нужна.
        else {
            if (cmp_i1 == -1) return 0;      // Закончилась только первая строка, i1 меньше i2, перестановка не нужна.
            if (cmp_i2 == -1) return 1;      // Закончилась только вторая строка, i1 больше i2, перестановка нужна.
        }
        // Строки не закончились
        a = get_sym_str(strings, i1, cmp_i1);
        b = get_sym_str(strings, i2, cmp_i2);

        // Сравниваем коды символов
        if (a > b) return 1;    // i1 больше i2, нужна перестановка
        if (a < b) return 0;    // i1 меньше i2, перестановка не нужна

        cmp_i1--;    // символы a и b равны, идем дальше
        cmp_i2--;
    }
}


//**********************************************************************************
//                             Функция сортировки
//**********************************************************************************

// Сортировка пузырьком
void sort_strings(LargeBlock** strings, size_t* mas_index, size_t count_str, int (*cmp_str_func)(LargeBlock**, size_t, size_t)) {
    size_t i, j;        // i, j - переменные цикла
    int flag_swap;      // flag_swap - флаг замены: 0 - не было замены, 1 - была замена.

    for (i = 0; i < count_str - 1; i++) {       // сортировка пузырьком
        flag_swap = 0;
        for (j = 0; j < count_str - i - 1; j++) {
            if (cmp_str_func(strings, mas_index[j], mas_index[j + 1])) {    // j-ая строка > j+1 строки
                swap_mas_index(mas_index, count_str, j, j + 1);             // меняем местами индексы строк в массиве индексов mas_index
                flag_swap = 1;                  // произошла замена
            }
        }
        if (flag_swap == 0) break;  // если за полный проход по массиву не было замен, массив отсортирован, прекращаем сортировку
    }
}


// Вспомогательная функция для быстрой сортировки
// Нахождение индекса опорного элемента, по которому разделяем массив на две части: 
//  - слева все элементы меньше и равные опорному элементу; (определяются возвращаемым значением return 0 из функции сравнения двух строк)
//  - справа все элементы больше опорного элемента.
// min_index и max_index - границы сортируемой области массива strings
size_t support_func(LargeBlock** strings, size_t* mas_index, size_t count_str, size_t min_index, size_t max_index, int (*cmp_str_func)(LargeBlock**, size_t, size_t)) {

    size_t opor_index = max_index;              // Выбираем за опорный элемент правый элемент области.
    size_t cur_write_index = min_index;     // cur_write_index - текущий индекс для записи очередного найденного элемента, меньшего или равного опорному. 

    for (size_t i = min_index; i < max_index; i++) {  // Исключаем последний элемент области из обработки, он зарезервирован под опорный элемент.
        if (cmp_str_func(strings, mas_index[i], mas_index[opor_index]) == 0) {    // если i-ая строка <= опорной
            swap_mas_index(mas_index, count_str, cur_write_index, i);             // меняем местами индексы строк в массиве индексов mas_index
            cur_write_index++;
        }
    }
    swap_mas_index(mas_index, count_str, cur_write_index, opor_index);        // Размещаем опорный элемент на правильной позиции (после всех меньших и равных, перед большими)

    return cur_write_index;              // новое местоположение опорного элемента
}

// Быстрая сортировка (через рекурсию)
void sort_strings_fast(LargeBlock** strings, size_t* mas_index, size_t count_str, size_t min_index, size_t max_index, int (*cmp_str_func)(LargeBlock**, size_t, size_t)) {

    if (min_index < max_index) {            // Если больше одного элемента в сортируемой области. Если 0 или 1 элемент, то массив отсортирован.
        size_t opor_index = support_func(strings, mas_index, count_str, min_index, max_index, cmp_str_func);    // Индекс опорного элемента
        if (opor_index > min_index)
            sort_strings_fast(strings, mas_index, count_str, min_index, opor_index - 1, cmp_str_func);              // Сортируем левую часть от опорного элемента
        if (opor_index < max_index)
            sort_strings_fast(strings, mas_index, count_str, opor_index + 1, max_index, cmp_str_func);              // Сортируем правую часть от опорного элемента  
    }
}




//************************************************************************
//                              Основная функция
//************************************************************************

int process_file(TArg arg_prog) {

    if (strcmp(arg_prog.type_proc, "test") == 0) {                  // arg_prog.type_proc = "test" - генерация файла строк
        // Инициализируем генератор случайных чисел текущим временем
        srand((unsigned int)time(NULL));

        generate_random_strings_to_file(arg_prog.input_file, arg_prog.count_str, arg_prog.max_size_str);
        return 0;
    }

    int (*cmp_str_func)(LargeBlock**, size_t, size_t) = cmp_str_alpha;    // переменная для указателя на функцию для выбора типа сравнения строк

    if (strcmp(arg_prog.type_proc, "sort") == 0)              // arg_prog.type_proc = "sort" - сортировка строк
        switch (arg_prog.type_cmp) {
        case 1:     // 1 - сортировка в лексикографическом порядке с учётом регистра
            cmp_str_func = cmp_str_alpha;
            break;
        case 2:     // 2 - сортировка в лексикографическом порядке буз учёта регистра
            cmp_str_func = cmp_str_alpha_tolower;
            break;
        case 3:     // 3 - сортировка по длине строки, а при равенстве — лексикографическая c учетом регистра
            cmp_str_func = cmp_str_length;
            break;
        case 4:     // 4 - лексикографическая сортировка строк с конца
            cmp_str_func = cmp_str_end;
            break;
        default:
            break;
        }


    FILE* in_file = fopen(arg_prog.input_file, "rb");
    if (in_file == NULL) {
        fprintf(stderr, "Error: Cannot open input file %s\n", arg_prog.input_file);
        return 1;
    }

    FILE* out_file = fopen(arg_prog.output_file, "wb");
    if (out_file == NULL) {
        fprintf(stderr, "Error: Cannot create output file %s\n", arg_prog.output_file);
        fclose(in_file);
        return 1;
    }

    printf("Files opened successfully.\n");


    char buf[N];                    // buf - считываемый блок (массив) символов из файла 
    size_t b_read;                  // b_read - количество считанных символов в блоке
    char b = 0;                     // b - текущий символ
    size_t i;                       // i - переменная цикла
    size_t strings_i = 0;           // strings_i - переменная цикла для массива strings
    size_t count_str = 0;           // count_str - количество строк в файле
    int flag_has_data = 0;          // был ли хоть один байт в файле


    //**********************************************************************************************//
    //                                                                                              // 
    //                   Выделяем память под шестимерный массив strings.                            //
    //                                                                                              // 
    //**********************************************************************************************//

    //==============================================================================================
    //                 Считаем количество строк во входном файле - первый проход файла
    //==============================================================================================

    char pred_b = 0;    // предыдущий считанный символ

    while ((b_read = fread(buf, 1, N, in_file)) > 0) {
        flag_has_data = 1;      // flag_has_data = 1 - есть данные в файле
        for (i = 0; i < b_read; i++) {
            b = buf[i];             // b - текущий (рассматриваемый) символ из считанного блока 

            // Если встретили \n и перед ним был \r — это конец строки
            if (b == 0x0A && pred_b == 0x0D) count_str++;        // 0x0D - символ возврата каретки \r
            // 0x0A - символ перевода строки \n
            pred_b = b;
        }
    }
    if (flag_has_data) { // Если файл не пустой 
        // И поcледний символ не перевод строки
        if (pred_b != 0x0A) count_str++;              // добавляем в счетчик строк +1
    }

    size_t count_LargeBlock;                    // count_LargeBlock -  количество групп LargeBlock для хранения количества строк

    count_LargeBlock = count_str / count_str_LargeBlock;                    // Считаем сколько целых блоков набирается из строк
    if (DIV_MOD(count_str, count_str_LargeBlock) > 0) count_LargeBlock++;   // Если есть хвостик из строк, то добавляем ещё один блок

    // Выделяем память под одномерный массив указателей (под count_LargeBlock указателей)
    LargeBlock** strings = (LargeBlock**)malloc(count_LargeBlock * sizeof(LargeBlock*));

    if (strings == NULL) { fprintf(stderr, "Error: Failed to allocate memory for array strings."); return 1; }

    fseek_begin(in_file);       // Возвращаем файловый указатель (курсор) в начало файла


    //=========================================================================================
    //                 Считаем длины строк во входном файле - второй проход файла
    //=========================================================================================

    size_t cur_len_str = 0;                        // cur_len_str - текущая длина строки
    size_t max_len_str = 0;                        // max_len_str - максимальная длина строки в блоке LargeBlock
    size_t cur_count_str = 0;                      // cur_count_str - текущее количество строк в блоке LargeBlock
    size_t count_lb = 0;                           // count_lb - количество элементов в блкое

    flag_has_data = 0;                              // пока не считано ни одного байта из файла

    while ((b_read = fread(buf, 1, N, in_file)) > 0) {
        flag_has_data = 1;                      // появились данные
        for (i = 0; i < b_read; i++) {
            b = buf[i];                         // b - текущий (рассматриваемый) символ из считанного блока buf

            // Конец строки
            if (b == 0x0A) {
                cur_count_str++;               // увеличиваем счетчик текущих строк в блоке
                if (cur_len_str > max_len_str) max_len_str = cur_len_str; // Сверяем длину текущей строки с максимальной длиной строки в блоке
                cur_len_str = 0;               // Обнуляем счетчмк символов для следующей строки

                if (cur_count_str == count_str_LargeBlock) {    // Набралось количество строк на блок
                    count_lb = max_len_str / count_sym_LargeBlock;                    // Считаем сколько целых блоков по горизонтали набирается из символов
                    if (DIV_MOD(max_len_str, count_sym_LargeBlock) > 0) count_lb++;   // Если есть хвостик из символов, то добавляем ещё один блок вправо

                    // Выделяем память под массив блоков
                    strings[strings_i] = (LargeBlock*)malloc(count_lb * sizeof(LargeBlock));

                    if (strings[strings_i] == NULL) { fprintf(stderr, "Error: Failed to allocate memory for array strings."); return 1; }

                    strings_i++;    // изменяем переменную цикла для массива strings на единицу

                    cur_count_str = 0;               // обнуляем текущее количество строк для следующего блока
                    max_len_str = 0;                 // обнуляем максимальную длину строки для следующего блока
                }
            }
            else cur_len_str++;         // строка ещё не закончилась, +1 в количество символов в строке
        }
    }
    // Если файл не пустой 
    if (flag_has_data) {
        if (cur_len_str > 0) {     // и последняя строка не закончена
            cur_count_str++;           // текущее количество строк
            cur_len_str++;             // текущая длина строки + 1 под символ '\0'
            if (cur_len_str > max_len_str) max_len_str = cur_len_str;    // Сверяем длину текущей строки с максимальной длиной строки в блоке
        }
        if ((cur_count_str > 0) && (cur_count_str <= count_str_LargeBlock)) {    // Хвостик строк на блок
            count_lb = max_len_str / count_sym_LargeBlock;                    // Считаем сколько целых блоков набирается из символов
            if (DIV_MOD(max_len_str, count_sym_LargeBlock) > 0) count_lb++;   // Если есть хвостик из символов, то добавляем ещё один блок вправо

            // Выделяем память под массив блоков
            strings[strings_i] = (LargeBlock*)malloc(count_lb * sizeof(LargeBlock));

            if (strings[strings_i] == NULL) { fprintf(stderr, "Error: Failed to allocate memory for array strings."); return 1; }
        }
    }

    fseek_begin(in_file);       // Возвращаем файловый указатель (курсор) в начало файла

    //================================================================================================================================================= 
    //                                  Инициализируем массив - третий проход файла
    //================================================================================================================================================= 
    // strings      [ strings_i ]       [ n_lb ]          [ n2 ]                    [ m2 ]                   [ n1 ]                    [ m1 ]
    //                    0                 0                0                         0                        0                         0
    //                   ...               ...              ...                       ...                      ...                       ...
    //          count_LargeBlock - 1                LARGE_BLOCK_HEIGHT - 1    LARGE_BLOCK_WIDTH - 1    SMALL_BLOCK_HEIGHT - 1    SMALL_BLOCK_WIDTH - 1
    //=================================================================================================================================================

    size_t vindex = 0, sym_index = 0;               // текущий номер строки и символа

    flag_has_data = 0;                              // пока не считано ни одного байта из файла

    while ((b_read = fread(buf, 1, N, in_file)) > 0) {
        flag_has_data = 1;                      // появились данные

        for (i = 0; i < b_read; i++) {
            b = buf[i];                             // b - текущий (рассматриваемый) символ из считанного блока buf

            // Конец строки
            if (b == 0x0A) {
                put_sym_str(strings, vindex, sym_index, '\0');   // в массив записали символ конца строки '\0'
                vindex++;           // меняем индекс строки на следующий,
                sym_index = 0;      // а индекс символа обнуляем    
            }
            // Cтрока ещё не закончилась
            else {
                put_sym_str(strings, vindex, sym_index, b);    // текущий символ записали в массив
                sym_index++;                                   // изменяем номер индекса для записи следующего символа
            }
        }
    }
    // Если файл не пустой и последняя строка не завершена
    if (flag_has_data && (sym_index > 0)) {
        put_sym_str(strings, vindex, sym_index, '\0');  // записываем в массив признак конца строки
    }


    //**********************************************************************************************//
    //                                                                                              // 
    //                   Создание массива индекса строк для сортировок                              //
    //                                                                                              // 
    //**********************************************************************************************//

    // Выделяем память под одномерный массив индексов mas_index
    size_t* mas_index = (size_t*)malloc(count_str * sizeof(size_t));

    if (mas_index == NULL) { fprintf(stderr, "Error: Failed to allocate memory for array indexes."); return 1; }

    // Инициализируем его значениями от 0 до count_str - 1
    for (i = 0; i < count_str; i++)     mas_index[i] = i;


    if (strcmp(arg_prog.type_sort, "slow") == 0) {              // arg_prog.type_sort = "slow" - сортировка пузырьком
        sort_strings(strings, mas_index, count_str, cmp_str_func);
    }
    else if (strcmp(arg_prog.type_sort, "fast") == 0) {         // arg_prog.type_sort = "fast" - быстрая сортировка
        sort_strings_fast(strings, mas_index, count_str, 0, count_str - 1, cmp_str_func);
    }


    print_strings(strings, mas_index, count_str);                  // вывод строк на экран
    print_strings_file(out_file, strings, mas_index, count_str);   // вывод строк в файл 


    for (strings_i = 0; strings_i < count_LargeBlock; strings_i++) free(strings[strings_i]);   // Очищаем память из под элементов массива strings   
    free(strings);          // очищаем память из под массива указателей strings
    fclose(in_file);
    fclose(out_file);
    return 0;


}