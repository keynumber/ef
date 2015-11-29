#include <stdio.h>

#include <dlfcn.h>

typedef int (*net_complete_func)(const char *);

int LoadFunc(const char * so_path, const char * func_name) {
    void *handler = dlopen(so_path, RTLD_LAZY);
    if (!handler) {
        printf("%s, errmsg: %s\n", "dlopen failed", dlerror());
        return 0;
    }

    net_complete_func func = (net_complete_func)dlsym(handler, func_name);
    if (!func) {
        printf("%s, errmsg: %s\n", "dlsys failed", dlerror());
        dlclose(handler);
        return 0;
    }

    func("");
}

int main() {
    LoadFunc("./net_complete.so", "net_complete_func");
    return 0;
}
