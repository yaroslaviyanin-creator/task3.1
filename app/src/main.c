#include <stdlib.h>
#include <stdio.h>

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

size_t count_str_LargeBlock = SMALL_BLOCK_HEIGHT * LARGE_BLOCK_HEIGHT;      // count_str_LargeBlock - количество строк в блоке
size_t count_sym_LargeBlock = SMALL_BLOCK_WIDTH * LARGE_BLOCK_WIDTH;        // count_sym_LargeBlock - количество символов в блоке



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
    b = 0x0D;   fwrite(&b, 1, 1, out_file);         // вывод признака конца строки в файл
    b = 0x0A;   fwrite(&b, 1, 1, out_file);
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
    char b = 0x0A;                  // b - текущий символ
    size_t i;                       // i - переменная цикла
    size_t strings_i = 0;           // strings_i - переменная цикла для массива strings
    size_t count_str = 0;           // count_str - количество строк в файле



    //**********************************************************************************************//
    //                                                                                              // 
    //                   Выделяем память под шестимерный массив strings.                            //
    //                                                                                              // 
    //**********************************************************************************************//

    //==============================================================================================
    //                 Считаем количество строк во входном файле - первый проход файла
    //==============================================================================================

    while ((b_read = fread(buf, 1, N, in_file)) > 0) {
        for (i = 0; i < b_read; i++) {
            b = buf[i];                        // b - текущий (рассматриваемый) символ из считанного блока 
            if (b == 0x0A) count_str++;        // 0x0D - символ возврата каретки
            // 0x0A - символ перевода строки
            // Признак конца строки:
            // в Unix-системах — один символ 0x0A;
            // в Windows — пара символов 0x0D и 0x0A.
        }
    }
    // Если при окончании входного файла последний символ не 0x0A, т.е. есть последняя строка без завершающих символов конца строки, то
    if ((b != 0x0A)) count_str++;              // добавляем в счетчик строк +1

    size_t count_LargeBlock;                    // count_LargeBlock -  количество групп LargeBlock для хранения количества строк
    count_LargeBlock = count_str / count_str_LargeBlock;            // Считаем сколько целых блоков набирается из строк
    if (DIV_MOD(count_str, count_str_LargeBlock) > 0) count_LargeBlock++;   // Если есть хвостик из строк, то добавляем ещё один блок

    // Выделяем память под одномерный массив указателей (под count_LargeBlock указателей)
    LargeBlock** strings = (LargeBlock**)malloc(count_LargeBlock * sizeof(LargeBlock*));

    if (strings == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for array strings.");
        return 1;
    }

    fseek_begin(in_file);       // Возвращаем файловый указатель (курсор) в начало файла


    //=========================================================================================
    //                 Считаем длины строк во входном файле - второй проход файла
    //=========================================================================================

    size_t cur_len_str = 0;                        // cur_len_str - текущая длина строки
    size_t max_len_str = 0;                        // max_len_str - максимальная длина строки в блоке LargeBlock
    size_t cur_count_str = 0;                      // cur_count_str - текущее количество строк в блоке LargeBlock
    size_t count_lb = 0;                           // count_lb - количество элементов в блкое

    while ((b_read = fread(buf, 1, N, in_file)) > 0) {
        for (i = 0; i < b_read; i++) {
            b = buf[i];                             // b - текущий (рассматриваемый) символ из считанного блока buf

            if (b == 0x0A) {                        // 0x0A - символ перевода строки. Строка закончилась.
                cur_count_str++;                    // увеличиваем счетчик текущих строк в блоке
                if (cur_len_str > max_len_str) {    // Сверяем длину текущей строки с максимальной длиной строки в блоке
                    max_len_str = cur_len_str;
                }

                cur_len_str = 0;                    // Обнуляем счетчмк символов для следующей строки

                if (cur_count_str == count_str_LargeBlock) {    // Набралось количество строк на блок
                    count_lb = max_len_str / count_sym_LargeBlock;                    // Считаем сколько целых блоков набирается из символов
                    if (DIV_MOD(max_len_str, count_sym_LargeBlock) > 0) count_lb++;   // Если есть хвостик из символов, то добавляем ещё один блок вправо

                    // Выделяем память под массив блоков
                    strings[strings_i] = (LargeBlock*)malloc(count_lb * sizeof(LargeBlock));
                    if (strings[strings_i] == NULL) {
                        fprintf(stderr, "Error: Failed to allocate memory for array strings.");
                        return 1;
                    }
                    strings_i++;    // изменяем переменную цикла для массива strings на единицу

                    cur_count_str = 0;                          // обнуляем текущее количество строк для следующего блока
                    max_len_str = 0;                            // обнуляем максимальную длину строки для следующего блока
                }
            }
            else {              // строка ещё не закончилась
                cur_len_str++;  // +1 в количество символов в строке
            }
        }
    }
    // Если при окончании входного файла последний символ не 0x0A, т.е. есть последняя строка без завершающих символов конца строки, то
    if ((b != 0x0A)) {             // добавляем в счетчик символов последней текущей строки +1
        cur_count_str++;
        cur_len_str++;
        if (cur_len_str > max_len_str) max_len_str = cur_len_str;    // Сверяем длину текущей строки с максимальной длиной строки в блоке

        if (cur_count_str <= count_str_LargeBlock) {    // Хвостик строк на блок
            count_lb = max_len_str / count_sym_LargeBlock;                    // Считаем сколько целых блоков набирается из символов
            if (DIV_MOD(max_len_str, count_sym_LargeBlock) > 0) count_lb++;   // Если есть хвостик из символов, то добавляем ещё один блок вправо

            // Выделяем память под массив блоков

            strings[strings_i] = (LargeBlock*)malloc(count_lb * sizeof(LargeBlock));

            if (strings[strings_i] == NULL) {
                fprintf(stderr, "Error: Failed to allocate memory for array strings.");
                return 1;
            }
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

    size_t vindex = 0, sym_index = 0;                          // текущий номер строки и символа

    while ((b_read = fread(buf, 1, N, in_file)) > 0)
        for (i = 0; i < b_read; i++) {
            b = buf[i];                             // b - текущий (рассматриваемый) символ из считанного блока buf

            if (b == 0x0D) {                        // Строка заканчивается
                put_sym_str(strings, vindex, sym_index, '\0');   // в массив записали символ конца строки '\0'
            }
            else if (b == 0x0A) {   // Строка закончилась:
                vindex++;           // меняем индекс строки на следующий,
                sym_index = 0;      // а индекс символа обнуляем    
            }
            else {                  // строка ещё не закончилась
                put_sym_str(strings, vindex, sym_index, b);    // текущий символ записали в массив
                sym_index++;                                   // изменяем номер индекса для записи следующего символа
            }
        }

    // Если при окончании входного файла последний символ не 0x0A, т.е. есть последняя строка без завершающих символов конца строки
    if ((b != 0x0A)) put_sym_str(strings, vindex, sym_index, '\0');  // записываем в массив признак конца строки

    //**********************************************************************************************//
    //                                                                                              // 
    //                   Создание массива индекса строк для сортировок                              //
    //                                                                                              // 
    //**********************************************************************************************//

    // Выделяем память под одномерный массив индексов mas_index
    size_t* mas_index = (size_t*)malloc(count_str * sizeof(size_t));
    // Инициализируем его значениями от 0 до count_str - 1
    for (i = 0; i < count_str; i++)     mas_index[i] = i;

    print_strings(strings, mas_index, count_str);
    print_strings_file(out_file, strings, mas_index, count_str);

    // GenerateRandomStrings ();


    // SortStrings (strings, 0);
    // PrintStrings (strings);

     // очищаем память из под элементов массива strings
     //for (strings_i = 0; strings_i < count_LargeBlock; strings_i++) free(strings[strings_i]);        

    free(strings);          // очищаем память из под массива указателей strings
    fclose(in_file);
    fclose(out_file);
    return 0;
}

