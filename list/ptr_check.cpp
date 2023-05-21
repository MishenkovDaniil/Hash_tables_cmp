#include "ptr_check.h"

int is_bad_ptr (void *p)
{
    MEMORY_BASIC_INFORMATION mbi = {};

    const int PROTECT_MASK = PAGE_EXECUTE | PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY;

    if (!(VirtualQuery (p, &mbi, sizeof (mbi))))
    {
        return 1;
    }
    else if (!(mbi.Protect & PROTECT_MASK))
    {
        return 1;
    }

    return 0;
}
