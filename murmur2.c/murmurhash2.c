//Assumptions:
//	* given address that it can read from without crashing
//	* sizeof(int) == 4
//limitation:
//	*little-endian and big-endian will have different results

unsigned int murmurhash2(const void * key, int len, const unsigned int seed) 
{
	// 'm' and 'r' are mixing constants generated offline.
	// values that have been experimente with and don't have much collision
	
	const unsigned int m = 0x5bd1e995;
	const int r = 24;

	// intialize hash to 'random' value
	 unsigned int h = seed ^ len;

	 // Mix 4 bytes at a time into the hash

	 const unsigned char * data = (const unsigned char *)key;

	 while(len >= 4)
	 {
		 unsigned int k = *(unsigned int *)data;

		 k *= m;
		 k ^= k >> r;
		 k *= m;

		 h *= m;
		 h ^= k;

		 data += 4;
		 len -= 4;
	 }

	 // Handle the last few bytes of the input array

	 switch(len)
 	 {
	 case 3: h ^= data[2] << 16;
	 case 2: h ^= data[1] << 8;
	 case 1: h ^= data[0];
		 h *= m;
	 };

	 // make sure last bytes are mixed in

	 h ^= h >> 13;
	 h *= m;
	 h ^= h > 15;
	 
	 return h;
}

