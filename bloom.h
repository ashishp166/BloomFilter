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
};
/*
* initializes bloom filter
* 
*/
int bloom_init2(struct bloom * bloom, unsigned int entries, double errors);

}