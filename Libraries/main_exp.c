#include <stdio.h>
#include <dlfcn.h>
#include "add.h"
#include "mul.h"

int main() {
    void *handle;
    int32_t (*add)(int32_t, int32_t);
    float (*mul)(float, float);

    handle = dlopen("libmath.so", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "%s\n", dlerror());
        return 1;
    }

    add = dlsym(handle, "add");
    printf("%d\n", (*add)(2, 4));

    mul = dlsym(handle, "mul");
    printf("%f\n", (*mul)(2.0, 4.0));

    dlclose(handle);
    return 0;
}