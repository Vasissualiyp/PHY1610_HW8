#include <iostream>
#include <cstdlib>
#include <string>

int main(int argc, char *argv[]) {
    std::string OUT_FILE = "longwaveparams.txt";

    system("make clean");
    system("make fullclean");
    std::string command = "export OMP_NUM_THREADS=" + std::string(argv[1]);
    system(command.c_str());
    system("make -j 10");
    command = "./wave2d " + OUT_FILE;
    system(command.c_str());

    return 0;
}

