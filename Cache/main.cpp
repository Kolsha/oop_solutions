#include <iostream>

#include "cache.h"

using namespace std;



int main(int argc, char *argv[])
{
    Kolsha::FileCache file_cache;
    Kolsha::NullCache null_cache;
    Kolsha::CacheApplier cache(file_cache);
    cache.set_strategy(null_cache);

    cache.write_to_cache("test//", "test");
    cache.write_to_cache("test////", "test/");
    return 0;
}
