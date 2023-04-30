#ifndef __EMULATOR_H__
#define __EMULATOR_H__

#include "os/os.h"

class Emulator {
    public:
        Emulator() = delete;
        Emulator(const OS *os);
        ~Emulator();

        int LoadROM(const char *pathname);

        int Process();

    private:
        unsigned char registers_[16];
        unsigned char memories_[4096];
        unsigned short index_;
        unsigned short pc_;
        unsigned short stack_[16];
        unsigned char sp_;
        unsigned char delay_timer_;
        unsigned char sound_timer_;
        unsigned keypads_[16];
        unsigned int display_[64 * 32];
        unsigned short opcode_;
        const OS *os_;

        void OP_00E0();
        void OP_00EE();
        void OP_1nnn();
        void OP_2nnn();
        void OP_3xkk();
        void OP_4xkk();
        void OP_5xy0();
        void OP_6xkk();
        void OP_7xkk();
        void OP_8xy0();
        void OP_8xy1();
        void OP_8xy2();
        void OP_8xy3();
        void OP_8xy4();
        void OP_8xy5();
        void OP_8xy6();
        void OP_8xy7();
        void OP_8xyE();
        void OP_9xy0();
        void OP_Annn();
        void OP_Bnnn();
        void OP_Cxkk();
        void OP_Dxyn();
        void OP_Ex9E();
        void OP_ExA1();
        void OP_Fx07();
        void OP_Fx0A();
        void OP_Fx15();
        void OP_Fx18();
        void OP_Fx1E();
        void OP_Fx29();
        void OP_Fx33();
        void OP_Fx55();
        void OP_Fx65();
};

#endif // __EMULATOR_H__