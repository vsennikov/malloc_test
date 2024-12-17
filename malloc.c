#define _GNU_SOURCE
#include <dlfcn.h>
#include <errno.h>
#include <stdlib.h>

typedef void *(*f)(size_t size);

void *malloc(size_t size)
{
    static void *(*og_malloc)(size_t) = NULL;
    static int calls = 0;

    if (!og_malloc)
    {
        og_malloc = (f)dlsym(RTLD_NEXT, "malloc");
    }
    calls++;
    if (calls <= 10)
    {
        return (og_malloc(size));
    }
    errno = ENOMEM;
    return (NULL);
}


//gcc -fPIC -shared -o "malloc.so" "malloc.c" -ldl
// LD_PRELOAD="./malloc.so" ./{program name}`