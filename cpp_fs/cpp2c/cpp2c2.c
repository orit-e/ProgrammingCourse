/*******************************************************************************
 * 																			   *
 * 					File name:			cpp2c2.c					           *
 * 					Written by:			Orit								   *
 * 					Reviewed by:		Elia  								   *
 * 																			   *
 * ****************************************************************************/
#include <stdio.h>  /*  printf()            */
#include <stdlib.h> /*  malloc(), free()    */

/******************************************************************************/
/*                      typedefs, enums and structs:                          */
/* PublicTransport: */
typedef struct PublicTransport PublicTransport;
typedef void (*dtor_t)(PublicTransport *);
typedef void (*display_this_t)(PublicTransport *);

enum PUBLIC_TRANSPORT_VTBL_FUNC_PLACE
{
    PT__DTOR = 0,
    PT__DISPLAY_THIS
};

struct PublicTransport
{
    void **vptr;
    int m_license_plate;
};

/* Minibus: */
typedef struct Minibus Minibus;
typedef void (*wash_this_int)(Minibus *, int);

enum MINIBUS_VTBL_FUNC_PLACE
{
    MB__WASH_THIS_INT = 2
};

struct Minibus
{
    PublicTransport m_p;
    int m_numSeats;
};

/* Taxi: */
typedef struct Taxi Taxi;
struct Taxi
{
    PublicTransport m_p;
};

/* SpecialTaxi: */
typedef struct SpecialTaxi SpecialTaxi;
struct SpecialTaxi
{
    Taxi m_t;
};

/* PublicConvoy: */
typedef struct PublicConvoy PublicConvoy;
struct PublicConvoy
{
    PublicTransport m_p;
    PublicTransport *m_ptr1;
    PublicTransport *m_ptr2;
    Minibus m_m;
    Taxi m_t;
};

/******************************************************************************/
/*                          Function declarations:                            */
/* PublicTransport: */
void PublicTransport__ctor(PublicTransport *mem);
void PublicTransport__dtor(PublicTransport *this);
void PublicTransport__cctor(PublicTransport *this, 
                            const PublicTransport *other);
void PublicTransport__display_this(PublicTransport *this);
void PublicTransport__print_count_static(void);
static int PublicTransport__get_ID_this(PublicTransport *this);

/* Minibus: */
void Minibus__ctor(Minibus *mem);
void Minibus__dtor(Minibus *this);
void Minibus__cctor(Minibus *this, const Minibus *other);
void Minibus__display_this(Minibus *this);
void Minibus__wash_this_int(Minibus *this, int minutes);

/* Taxi: */
void Taxi__ctor(Taxi *mem);
void Taxi__dtor(Taxi *this);
void Taxi__cctor(Taxi *this, const Taxi *other);
void Taxi__display_this(Taxi *this);

/* SpecialTaxi: */
void SpecialTaxi__ctor(SpecialTaxi *mem);
void SpecialTaxi__dtor(SpecialTaxi *this);
void SpecialTaxi__cctor(SpecialTaxi *this, const SpecialTaxi *other);
void SpecialTaxi__display_this(SpecialTaxi *this);

/* PublicConvoy: */
void PublicConvoy__ctor(PublicConvoy *mem);
void PublicConvoy__dtor(PublicConvoy *this);
void PublicConvoy__cctor(PublicConvoy *this, const PublicConvoy *other);
void PublicConvoy__display_this(PublicConvoy *this);

/******************************************************************************/
/*                 PublicTransport static global variable:                    */
static int PublicTransport__s_count;

/******************************************************************************/
/*                              vtable structs:                               */
/* PublicTransport: */
void *g__PublicTransport__vtbl[] = 
{
    (void *)(size_t)&PublicTransport__dtor,
    (void *)(size_t)&PublicTransport__display_this
};

/* Minibus: */
void *g__Minibus__vtbl[] = 
{
    (void *)(size_t)&Minibus__dtor,
    (void *)(size_t)&Minibus__display_this,
    (void *)(size_t)&Minibus__wash_this_int
};

/* Taxi: */
void *g__Taxi__vtbl[] = 
{
    (void *)(size_t)&Taxi__dtor,
    (void *)(size_t)&Taxi__display_this
};

/* SpecialTaxi: */
void *g__SpecialTaxi__vtbl[] = 
{
    (void *)(size_t)&SpecialTaxi__dtor,
    (void *)(size_t)&SpecialTaxi__display_this
};

/* PublicConvoy: */
void *g__PublicConvoy__vtbl[] = 
{
    (void *)(size_t)&PublicConvoy__dtor,
    (void *)(size_t)&PublicConvoy__display_this
};

/******************************************************************************/
/*                           Function definition:                             */
/* PublicTransport: */
void PublicTransport__ctor(PublicTransport *mem)
{
    mem->m_license_plate = ++PublicTransport__s_count;
    *(void **)mem = &g__PublicTransport__vtbl;

    printf("PublicTransport::Ctor()%d\n", mem->m_license_plate);
}

void PublicTransport__dtor(PublicTransport *this)
{
    *(void **)this = &g__PublicTransport__vtbl;
    --PublicTransport__s_count;
    printf("PublicTransport::Dtor()%d\n", this->m_license_plate);
}

void PublicTransport__cctor(PublicTransport *this, const PublicTransport *other)
{
    this->m_license_plate = ++PublicTransport__s_count;
    *(void **)this = &g__PublicTransport__vtbl;

    printf("PublicTransport::CCtor() %d\n", this->m_license_plate);
    (void)other;
}

void PublicTransport__display_this(PublicTransport *this)
{
    printf("PublicTransport::display(): %d\n", this->m_license_plate);
}

void PublicTransport__print_count_static(void)
{
    printf("s_count: %d\n", PublicTransport__s_count);
}

static int PublicTransport__get_ID_this(PublicTransport *this)
{
    return this->m_license_plate;
}

/* Minibus: */
void Minibus__ctor(Minibus *mem)
{
    PublicTransport__ctor((PublicTransport *)mem);
    mem->m_numSeats = 20;
    *(void **)mem = &g__Minibus__vtbl;

    printf("Minibus::Ctor()\n");
}

void Minibus__dtor(Minibus *this)
{
    *(void **)this = &g__Minibus__vtbl;
    printf("Minibus::Dtor()\n");

    PublicTransport__dtor((PublicTransport *)this);
}

void Minibus__cctor(Minibus *this, const Minibus *other)
{
    PublicTransport__cctor((PublicTransport *)this, (PublicTransport *)other);
    this->m_numSeats = other->m_numSeats;
    *(void **)this = &g__Minibus__vtbl;

    printf("Minibus::CCtor()\n");
}

void Minibus__display_this(Minibus *this)
{
    printf("Minibus::display() ID:%d", 
            PublicTransport__get_ID_this((PublicTransport *)this));
    printf(" num seats:%d\n", this->m_numSeats);
}

void Minibus__wash_this_int(Minibus *this, int minutes)
{
    printf("Minibus::wash(%d) ID:%d\n", minutes, 
            PublicTransport__get_ID_this((PublicTransport *)this));
}

/* Taxi: */
void Taxi__ctor(Taxi *mem)
{
    PublicTransport__ctor((PublicTransport *)mem);
    *(void **)mem = &g__Taxi__vtbl;

    printf("Taxi::Ctor()\n");
}

void Taxi__dtor(Taxi *this)
{
    *(void **)this = &g__Taxi__vtbl;
    printf("Taxi::Dtor()\n");

    PublicTransport__dtor((PublicTransport *)this);
}

void Taxi__cctor(Taxi *this, const Taxi *other)
{
    PublicTransport__cctor((PublicTransport *)this, (PublicTransport *)other);
    *(void **)this = &g__Taxi__vtbl;

    printf("Taxi::CCtor()\n");
}

void Taxi__display_this(Taxi *this)
{
    printf("Taxi::display() ID:%d\n", 
            PublicTransport__get_ID_this((PublicTransport *)this));
}

/* SpecialTaxi: */
void SpecialTaxi__ctor(SpecialTaxi *mem)
{
    Taxi__ctor((Taxi *)mem);
    *(void **)mem = &g__SpecialTaxi__vtbl;

    printf("SpecialTaxi::Ctor()\n");
}

void SpecialTaxi__dtor(SpecialTaxi *this)
{
    *(void **)this = &g__SpecialTaxi__vtbl;
    printf("SpecialTaxi::Dtor()\n");

    Taxi__dtor((Taxi *)this);
}

void SpecialTaxi__cctor(SpecialTaxi *this, const SpecialTaxi *other)
{
    Taxi__cctor((Taxi *)this, (Taxi *)other);
    *(void **)this = &g__SpecialTaxi__vtbl;

    printf("SpecialTaxi::CCtor()\n");
}

void SpecialTaxi__display_this(SpecialTaxi *this)
{
    printf("SpecialTaxi::display() ID: %d", 
            PublicTransport__get_ID_this((PublicTransport *)this));
}

/* PublicConvoy */
void PublicConvoy__ctor(PublicConvoy *mem)
{
    PublicTransport__ctor((PublicTransport *)mem);
    
    mem->m_ptr1 = (PublicTransport *)malloc(sizeof(Minibus));
    Minibus__ctor((Minibus *)mem->m_ptr1);
    mem->m_ptr2 = (PublicTransport *)malloc(sizeof(Taxi));
    Taxi__ctor((Taxi *)mem->m_ptr2);
    
    Minibus__ctor(&mem->m_m);
    Taxi__ctor(&mem->m_t);
    
    *(void **)mem = &g__PublicConvoy__vtbl;
}

void PublicConvoy__dtor(PublicConvoy *this)
{
    *(void **)this = &g__PublicConvoy__vtbl;
    
    ((dtor_t)(size_t)((this->m_ptr1->vptr)[PT__DTOR]))(
                                            (PublicTransport *)this->m_ptr1);
    free(this->m_ptr1);
    ((dtor_t)(size_t)((this->m_ptr2->vptr)[PT__DTOR]))(
                                            (PublicTransport *)this->m_ptr2);
    free(this->m_ptr2);

    Taxi__dtor((Taxi *)&this->m_t);
    Minibus__dtor((Minibus *)&this->m_m);
    PublicTransport__dtor((PublicTransport *)&this->m_p);
}

void PublicConvoy__cctor(PublicConvoy *this, const PublicConvoy *other)
{
    PublicTransport__ctor((PublicTransport *)this);

    this->m_ptr1 = (PublicTransport *)malloc(sizeof(Minibus));
    Minibus__cctor((Minibus *)this->m_ptr1, (Minibus *)other->m_ptr1);
    this->m_ptr2 = (PublicTransport *)malloc(sizeof(Taxi));
    Taxi__cctor((Taxi *)this->m_ptr2, (Taxi *)other->m_ptr2);

    Minibus__cctor((Minibus *)&this->m_m, (Minibus *)&other->m_m);
    Taxi__cctor((Taxi *)&this->m_t, (Taxi *)&other->m_t);

    *(void **)this = &g__PublicConvoy__vtbl;
}

void PublicConvoy__display_this(PublicConvoy *this)
{
    ((display_this_t)(size_t)((this->m_ptr1->vptr)[PT__DISPLAY_THIS]))(
                                            (PublicTransport *)this->m_ptr1);
    ((display_this_t)(size_t)((this->m_ptr2->vptr)[PT__DISPLAY_THIS]))(
                                            (PublicTransport *)this->m_ptr2);

    Minibus__display_this(&this->m_m);
    Taxi__display_this(&this->m_t);
}

/* template: */
int max_func_int(int t1, int t2)
{
    return ((t1 > t2) ? t1 : t2);
}

/* other functions: */
void print_info__PublicTransport(PublicTransport *a)
{   
    ((display_this_t)(size_t)((a->vptr)[PT__DISPLAY_THIS]))(a);
}

void print_info__void()
{
    PublicTransport__print_count_static();
}

void print_info__Minibus(Minibus *m)
{
    ((wash_this_int)((size_t)(((PublicTransport *)m)
                                            ->vptr)[MB__WASH_THIS_INT]))(m, 3);
}

void print_info__PublicTransport_int(PublicTransport *pt, int i)
{
    Minibus ret;

    Minibus__ctor(&ret);
    printf("print_info(int i)\n");

    Minibus__display_this(&ret);

    PublicTransport__cctor(pt, (PublicTransport *)&ret);
    Minibus__dtor(&ret);

    (void)i;
}

void taxi_display__Taxi(Taxi *s)
{
    ((display_this_t)(size_t)((((PublicTransport *)s)
                            ->vptr)[PT__DISPLAY_THIS]))((PublicTransport *)s);
}

int main(int argc, char **argv, char **envp)
{
    Minibus m;
    Minibus tmp2;
    Minibus m2;
    Minibus arr3[4];
    SpecialTaxi st;
    Taxi tmp3;
    Taxi *arr4;
    PublicTransport tmp;
    PublicTransport *array[3];
    PublicTransport arr2[3];
    int *tmp4 = NULL;
    int tmp5 = 0;
    int i = 0;
    PublicConvoy *ts1 = NULL;
    PublicConvoy *ts2 = NULL;

    Minibus__ctor(&m);
    print_info__Minibus(&m);

    print_info__PublicTransport_int(&tmp, 3);
    PublicTransport__display_this(&tmp);
    PublicTransport__dtor(&tmp);

    array[0] = (PublicTransport *)malloc(sizeof(Minibus));
    Minibus__ctor((Minibus *)array[0]);
    array[1] = (PublicTransport *)malloc(sizeof(Taxi));
    Taxi__ctor((Taxi *)array[1]);
    array[2] = (PublicTransport *)malloc(sizeof(Minibus));
    Minibus__ctor((Minibus *)array[2]);

    for(i = 0; i < 3; ++i)
    {
        ((display_this_t)(size_t)((array[i]->vptr)[PT__DISPLAY_THIS]))(
                                                                    array[i]);
    }

    for(i = 0; i < 3; ++i)
    {
        ((dtor_t)(size_t)((array[i]->vptr)[PT__DTOR]))(array[i]);
        free(array[i]);
    }

    Minibus__ctor(&tmp2);
    PublicTransport__cctor(&arr2[0], (PublicTransport *)&tmp2);
    Minibus__dtor(&tmp2);
    Taxi__ctor(&tmp3);
    PublicTransport__cctor(&arr2[1], (PublicTransport *)&tmp3);
    Taxi__dtor(&tmp3);
    PublicTransport__ctor(&arr2[2]);

    for(i = 0; i < 3; ++i)
    {
        ((display_this_t)(size_t)((arr2[i].vptr)[PT__DISPLAY_THIS]))(&arr2[i]);
    }

    print_info__PublicTransport(&arr2[0]);

    PublicTransport__print_count_static();

    Minibus__ctor(&m2);
    PublicTransport__print_count_static();

    for(i = 0; i < 4; ++i)
    {
        Minibus__ctor(&arr3[i]);
    }

    tmp4 = (int *)malloc(sizeof(Taxi) * 4 + sizeof(size_t));
    ++tmp4;
    *tmp4 = 4;
    arr4 = (Taxi *)(tmp4 + 1);
    for(i = 0; i < 4; ++i)
    {
        Taxi__ctor(&arr4[i]);
    }
    tmp5 = *((int *)arr4 - 1) - 1;
    for(i = tmp5; i >= 0; --i)
    {
        ((dtor_t)(size_t)((((PublicTransport *)&arr4[i])->vptr)[PT__DTOR]))(
                                                (PublicTransport *)&arr4[i]);
    }
    free(((size_t *)arr4) - 1);

    printf("%d\n", max_func_int(1,2));
    printf("%d\n", max_func_int(1,2.0f));

    SpecialTaxi__ctor(&st);
    Taxi__cctor(&tmp3, (Taxi *)&st);
    taxi_display__Taxi(&tmp3);
    Taxi__dtor(&tmp3);

    ts1 = (PublicConvoy *)malloc(sizeof(PublicConvoy));
    PublicConvoy__ctor(ts1);
    ts2 = (PublicConvoy *)malloc(sizeof(PublicConvoy));
    PublicConvoy__cctor(ts2, ts1);
    ((display_this_t)(size_t)((((PublicTransport *)ts1)
                            ->vptr)[PT__DISPLAY_THIS]))((PublicTransport *)ts1);
    ((display_this_t)(size_t)((((PublicTransport *)ts2)
                            ->vptr)[PT__DISPLAY_THIS]))((PublicTransport *)ts2);

    ((dtor_t)(size_t)((((PublicTransport *)ts1)
                                    ->vptr)[PT__DTOR]))((PublicTransport *)ts1);

    ((display_this_t)(size_t)((((PublicTransport *)ts2)
                            ->vptr)[PT__DISPLAY_THIS]))((PublicTransport *)ts2);

    ((dtor_t)(size_t)((((PublicTransport *)ts2)
                            ->vptr)[PT__DTOR]))((PublicTransport *)ts2);

    SpecialTaxi__dtor(&st);
    for(i = 3; i >= 0; --i)
    {
        Minibus__dtor(&arr3[i]);
    }
    Minibus__dtor(&m2);
    for(i = 2; i >= 0; --i)
    {
        ((dtor_t)(size_t)((arr2[i].vptr)[PT__DTOR]))(&arr2[i]);
    }
    Minibus__dtor(&m);

    (void)argc;
    (void)argv;
    (void)envp;

    return 0;
}