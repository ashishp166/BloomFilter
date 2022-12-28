#include <assert.h>
#include <fcntl.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "bloom.h"
#include "murmurhash2.h"

//#define MAKESTRING(n) STRING(n)
//#define STRING(n) #n

inline static int test_bit_set_bit(unsigned char * buf,
                                    unsigned long int bit, int set_bit)
{
    unsigned long int byte = bit >> 3;
    unsigned char c = buf[byte];
    unsigned char mask = 1 << (bit%8ul);

    if (c & mask) {
        return 1;
    } else {
        if (set_bit) {
            buf[byte] = c | mask;
        }
        return 0;
    }
}

static int bloom_check_add(struct bloom * bloom,
                            const void * buffer, int len, int add)
{
    if (bloom->ready == 0) {
        printf("bloom at %p not initialized\n", (void *) bloom);
        return -1;
    }

    unsigned char hits = 0;
    unsigned int a = murmurhash2(buffer, len, 0x9747b28c); // 0x9747b28c
    unsigned int b = murmurhash2(buffer, len, a); // why is this one a
    unsigned long int x;
    unsigned long int i;

    for (i = 0; i < bloom->hashes; i++) {
        x = (a + b*i) % bloom->bits; // utilizes idea only need for 2 hash functions and use i as a way to be able to create a total of bloom -> hash different combindations
        if (test_bit_set_bit(bloom->bf, x, add)) {
            hits++;
        } else if (!add) { // add = 1 if add so then skip this if statement
            return 0; // add = 0 if check so then return 0 since doesn't exist
        }
    }

    if (hits == bloom->hashes) {
        return 1; // existence
    }

    return 0;
}

int bloom_init(struct bloom * bloom, unsigned int entries, double error) 
{
    if (sizeof(unsigned long int) < 8) {
        printf("error: libbloom will not function correctly because\n");
        printf("sizeof(unsigned long int) == %ld\n", sizeof(unsigned long int));
        exit(1);
    }

    memset(bloom, 0, sizeof(struct bloom));

    if (entries < 1000 || error <= 0 || error >= 1) { // don't want too small size and error in bound [0, 1]
        return 1;
    }

    bloom -> entries = entries;
    bloom->error = error;

    double num = -log(bloom->error);
    double denom = 0.480453013918; // ln(2) ^2
    bloom->bpe = num/denom;
    
    long double dentries = (long double)entries;
    long double allbits = dentries * bloom->bpe;
    bloom->bits = (unsigned long int)allbits;

    if (bloom->bits % 8) {
        bloom->bytes = (bloom->bits / 8) + 1; // want the ceiling of the bytes
    } else {
        bloom->bytes = bloom->bits / 8; // 8 bits in one byte
    }

    bloom->hashes = (unsigned char)ceil(0.6931471805599 * bloom->bpe); // ln(2)
    
    bloom->bf = (unsigned char *)calloc(bloom->bytes, sizeof(unsigned char));
    if (bloom->bf == NULL) {
        return 1;
    }

    bloom->ready = 1; // bloom filter initialized

    return 0;
}

int bloom_check(struct bloom * bloom, const void * buffer, int len)
{
    return bloom_check_add(bloom, buffer, len, 0);
}

int bloom_add(struct bloom * bloom, const void * buffer, int len)
{
    return bloom_check_add(bloom, buffer, len, 1);
}

void bloom_print(struct bloom * bloom) 
{
    printf("bloom at %p\n", (void *)bloom);
    if(!bloom->ready) { 
        printf(" ** NOT READY ***\n");
    }
    printf(" ->entries = %u\n", bloom->entries);
    printf(" ->error = %f\n", bloom->error);
    printf(" ->bits = %lu\n", bloom->bits);
    printf(" ->bits per elem = %f\n", bloom->bpe);
    printf(" ->bytes = %lu", bloom->bytes);
    unsigned int KB = bloom->bytes / 1024; // math.pow(2, 10) = 1024
    unsigned int MB = KB / 1024;
    printf(" (%u KB, %u MB)\n", KB, MB);
    printf(" ->hash functions = %d\n", bloom->hashes);
}

void bloom_free(struct bloom * bloom)
{
    if (bloom->ready) {
        free(bloom->bf);
    }
    bloom->ready = 0;
}

int bloom_reset(struct bloom * bloom) 
{
    if (!bloom->ready) {
        return 1;
    }
    memset(bloom->bf, 0, bloom->bytes);
    return 0;
}
