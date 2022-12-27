#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "bloom.h"

#ifdef __linux
#include <sys/time.h>
#include <time.h>
#endif

/**
 * test to verify filter works
*/

int main(int argc, char **argv)
{
    struct bloom bloom;

    printf("----- Basic tests with static library -----\n");
    assert(bloom_init(&bloom, 0, 1.0) == 1);
    assert(bloom_init(&bloom, 10, 0) == 1);
    assert(bloom.ready == 0);
    assert(bloom_add(&bloom, "hello world", 11) == -1);
    assert(bloom_check(&bloom, "hello world", 11) == -1);
    bloom_free(&bloom);

    assert(bloom_init(&bloom, 1002, 0.1) == 0);
    assert(bloom.ready == 1);
    bloom_print(&bloom);

    assert(bloom_check(&bloom, "hello world", 11) == 0);
    assert(bloom_add(&bloom, "hello world", 11) == 0);
    assert(bloom_check(&bloom, "hello world", 11) == 1);
    assert(bloom_add(&bloom, "hello world", 11) > 0);
    assert(bloom_add(&bloom, "hello", 5) == 0);
    assert(bloom_add(&bloom, "hello", 5) > 0);
    assert(bloom_check(&bloom, "hello", 5) == 1);
    bloom_free(&bloom);
    printf("----- DONE Basic tests with static library -----\n");
    return 0;
}