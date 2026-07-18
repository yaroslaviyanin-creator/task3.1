/*
test_lib.c - модуль проверки библиотеки.
Янин Ярослав Иванович
Группа МК-101
*/


#include "unity.h"
#include "lib_main.h"
#include <stdlib.h>

LargeBlock** test_strings = NULL;
size_t* test_mas_index = NULL;

void setUp(void) {
    // Выделяем 1 минимальный блок (вмещает 6 строк по 12 символов) для тестов
    test_strings = (LargeBlock**)malloc(1 * sizeof(LargeBlock*));
    test_strings[0] = (LargeBlock*)malloc(1 * sizeof(LargeBlock));

    // Выделяем память под массив индексов
    test_mas_index = (size_t*)malloc(6 * sizeof(size_t));
    for (int i = 0; i < 6; i++) test_mas_index[i] = i;
}

void tearDown(void) {
    free(test_strings[0]);
    free(test_strings);
    free(test_mas_index);
}

// Вспомогательная функция для быстрой записи обычной строки в 6-мерную структуру
void helper_write_string(size_t vindex, const char* str) {
    size_t i = 0;
    while (str[i] != '\0') {
        put_sym_str(test_strings, vindex, i, str[i]);
        i++;
    }
    put_sym_str(test_strings, vindex, i, '\0');
}

// Проверка записи и чтения одного символа
void test_put_get_sym(void) {
    put_sym_str(test_strings, 0, 0, 'X');
    put_sym_str(test_strings, 0, 1, '\0');
    TEST_ASSERT_EQUAL_CHAR('X', get_sym_str(test_strings, 0, 0));
    TEST_ASSERT_EQUAL_CHAR('\0', get_sym_str(test_strings, 0, 1));
}

// Проверка функции расчета длины строки
void test_index_end_str(void) {
    helper_write_string(0, "Hello");
    TEST_ASSERT_EQUAL_INT(5, index_end_str(test_strings, 0));
}

// Обмен элементов массива индексов
void test_swap_mas_index(void) {
    swap_mas_index(test_mas_index, 6, 0, 1);
    TEST_ASSERT_EQUAL_INT(1, test_mas_index[0]);
    TEST_ASSERT_EQUAL_INT(0, test_mas_index[1]);
}

// Сравнение с учетом регистра
void test_cmp_str_alpha(void) {
    helper_write_string(0, "abc");
    helper_write_string(1, "abd");
    helper_write_string(2, "abc");

    TEST_ASSERT_EQUAL_INT(0, cmp_str_alpha(test_strings, 0, 1)); // abc < abd (перестановка не нужна)
    TEST_ASSERT_EQUAL_INT(1, cmp_str_alpha(test_strings, 1, 0)); // abd > abc (нужна перестановка)
    TEST_ASSERT_EQUAL_INT(0, cmp_str_alpha(test_strings, 0, 2)); // равны
}

// Сравнение без учета регистра
void test_cmp_str_alpha_tolower(void) {
    helper_write_string(0, "aBc");
    helper_write_string(1, "AbC");
    helper_write_string(2, "Abd");

    TEST_ASSERT_EQUAL_INT(0, cmp_str_alpha_tolower(test_strings, 0, 1)); // aBc == AbC
    TEST_ASSERT_EQUAL_INT(0, cmp_str_alpha_tolower(test_strings, 0, 2)); // aBc < Abd
}

// Сравнение по длине строки
void test_cmp_str_length(void) {
    helper_write_string(0, "a");
    helper_write_string(1, "ab");

    TEST_ASSERT_EQUAL_INT(0, cmp_str_length(test_strings, 0, 1)); // len 1 < len 2
    TEST_ASSERT_EQUAL_INT(1, cmp_str_length(test_strings, 1, 0)); // len 2 > len 1
}

// Сравнение строк с конца
void test_cmp_str_end(void) {
    helper_write_string(0, "xbc");
    helper_write_string(1, "abc");

    // xbc vs abc с конца: c==c, b==b, x > a
    TEST_ASSERT_EQUAL_INT(1, cmp_str_end(test_strings, 0, 1));
    TEST_ASSERT_EQUAL_INT(0, cmp_str_end(test_strings, 1, 0));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_put_get_sym);
    RUN_TEST(test_index_end_str);
    RUN_TEST(test_swap_mas_index);
    RUN_TEST(test_cmp_str_alpha);
    RUN_TEST(test_cmp_str_alpha_tolower);
    RUN_TEST(test_cmp_str_length);
    RUN_TEST(test_cmp_str_end);
    return UNITY_END();
}