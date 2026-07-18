/*
test_app.c - модуль проверки программы.
Янин Ярослав Иванович
Группа МК-101
*/

#include "unity.h"
#include "lib_main.h"

void setUp(void) {}
void tearDown(void) {}

// Передача несуществующего файла должна возвращать код ошибки 1
void test_process_file_invalid_input(void) {
    TArg arg_prog;
    arg_prog.input_file = "non_existent_file_12345.txt";
    arg_prog.output_file = "out.txt";
    arg_prog.type_proc = "sort";
    arg_prog.type_sort = "fast";
    arg_prog.type_cmp = 1;

    int res = process_file(arg_prog);
    TEST_ASSERT_EQUAL_INT(1, res);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_process_file_invalid_input);
    return UNITY_END();
}
