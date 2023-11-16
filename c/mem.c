/***********************
 * Submitted by: Orit. *
 * Reviewed by: Tomer. *
 ***********************/ 
 
#include <stdio.h>/* size_t, sizeof */
#include <assert.h>/* assert */
#include "mem.h"

/******************************************************************************/
/* Macro functions for Memset: */
/* This macro function copy ch to variable of word size (size_t). */
#define BYTE 8
#if 8 == size_t
	#define WORDSIZE_OF_C(c) (size_t)(((c) << 56) | ((c) << 48) | ((c) << 40) |\
									((c) << 32) | ((c) << 24) | ((c) << 16) |  \
									((c) << 8) | c)
#else
	#define WORDSIZE_OF_C(c) (size_t)(((c) << 24) | ((c) << 16) | 			   \
									((c) << 8) | c)
#endif

/* Misalignment handeling. */
#define FILL_MISALIGNMENT_MEMSET(ptr_s, ch, misalignment, n)	\
{																\
	while(0 < misalignment && 0 < n)							\
	{ 															\
		*ptr_s++ = ch;											\
		--misalignment;											\
		--n;													\
	}															\
}

/* Filling memory pointed to by ptr_s with byte or word size filling n times. */
#define FILL_MEMSET(ptr_s, filling, n)	\
{										\
	while(0 < n) 						\
	{ 									\
		*ptr_s++ = filling;				\
		--n;							\
	}									\
}

/******************************************************************************/
/* Macro functions for Memcpy: */
/* Misalignment handeling. */
#define FILL_MISALIGNMENT_MEMCPY(ptr_dest, ptr_src, misalignment, n)	\
{																		\
	while(0 < misalignment && 0 < n)									\
	{ 																	\
		*ptr_dest++ = *ptr_src++;										\
		--misalignment;													\
		--n;															\
	}																	\
}

/* Filling memory pointed to by ptr_s with byte or word size filling n times. */
#define COPY_MEMCPY(ptr_dest, ptr_src, n)	\
{											\
	while(0 < n) 							\
	{ 										\
		*ptr_dest++ = *ptr_src++;			\
		--n;								\
	}										\
}

/******************************************************************************/
/* Macro functions for BackwardMemcpy: */
/* Misalignment handeling. */
#define FILL_MISALIGNMENT_BACK_MEMCPY(ptr_dest, ptr_src, misalignment, n)	\
{																			\
	while(0 < misalignment && 0 < n)										\
	{ 																		\
		*--ptr_dest = *--ptr_src;											\
		--misalignment;														\
		--n;																\
	}																		\
}

/* Filling memory pointed to by ptr_s with byte or word size filling n times. */
#define COPY_BACK_MEMCPY(ptr_dest, ptr_src, n)	\
{												\
	while(0 < n) 								\
	{ 											\
		*--ptr_dest = *--ptr_src;				\
		--n;									\
	}											\
}

const size_t word_size = sizeof(size_t);

void *Memset(void *s, int c, size_t n)
{
	unsigned char *ptr_s = s;
	size_t misalignment = (size_t)ptr_s & (word_size - 1);
	size_t wordsize_c = WORDSIZE_OF_C(c);
	size_t leftover = 0;
	
	assert(NULL != s);

	FILL_MISALIGNMENT_MEMSET(ptr_s, c, misalignment, n);
	leftover = n & (word_size - 1);
	n /= word_size;
	{
		size_t *ptr_s_t = (size_t *)ptr_s;
		
		FILL_MEMSET(ptr_s_t, wordsize_c, n);
		ptr_s = (unsigned char *)ptr_s_t;
		FILL_MEMSET(ptr_s, c, leftover);
	}
	return s;
}

void *Memcpy(void *dest, const void *src, size_t n)
{
	const unsigned char *ptr_src = src;	
	unsigned char *ptr_dest = dest;
	size_t misalignment = (size_t)dest & (word_size - 1);
	size_t leftover = 0;
	
	assert(NULL != dest);
	assert(NULL != src);
	
	FILL_MISALIGNMENT_MEMCPY(ptr_dest, ptr_src, misalignment, n);
	leftover = n & (word_size - 1);
	n /= word_size;
	{
		const size_t *ptr_src_t = (const size_t *)ptr_src;
		size_t *ptr_dest_t = (size_t *)ptr_dest;
		
		COPY_MEMCPY(ptr_dest_t, ptr_src_t, n);
		ptr_src = (const unsigned char *)ptr_src_t;
		ptr_dest = (unsigned char *)ptr_dest_t;
		COPY_MEMCPY(ptr_dest, ptr_src, leftover);
	}
	return dest;
}

/* Same as Memcpy() function but backwards. */
static void *BackwardMemcpy(void *dest, const void *src, size_t n)
{
	const unsigned char *ptr_src = (unsigned char *)src + n;
	unsigned char *ptr_dest = (unsigned char *)dest + n;
	size_t misalignment = (size_t)ptr_src & (word_size - 1);
	size_t leftover = 0;
	
	assert(NULL != dest);
	assert(NULL != src);
	
	FILL_MISALIGNMENT_BACK_MEMCPY(ptr_dest, ptr_src, misalignment, n);
	leftover = n & (word_size - 1);
	n /= word_size;
	{
		const size_t *ptr_src_t = (const size_t *)ptr_src;
		size_t *ptr_dest_t = (size_t *)ptr_dest;
		
		COPY_BACK_MEMCPY(ptr_dest_t, ptr_src_t, n);
		ptr_src = (const unsigned char *)ptr_src_t;
		ptr_dest = (unsigned char *)ptr_dest_t;
		COPY_BACK_MEMCPY(ptr_dest, ptr_src, leftover);
	}
	return dest;
}

void *Memmove(void *dest, const void *src, size_t n)
{
	assert(NULL != dest);
	assert(NULL != src);
	
	if((size_t)dest > (size_t)src)
	{
		BackwardMemcpy(dest, src, n);
	}
	else
	{
		Memcpy(dest, src, n);
	}
	
	return dest;
}
