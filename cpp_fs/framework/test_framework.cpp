#include "framework.hpp"

int main()
{
    setenv(ilrd::Framework::FW_PLUG_IN_DIR_ENV, "./test_dir", 1);
    setenv("LOGGER_FILE_PATH", "./log.txt", 1);

    ilrd::Framework& fw = ilrd::Singleton<ilrd::Framework>::GetInstance();
    sleep(1);

    //system("mv ./libtest.so ./test_dir/");
    system("mv ./libfoo.so ./test_dir/");
    //rename("./libtest.so", "./test_dir/libtest.so");
    sleep(1);

/*    int fifoFd = open("./test_lib.fifo", 0666 | O_RDWR);
    write(fifoFd, "Hello World!", 32);
    sleep(1);
    write(fifoFd, "Goodbye World!", 32);
    sleep(1);
*/
    //rename("./test_dir/libtest.so", "./libtest.so");
    //system("rm ./test_dir/libtest.so");
    sleep(1);
    system("rm ./test_dir/libfoo.so");
    sleep(1);

//    close(fifoFd);

    return 0;
}
