#ifndef _BLOOM_H //what does ifndef mean
#define _BLOOM_H

#ifdef __cplusplus
extern "C" {
#endif


#define NULL_BLOOM_FILTER { 0, 0, 0, 0, 0.0, 0, 0, 0, 0.0, NULL }

#define ENTRIES_T unsigned int
#define BYTES_T unsigned long int
#define BITS_T unsigned long int

/*
* First call bloom_init()
*/
struct bloom
{
    unsigned int entries;
    unsigned long int bits;
    unsigned long int bytes;
    unsigned char hashes;
    double error;

    unsigned char ready;
    double bpe;
    unsigned char * bf;
};
/*
* initializes bloom filter
* optimal number of bits = (entries * ln(error)) / ln(2)^2
* optimal number of hash function k = bpe * ln(2) What is bpe
* inputs:
*   bloom - pointer to an allocated struct bloom 
*   entries - expected number of entries which will be added
*   error - probability of collision
* 
* return value: 0 sucesss, 1 error
*/
int bloom_init(struct bloom * bloom, unsigned int entries, double errors);

/*
Check if element ecists in bloom filter(chance of false positive)
inputs: 
    buffer - pointer to buffer containing element to check
    len - size of 'buffer'
return value: 0 if element not present, 1 if present, -1 if bloom not initialized(bloom_init)
*/
int bloom_check(struct bloom * bloom, const void * buffer, int len);

/*
Adds element to the filter(bloom_check and then if not present, adds it)
inputs:
    buffer - pointer to buffer containing element to check
    len - size of 'buffer'
return value: 0 if element is not present and added, 1 if element already added, -1 if bloom not initialized
*/
int bloom_add(struct bloom * bloom, const void * buffer, int len);

/*
Prints bloom filter
*/
void bloom_print(struct bloom * bloom);

/*
Deallocates internal storage
Bloom struct no longer usable and must call bloom_init to reinitalize it
*/
void bloom_free(struct bloom * bloom);

/**
 * Erase internal storage, but keeps intialized state(no need to call bloom_init)
 * return value: 0 if sucess, 1 if failure
*/
int bloom_reset(struct bloom * bloom);

#ifdef __cplusplus
}
#endif

#endif
