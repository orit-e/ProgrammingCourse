#ifndef __SPELL_CHECKER__H__
#define __SPELL_CHECKER__H__

typedef enum spell_checker_errors
{
    HASH_ERROR = -4,
    MMAP_ERROR = -3,
    OPEN_ERROR = -2,
    FSTAT_ERROR = -1,
    SPELL_CHECKER_SUCCESS = 0
}spell_checker_errors_t;

spell_checker_errors_t SpellChecker(void);

#endif /* __SPELL_CHECKER__H__ */