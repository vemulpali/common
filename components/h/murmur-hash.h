#ifndef _MURMUR_HASH_H_
#define _MURMUR_HASH_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
	returns hash index for the given key	
*/
extern unsigned int murmur3_32( const char *key,
			    unsigned int len,
			    unsigned int seed );


#ifdef __cplusplus
};
#endif


#endif
