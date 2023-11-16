#include <stdio.h>  /*  printf()            */
#include <stdlib.h> /*  malloc(), free()    */

/******************************************************************************/
/*                          typedefs and structs:                             */
/* PublicTransport: */
typedef struct PublicTransport PublicTransport;
typedef void (*PublicTransport_dtor_t)(PublicTransport *);
typedef void (*PublicTransport_display_this_t)(PublicTransport *);
typedef struct PublicTransport_vtbl
{
    PublicTransport_dtor_t dtor;
    PublicTransport_display_this_t display_this;
}PublicTransport_vtbl;
struct PublicTransport
{
    PublicTransport_vtbl *vptr;
    int m_license_plate;
};

/* Minibus: */
typedef struct Minibus Minibus;
typedef struct Minibus_vtbl
{
    PublicTransport_vtbl PublicTransport_vptr;
    void (*wash_this_int)(Minibus *, int);
}Minibus_vtbl;
struct Minibus
{
    PublicTransport m_p;
    int m_numSeats;
};

/* Taxi: */
typedef struct Taxi Taxi;
typedef struct Taxi_vtbl
{
    PublicTransport_vtbl PublicTransport_vptr;
}Taxi_vtbl;
struct Taxi
{
    PublicTransport m_p;
};

/* SpecialTaxi: */
typedef struct SpecialTaxi SpecialTaxi;
typedef struct SpecialTaxi_vtbl
{
    Taxi_vtbl Taxi_vptr;
}SpecialTaxi_vtbl;
struct SpecialTaxi
{
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




/******************************************************************************/
/*                 PublicTransport static global variable:                    */

static int PublicTransport__s_count;

/******************************************************************************/
/*                              vtable structs:                               */
/* PublicTransport: */
PublicTransport_vtbl g__PublicTransport__vtbl = 
{
    &PublicTransport__dtor,
    &PublicTransport__display_this
};

/* Minibus: */
Minibus_vtbl g__Minibus__vtbl = 
{
    {
        (PublicTransport_dtor_t)(size_t)&Minibus__dtor,
        (PublicTransport_display_this_t)(size_t)&Minibus__display_this
    },
    &Minibus__wash_this_int
};

/* Taxi: */
Taxi_vtbl g__Taxi__vtbl = 
{
    {
        (PublicTransport_dtor_t)(size_t)&Taxi__dtor,
        (PublicTransport_display_this_t)(size_t)&Taxi__display_this
    }
};

/* SpecialTaxi: */
SpecialTaxi_vtbl g__SpecialTaxi__vtbl = 
{
    {
        {
            (PublicTransport_dtor_t)(size_t)&SpecialTaxi__dtor,
            (PublicTransport_display_this_t)(size_t)&SpecialTaxi__display_this
        }
    }
};

/******************************************************************************/
/*                           Function definition:                             */
/* PublicTransport: */
void PublicTransport__ctor(PublicTransport *mem)
{
    mem->m_license_plate = ++PublicTransport__s_count;
    *(PublicTransport_vtbl **)mem = &g__PublicTransport__vtbl;

    printf("PublicTransport::Ctor()%d\n", mem->m_license_plate);
}

void PublicTransport__dtor(PublicTransport *this)
{
    *(PublicTransport_vtbl **)this = &g__PublicTransport__vtbl;
    --PublicTransport__s_count;
    printf("PublicTransport::Dtor()%d\n", this->m_license_plate);
}

void PublicTransport__cctor(PublicTransport *this, const PublicTransport *other)
{
    this->m_license_plate = ++PublicTransport__s_count;
    *(PublicTransport_vtbl **)this = &g__PublicTransport__vtbl;

    printf("PublicTransport::CCtor() %d\n", this->m_license_plate);
    (void)other;
}

void PublicTransport__display_this(PublicTransport *this)
{
    printf("PublicTransport::display(): %d\n", this->m_license_plate);
}

void PublicTransport__print_count_static(void)
{
    printf("PublicTransport__s_count: %d\n", PublicTransport__s_count);
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
    *(Minibus_vtbl **)mem = &g__Minibus__vtbl;

    printf("Minibus::Ctor()\n");
}

void Minibus__dtor(Minibus *this)
{
    *(Minibus_vtbl **)this = &g__Minibus__vtbl;
    printf("Minibus::Dtor()\n");

    PublicTransport__dtor((PublicTransport *)this);
}

void Minibus__cctor(Minibus *this, const Minibus *other)
{
    PublicTransport__cctor((PublicTransport *)this, (PublicTransport *)other);
    this->m_numSeats = other->m_numSeats;
    *(Minibus_vtbl **)this = &g__Minibus__vtbl;

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
    *(Taxi_vtbl **)mem = &g__Taxi__vtbl;

    printf("Taxi::Ctor()\n");
}

void Taxi__dtor(Taxi *this)
{
    *(Taxi_vtbl **)this = &g__Taxi__vtbl;
    printf("Taxi::Dtor()\n");

    PublicTransport__dtor((PublicTransport *)this);
}

void Taxi__cctor(Taxi *this, const Taxi *other)
{
    PublicTransport__cctor((PublicTransport *)this, (PublicTransport *)other);
    *(Taxi_vtbl **)this = &g__Taxi__vtbl;

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
    *(SpecialTaxi_vtbl **)mem = &g__SpecialTaxi__vtbl;

    printf("SpecialTaxi::Ctor()\n");
}

void SpecialTaxi__dtor(SpecialTaxi *this)
{
    *(SpecialTaxi_vtbl **)this = &g__SpecialTaxi__vtbl;
    printf("SpecialTaxi::Dtor()\n");

    Taxi__dtor((Taxi *)this);
}

void SpecialTaxi__cctor(SpecialTaxi *this, const SpecialTaxi *other)
{
    Taxi__cctor((Taxi *)this, (Taxi *)other);
    *(SpecialTaxi_vtbl **)this = &g__SpecialTaxi__vtbl;

    printf("SpecialTaxi::CCtor()\n");
}

void SpecialTaxi__display_this(SpecialTaxi *this)
{
    printf("SpecialTaxi::display() ID: %d", 
            PublicTransport__get_ID_this((PublicTransport *)this));
}

/* template: */
int max_func_int(int t1, int t2)
{
    return ((t1 > t2) ? t1 : t2);
}

/* other functions: */
void print_info__PublicTransport(PublicTransport *a)
{   
    (*(PublicTransport_vtbl **)a)->display_this(a);
}

void print_info__void()
{
    PublicTransport__print_count_static();
}

void print_info__Minibus(Minibus *m)
{
    (*(Minibus_vtbl **)m)->wash_this_int(m, 3);
}

void print_info__PublicTransport_int(PublicTransport *pt, int i)
{
    Minibus ret;

    Minibus__ctor(&ret);
    printf("print_info(int i)\n");

    Minibus__display_this(&ret);

    PublicTransport__cctor(pt, (PublicTransport *)&ret);
    (*(PublicTransport_vtbl **)&ret)->dtor((PublicTransport *)&ret);
    (void)i;
}

void taxi_display__Taxi(Taxi *s)
{
    (*(PublicTransport_vtbl **)s)->display_this((PublicTransport *)s);
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
        (*(PublicTransport_vtbl **)array[i])->display_this(array[i]);
    }

    for(i = 0; i < 3; ++i)
    {
        (*(PublicTransport_vtbl **)array[i])->dtor(array[i]);
        free(array[i]);
    }

    Minibus__ctor(&tmp2);
    PublicTransport__cctor(&arr2[0], (PublicTransport *)&tmp2);
    (*(PublicTransport_vtbl **)&tmp2)->dtor((PublicTransport *)&tmp2);
    Taxi__ctor(&tmp3);
    PublicTransport__cctor(&arr2[1], (PublicTransport *)&tmp3);
    (*(PublicTransport_vtbl **)&tmp3)->dtor((PublicTransport *)&tmp3);
    PublicTransport__ctor(&arr2[2]);

    for(i = 0; i < 3; ++i)
    {
        (*(PublicTransport_vtbl **)&arr2[i])->display_this(&arr2[i]);
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
        (*(PublicTransport_vtbl **)&arr4[i])->dtor((PublicTransport *)&arr4[i]);
    }
    free(((size_t *)arr4) - 1);

    printf("%d\n", max_func_int(1,2));
    printf("%d\n", max_func_int(1,2.0f));

    SpecialTaxi__ctor(&st);
    Taxi__cctor(&tmp3, (Taxi *)&st);
    taxi_display__Taxi(&tmp3);
    Taxi__dtor(&tmp3);

    SpecialTaxi__dtor(&st);
    for(i = 3; i >= 0; --i)
    {
        Minibus__dtor(&arr3[i]);
    }
    Minibus__dtor(&m2);
    for(i = 2; i >= 0; --i)
    {
        (*(PublicTransport_vtbl **)&arr2[i])->dtor(&arr2[i]);
    }
    PublicTransport__dtor((PublicTransport *)&m);

    (void)argc;
    (void)argv;
    (void)envp;

    return 0;
}