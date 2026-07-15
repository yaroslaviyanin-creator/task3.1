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

// вывод строки на экран
void print_string(LargeBlock** strings, size_t vindex) {
    size_t n_lb = 0, n2 = 0, m2 = 0, n1 = 0, m1 = 0;                // текущие индексы для вложенных массивов
    size_t strings_i, ostatok_str, ostatok_str_n2, ostatok_str_n1;  // переменные для расчета индексов строки
    char b;

    // индексы для строки

    strings_i = vindex / count_str_LargeBlock;              // сколько больших блоков входит по высоте в строках
    ostatok_str = DIV_MOD(vindex, count_str_LargeBlock);    // остаток

    n2 = ostatok_str / SMALL_BLOCK_HEIGHT;                  // сколько маленьких блоков входит по высоте в строках
    n1 = DIV_MOD(ostatok_str, SMALL_BLOCK_HEIGHT);          // остаток

    printf("vindex = %zd, strings_i = %zd, ostatok_str = %zd, n2 = %zd, n1 = %zd     ", vindex, strings_i, ostatok_str, n2, n1);
    do {
        b = strings[strings_i][n_lb][n2][m2][n1][m1];   // текущий символ в строке
        printf("%c", b);                                // выводим символ на экран

        // изменяем координаты по горизонтали для доступа к следующему символу строки
        if (m1 + 1 == SMALL_BLOCK_WIDTH) {      // Если в SmallBlock индекс по столбцам достиг правой границы,
            m2++;                               // то переходим в новый маленький блок
            m1 = 0;                             // на его начало по столбцам
            if (m2 + 1 == LARGE_BLOCK_WIDTH) {  // Преодолели границу по ширине в большом блоке
                m2 = 0;
                n_lb++;
            }
        }
        else {           // Если в SmallBlock индекс по столбцам не достиг правой границы,
            m1++;        // то сдвигаемся в этом же маленьком блоке правее   
        }
    } while (b != '\0');
    printf("\n");        // вывод признака конгца строки на экран
}


// вывод строк на экран
void print_strings(LargeBlock** strings, size_t* mas_index, size_t count_str) {
    for (int i = 0; i < count_str; i++) {
        printf("mas_index[%zd] = %zd     ", i, mas_index[i]);
        print_string(strings, mas_index[i]);
    }
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

    printf("count_str_LargeBlock = %zd\n", count_str_LargeBlock);
    printf("count_sym_LargeBlock = %zd\n", count_sym_LargeBlock);

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

    printf("\n\ncount_str = %zd", count_str);

    size_t count_LargeBlock;                    // count_LargeBlock -  количество групп LargeBlock для хранения количества строк
    count_LargeBlock = count_str / count_str_LargeBlock;            // Считаем сколько целых блоков набирается из строк
    if (count_str % count_str_LargeBlock > 0) count_LargeBlock++;   // Если есть хвостик из строк, то добавляем ещё один блок

    printf("\ncount_LargeBlock = %zd\n\n", count_LargeBlock);

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
            printf("%c", b);
            if (b == 0x0A) {                        // 0x0A - символ перевода строки. Строка закончилась.
                cur_count_str++;                    // увеличиваем счетчик текущих строк в блоке
                printf("cur_count_str = %zd    ", cur_count_str);
                printf("cur_len_str = %zd    ", cur_len_str);
                if (cur_len_str > max_len_str) {    // Сверяем длину текущей строки с максимальной длиной строки в блоке
                    max_len_str = cur_len_str;
                    printf("max_len_str = %zd    ", max_len_str);
                }

                cur_len_str = 0;                    // Обнуляем счетчмк символов для следующей строки

                if (cur_count_str == count_str_LargeBlock) {    // Набралось количество строк на блок
                    printf("\nNabnralos strok na block!");

                    count_lb = max_len_str / count_sym_LargeBlock;            // Считаем сколько целых блоков набирается из символов
                    if (max_len_str % count_sym_LargeBlock > 0) count_lb++;   // Если есть хвостик из символов, то добавляем ещё один блок вправо
                    printf("            count_lb = %zd    \n", count_lb);

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
        printf("\ncur_count_str = %zd    ", cur_count_str);
        cur_len_str++;
        printf("cur_len_str = %zd    ", cur_len_str);
        if (cur_len_str > max_len_str) {    // Сверяем длину текущей строки с максимальной длиной строки в блоке
            max_len_str = cur_len_str;
            printf("max_len_str = %zd    ", max_len_str);
        }
        if (cur_count_str <= count_str_LargeBlock) {    // Хвостик строк на блок
            printf("\nNabnralos strok na block!");

            count_lb = max_len_str / count_sym_LargeBlock;            // Считаем сколько целых блоков набирается из символов
            if (max_len_str % count_sym_LargeBlock > 0) count_lb++;   // Если есть хвостик из символов, то добавляем ещё один блок вправо
            printf("            count_lb = %zd    \n", count_lb);

            // Выделяем память под массив блоков

            strings[strings_i] = (LargeBlock*)malloc(count_lb * sizeof(LargeBlock));
            if (strings[strings_i] == NULL) {
                fprintf(stderr, "Error: Failed to allocate memory for array strings.");
                return 1;
            }
        }
    }

    printf("\n\n");
    fseek_begin(in_file);       // Возвращаем файловый указатель (курсор) в начало файла

    //================================================================================================================================================= 
    //                                  Инициализируем массив - третий проход файла
    //================================================================================================================================================= 
    // strings      [ strings_i ]       [ n_lb ]          [ n2 ]                    [ m2 ]                   [ n1 ]                    [ m1 ]
    //                    0                 0                0                         0                        0                         0
    //                   ...               ...              ...                       ...                      ...                       ...
    //          count_LargeBlock - 1                LARGE_BLOCK_HEIGHT - 1    LARGE_BLOCK_WIDTH - 1    SMALL_BLOCK_HEIGHT - 1    SMALL_BLOCK_WIDTH - 1
    //=================================================================================================================================================

    size_t n_lb = 0, n2 = 0, m2 = 0, n1 = 0, m1 = 0;          // текущие индексы для вложенных массивов
    strings_i = 0;

    while ((b_read = fread(buf, 1, N, in_file)) > 0) {
        for (i = 0; i < b_read; i++) {
            b = buf[i];                             // b - текущий (рассматриваемый) символ из считанного блока buf

            //printf("[%zd][%zd][%zd][%zd][%zd][%zd] b = %c\n", strings_i, n_lb, n2, m2, n1, m1, b);
            if (b == 0x0D) {                        // Строка заканчивается
                strings[strings_i][n_lb][n2][m2][n1][m1] = '\0';     // в массив записали символ конца строки '\0'
            }
            else if (b == 0x0A) { // Строка закончилась, пересчитываем координаты для начала новой строки
                m2 = 0;         // Начало новой строки
                m1 = 0;
                n_lb = 0;
                if (n1 + 1 == SMALL_BLOCK_HEIGHT) {         // Преодолели границу по высоте в маленьком блоке
                    n1 = 0;
                    if (n2 + 1 == LARGE_BLOCK_HEIGHT) {     // Преодолели границу по высоте в большом блоке
                        n2 = 0;
                        strings_i++;
                    }
                    else n2++;
                }
                else n1++;
            }
            else {              // строка ещё не закончилась
                strings[strings_i][n_lb][n2][m2][n1][m1] = b;     // текущий символ записали в массив

                // изменяем координаты по горизонтали для записи следующего символа
                if (m1 + 1 == SMALL_BLOCK_WIDTH) {      // Если в SmallBlock индекс по столбцам достиг правой границы,
                    m2++;                               // то переходим в новый маленький блок
                    m1 = 0;  // на его начало по столбцам
                    if (m2 + 1 == LARGE_BLOCK_WIDTH) {  // Преодолели границу по ширине в большом блоке
                        m2 = 0;
                        n_lb++;
                    }
                }
                else {           // Если в SmallBlock индекс по столбцам не достиг правой границы,
                    m1++;        // то сдвигаемся в этом же маленьком блоке правее   
                }
            }
        }
    }
    // Если при окончании входного файла последний символ не 0x0A, т.е. есть последняя строка без завершающих символов конца строки
    if ((b != 0x0A)) strings[strings_i][n_lb][n2][m2][n1][m1] = '\0';

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

