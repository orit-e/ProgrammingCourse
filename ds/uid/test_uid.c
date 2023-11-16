#include <stdio.h> /* printf */
#include <stdlib.h>
#include <string.h> /* strcmp */
#include "uid.h"

#define GREEN_COLOR 	"\x1b[0;32m"
#define RED_COLOR	 	"\x1b[0;31m"
#define DEFAULT_COLOR	"\x1b[0;0m"
#define BLUE_COLOR		"\x1b[0;34m"

enum EXIT_STATUS {FAILED, SUCCESS, DONE};

#define RUN_TEST(result, name) {\
			printf("test %s: ", name);\
			switch (result)\
			{\
				case SUCCESS:\
					printf(GREEN_COLOR);\
					printf("Success\n");\
					printf(DEFAULT_COLOR);\
					break;\
				case FAILED:\
					printf(RED_COLOR);\
					printf("Fail\n");\
					printf(DEFAULT_COLOR);\
					break;\
				case DONE:\
					printf(BLUE_COLOR);\
					printf("Done\n");\
					printf(DEFAULT_COLOR);\
			};\
		}

int TestUIDCreate(void);
int TestBadUIDCreate(void);
int TestUIDISEqual(void);

ilrd_uid_t uid1 = {0};
ilrd_uid_t uid2 = {0};

int main(int argc, char *argv[])
{
	RUN_TEST(TestUIDCreate(),					"UIDCreate");
	RUN_TEST(TestBadUIDCreate(),				"BadUIDCreate");
	RUN_TEST(TestUIDISEqual(),					"UIDISEqual");


	(void) argc;
	(void) argv;

    return 0;
}

int TestUIDCreate()
{
	int result = 0;
	
	uid1 = UIDCreate();
	result += (-1 != uid1.pid);
	result += (-1 != uid1.time_stamp);
	result += (0 != uid1.counter);
	
	return (3 == result);
}


int TestBadUIDCreate()
{
	int result = 0;
	
	uid2 = BadUIDCreate();
	result += (-1 == uid2.pid);
	result += (-1 == uid2.time_stamp);
	result += (0 == uid2.counter);
	
	return (3 == result);
}


int TestUIDISEqual()
{
	int result = 0;
	
	result += (0 == UIDISEqual(&uid1, &uid2));
	uid2 = UIDCreate();
	result += (0 == UIDISEqual(&uid1, &uid2));
	uid1 = BadUIDCreate();
	uid2 = BadUIDCreate();
	result += (1 == UIDISEqual(&uid1, &uid2));
	
	return (3 == result);
}

