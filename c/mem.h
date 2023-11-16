#ifndef __MEM_H__
#define __MEM_H__

/* DESCRIPTION:
 * The Memset() function fills the first n bytes of the memory area pointed to 
 * by s with the constant byte c.
 *
 * Return value:
 * The Memset() function returns a pointer to the memory area s.
 */
void *Memset(void *s, int c, size_t n);

/* DESCRIPTION:
 * The Memcpy() function copies n bytes from memory area src to memory area 
 * dest. The memory areas must not overlap.  Use Memmove() if the memory areas 
 * do overlap.
 *
 * Return value:
 * The Memcpy() function returns a pointer to dest.
 */
void *Memcpy(void *dest, const void *src, size_t n);

/* DESCRIPTION:
 * The Memmove() function copies n bytes from memory area src to memory area 
 * dest. The memory areas may overlap.
 *
 * Return value:
 * The Memmove() function returns a pointer to dest.
 */
void *Memmove(void *dest, const void *src, size_t n);
 
#endif /* __MEM_H__ */
