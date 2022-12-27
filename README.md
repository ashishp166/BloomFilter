# BloomFilter
Problem: Data structure that is able to say whether a given element is located in the database. We would like the data structure to be time and space efficent

Issues: An array with all the elements won't work as this would have a linear time and space complexity. Likewise, a hashtable won't work because it would have a larger than linear space complexity and due to hashing collisions the time complexity won't be constant. 

Solution: A bloom filter uses multiple hash functions that call the element as input and output an index in which would be set to 1 in the bit array. Therefor, when querying for an element, it is unlikely that the hash of an element not in the database would return true. This is based on probability and the number of hashes we have. The more number of filters the better the bloom filter confidence level of it being in the database, but this means that the time complexity would increase and the space complexity would have to also increase. Another thing is that there is no false negatives in bloom filters. There are only false positives from bloom filters and these come from when the query says an element exists when it actually isn't in the database.

Applications of Bloom Filters: caching, packet routing and forwarding, p2p networks

Literary Reviews:
    1. https://dl.acm.org/doi/10.5555/1400123.1400125
        -Given 2 hash functions h1 and h2, you can create k hash funcctions wihtout increasin gthe asymptotic false positive probability. gi(x) = hh1(x) + ih2(x) + i^2 mod m where m is the size of the hash table and i goes from 0 to k - 1. While having k pairwise indepenedent hash functions result in there being a constant increase in the space to keep the same bound of false postive probabiliy, but with using the gi's there is no increase for the bound of false positive probability. Therefor it is viewed as a replacement for fully random hash functions.
    2. https://arxiv.org/abs/1212.3964
        -De-duplication is a streaming scenario is the process of getting rid of duplicates in unbounded data streams. Reservoir Sampling based Bloom Filter(RSBF) is based on applying reservoir sampling and bloom filters. Secondly, Biased Sampling based Bloom Filters(BSBF) is based on biased smapling concepts. 
    3. https://www.cs.cmu.edu/~dga/papers/cuckoo-conext2014.pdf
        -Bloom filters don't support deletion because if you choose to remove an element without rebuilding the whole filter, then this means that there would be false negatives in the data structure as you might have an instance of an element, but it would output that the element does not ecist.