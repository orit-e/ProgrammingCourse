#include <iostream>
#include <dlfcn.h>
#include <cstdio>
#include <unistd.h>

#include "dir_monitor.hpp"

using namespace ilrd;

enum PRINT_COLOR
{
    RESET_STYLE = 0,
    RED_COLOR = 31,
    GREEN_COLOR,
    YELLOW_COLOR,
    BLUE_COLOR,
    MAGENTA_COLOR
};

typedef int(*addFunc_t)(int, int);

void TestDirMonitorAndDLLLoaderCreationAndConnection();
void TestLibMovedToDir();
void TestLibModifiedInDir();

void InitFuncForTests(void *symHandler);
void PrintTestResult(int result);

int g_x = 1;
int g_y = 2;
int g_result;

int main()
{
    TestDirMonitorAndDLLLoaderCreationAndConnection();
    TestLibMovedToDir();
    TestLibModifiedInDir();

    return 0;
}

void TestDirMonitorAndDLLLoaderCreationAndConnection()
{
    std::cout << "TestDirMonitorAndDLLLoaderCreationAndConnection: " 
                << std::endl;

    DirMonitor *dM = new DirMonitor("./test_dir");
    DLLLoader *dL = new DLLLoader(*dM, InitFuncForTests);

    int leftToSleep = 1;
    do
    {
        leftToSleep = sleep(leftToSleep);
    }
    while(0 != leftToSleep);
    std::rename("./some_text_file.txt", "./test_dir/some_text_file.txt");
    leftToSleep = 1;
    do
    {
        leftToSleep = sleep(leftToSleep);
    }
    while(0 != leftToSleep);
    int result = 0;

    delete dL;
    delete dM;
    std::rename("./test_dir/some_text_file.txt", "./some_text_file.txt");

    PrintTestResult(result);
}

void TestLibMovedToDir()
{
    std::cout << "TestLibMovedToDir: " << std::endl;
    DirMonitor dM("./test_dir");
    DLLLoader dL(dM, &InitFuncForTests);
    int leftToSleep = 1;
    do
    {
        leftToSleep = sleep(leftToSleep);
    }
    while(0 != leftToSleep);
    system("gcc -shared -fpic -o libfunc.so func1.o");
    std::rename("./libfunc.so", "./test_dir/libfunc.so");
    leftToSleep = 1;
    do
    {
        leftToSleep = sleep(leftToSleep);
    }
    while(0 != leftToSleep);
    int result = (3 != g_result);
    
    PrintTestResult(result);
}

void TestLibModifiedInDir()
{
    std::cout << "TestLibModifiedInDir: " << std::endl;
    DirMonitor dM("./test_dir");
    DLLLoader dL(dM, &InitFuncForTests);
    int leftToSleep = 1;
    do
    {
        leftToSleep = sleep(leftToSleep);
    }
    while(0 != leftToSleep);
    system("gcc -shared -fpic -o libfunc.so func2.o");
    std::rename("./libfunc.so", "./test_dir/libfunc.so");
    leftToSleep = 1;
    do
    {
        leftToSleep = sleep(leftToSleep);
    }
    while(0 != leftToSleep);
    int result = (6 != g_result);

    PrintTestResult(result);
}

void InitFuncForTests(void *symHandler)
{
    addFunc_t add = reinterpret_cast<addFunc_t>(dlsym(symHandler, "Add"));
    g_result = add(g_x, g_y);
    dlclose(symHandler);
    dlerror();
}

void PrintTestResult(int result)
{
    (0 == result) ? 
    std::cout << "\033[" << GREEN_COLOR << "m"
              << "success!" 
              << "\033[" << RESET_STYLE << "m"  << std::endl :
    std::cout << "\033[" << RED_COLOR << "m" 
              << "test failed with " << result << " errors." 
              << "\033[" << RESET_STYLE << "m" << std::endl;            
}