#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>

#include <cstring>

#include "emulator.h"
#include "os/linux.h"
#include "log.h"

int main(int argc, char *argv[]) {
    const char *pathname = nullptr;
    int opt, ret;
    
    struct option long_options[] {
        {"file", required_argument, nullptr, 'f'},
        {nullptr, 0, nullptr, 0}
    };

    do {
        opt = getopt_long(argc, argv, "f:", long_options, nullptr);
        switch (opt) {
            case 'f':
                pathname = optarg;
                break;
            
            default:
                break;
        }
    } while (opt != -1);

    OS *os = new Linux();
    Emulator emulator(os);

    ret = emulator.LoadROM(pathname);
    if (ret < 0) {
        log_error("Failed to load instructions to ROM\n");
        return -1;
    }

    while (true) {
        ret = emulator.Process();
        if (ret < 0) {
            log_error("Occur a error when perform opcode\n");
            break;
        }
    }

    delete os;
    return 0;
}