#include <cstdio>


extern "C" {
    void inject_func(char *func_name);
}
void inject_func(char *func_name) {
    printf("%s\n",func_name);
}