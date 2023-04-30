#include "emulator.h"

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <cstring>

#include "log.h"

#define FONTSET_SIZE            80
#define FONTSET_START_ADDRESS   0x50

#define ROM_START_ADDRESS       0x200

unsigned char fontset[FONTSET_SIZE] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

Emulator::Emulator(const OS *os) :
                        os_(os),
                        registers_{0},
                        memories_{0},
                        pc_(ROM_START_ADDRESS) {
    strncpy((char *)&memories_[FONTSET_START_ADDRESS], (char *)fontset, FONTSET_SIZE);
}

Emulator::~Emulator() {

}

int 
Emulator::LoadROM(const char *pathname) {
    int fd, len;

    fd = open(pathname, O_RDONLY);
    if (fd < 0) {
        log_error("Failed to open %s, err: %s\n", pathname, strerror(errno));
        return -1;
    }

    len = read(fd, &memories_[ROM_START_ADDRESS], sizeof(memories_) - ROM_START_ADDRESS);
    close(fd);
    if (len < 0) {
        log_error("Failed to read instructions, err: %s\n", strerror(errno));
        return -1;
    }

    log_info("Loading opcodes into ROM is successful\n");
    return 0; 
}

static int ii = 0;

int 
Emulator::Process() { 
    opcode_ = memories_[pc_] << 8u | memories_[pc_ + 1];
    pc_ += 2;

    if (opcode_ & 0xFFFF == 0x00E0) {
        OP_00E0();
    } else if (opcode_ & 0xFFFF == 0x00EE) {
        OP_00EE();
    } else if (opcode_ & 0xF000 == 0x1000) {
        OP_1nnn();
    } else if (opcode_ & 0xF000 == 0x2000) {
        OP_2nnn();
    } else if (opcode_ & 0xF000 == 0x3000) {
        OP_3xkk();
    } else if (opcode_ & 0xF000 == 0x4000) {
        OP_4xkk();
    } else if (opcode_ & 0xF00F == 0x5000) {
        OP_5xy0();
    } else if (opcode_ & 0xF000 == 0x6000) {
        OP_6xkk();
    } else if (opcode_ & 0xF000 == 0x7000) {
        OP_7xkk();
    } else if (opcode_ & 0xF00F == 0x8000) {
        OP_8xy0();
    } else if (opcode_ & 0xF00F == 0x8001) {
        OP_8xy1();
    } else if (opcode_ & 0xF00F == 0x8002) {
        OP_8xy2();
    } else if (opcode_ & 0xF00F == 0x8003) {
        OP_8xy3();
    } else if (opcode_ & 0xF00F == 0x8004) {
        OP_8xy4();
    } else if (opcode_ & 0xF00F == 0x8005) {
        OP_8xy5();
    } else if (opcode_ & 0xF00F == 0x8006) {
        OP_8xy6();
    } else if (opcode_ & 0xF00F == 0x8007) {
        OP_8xy7();
    } else if (opcode_ & 0xF00F == 0x800E) {
        OP_8xyE();
    } else if (opcode_ & 0xF00F == 0x9000) {
        OP_9xy0();
    } else if (opcode_ & 0xF000 == 0xA000) {
        OP_Annn();
    } else if (opcode_ & 0xF000 == 0xB000) {
        OP_Bnnn();
    } else if (opcode_ & 0xF000 == 0xC000) {
        OP_Cxkk();
    } else if (opcode_ & 0xF000 == 0xD000) {
        OP_Dxyn();
    } else if (opcode_ & 0xF0FF == 0xE09E) {
        OP_Ex9E();
    } else if (opcode_ & 0xF0FF == 0xE0A1) {
        OP_ExA1();
    } else if (opcode_ & 0xF0FF == 0xF007) {
        OP_Fx07();
    } else if (opcode_ & 0xF0FF == 0xF00A) {
        OP_Fx0A();
    } else if (opcode_ & 0xF0FF == 0xF015) {
        OP_Fx15();
    } else if (opcode_ & 0xF0FF == 0xF018) {
        OP_Fx18();
    } else if (opcode_ & 0xF0FF == 0xF01E) {
        OP_Fx1E();
    } else if (opcode_ & 0xF0FF == 0xF029) {
        OP_Fx29();
    } else if (opcode_ & 0xF0FF == 0xF033) {
        OP_Fx33();
    } else if (opcode_ & 0xF0FF == 0xF055) {
        OP_Fx55();
    } else if (opcode_ & 0xF0FF == 0xF065) {
        OP_Fx65();
    }

    if (ii++ < 200)
        log_info("%X\n", opcode_);

    return 0; 
}

void 
Emulator::OP_00E0() {
    memset(display_, 0, sizeof(display_));
}

void 
Emulator::OP_00EE() {
    --sp_;
    pc_ = stack_[sp_];
}

void 
Emulator::OP_1nnn() {
    pc_ = opcode_ & 0x0FFFu;
}

void 
Emulator::OP_2nnn() {
    stack_[sp_] = pc_;
    sp_ = sp_ + 1;
    pc_ = opcode_ & 0x0FFFu;
}

void 
Emulator::OP_3xkk() {
    unsigned char Vx = (opcode_ & 0x0F00u) >> 8u;
    unsigned short byte = opcode_ & 0x00FFu;

    if (registers_[Vx] == byte)
        pc_ += 2;
}

void 
Emulator::OP_4xkk() {
    unsigned char Vx = (opcode_ & 0x0F00u) >> 8u;
    unsigned short byte = opcode_ & 0x00FFu;

    if (registers_[Vx] != byte)
        pc_ += 2;
}

void 
Emulator::OP_5xy0() {
    unsigned char Vx = (opcode_ & 0x0F00u) >> 8u;
    unsigned char Vy = (opcode_ & 0x00F0u) >> 4u;

    if (registers_[Vx] == registers_[Vy])
        pc_ += 2;
}

void 
Emulator::OP_6xkk() {
    unsigned char Vx = (opcode_ & 0x0F00u) >> 8u;
    unsigned short byte = opcode_ & 0x00FFu;
    
    registers_[Vx] = byte;
}

void 
Emulator::OP_7xkk() {
    unsigned char Vx = (opcode_ & 0x0F00u) >> 8u;
    unsigned short byte = opcode_ & 0x00FFu;
    
    registers_[Vx] += byte;
}

void 
Emulator::OP_8xy0() {
    unsigned char Vx = (opcode_ & 0x0F00u) >> 8u;
    unsigned char Vy = (opcode_ & 0x00F0u) >> 4u;

    registers_[Vx] = registers_[Vy];
}

void 
Emulator::OP_8xy1() {
    unsigned char Vx = (opcode_ & 0x0F00u) >> 8u;
    unsigned char Vy = (opcode_ & 0x00F0u) >> 4u;

    registers_[Vx] = registers_[Vx] | registers_[Vy];
}

void 
Emulator::OP_8xy2() {
    unsigned char Vx = (opcode_ & 0x0F00u) >> 8u;
    unsigned char Vy = (opcode_ & 0x00F0u) >> 4u;

    registers_[Vx] = registers_[Vx] & registers_[Vy];
}

void 
Emulator::OP_8xy3() {
    unsigned char Vx = (opcode_ & 0x0F00u) >> 8u;
    unsigned char Vy = (opcode_ & 0x00F0u) >> 4u;

    registers_[Vx] = registers_[Vx] ^ registers_[Vy];
}

void Emulator::OP_8xy4() {
    unsigned char Vx = (opcode_ & 0x0F00u) >> 8u;
    unsigned char Vy = (opcode_ & 0x00F0u) >> 4u;

    unsigned short sum = registers_[Vx] + registers_[Vy];
    if (sum > 255u) {
        registers_[0xF] = 1;
    } else {
        registers_[0xF] = 0;
    }

    registers_[Vx] = sum & 0xFFu;
}

void 
Emulator::OP_8xy5() {
    unsigned char Vx = (opcode_ & 0x0F00u) >> 8u;
    unsigned char Vy = (opcode_ & 0x00F0u) >> 4u;

    if (registers_[Vx] > registers_[Vy]) {
        registers_[0xF] = 1;
    } else {
        registers_[0xF] = 0;
    }

    registers_[Vx] -= registers_[Vy];
}

void 
Emulator::OP_8xy6() {
    unsigned char Vx = (opcode_ & 0x0F00u) >> 8u;

    registers_[0xF] = (registers_[Vx] & 0x1u);
    registers_[Vx] >>= 1;
}

void 
Emulator::OP_8xy7() {
    unsigned char Vx = (opcode_ & 0x0F00u) >> 8u;
    unsigned char Vy = (opcode_ & 0x00F0u) >> 4u;

    if (registers_[Vy] > registers_[Vx])  {
        registers_[0xF] = 1;
    } else {
        registers_[0xF] = 0;
    }

    registers_[Vx] = registers_[Vy] - registers_[Vx];
}

void 
Emulator::OP_8xyE() {
    unsigned char Vx = (opcode_ & 0x0F00u) >> 8u;

    registers_[0xF] = (registers_[Vx] & 0x80u) >> 7u;
    registers_[Vx] <<= 1;
}

void 
Emulator::OP_9xy0() {
    unsigned char Vx = (opcode_ & 0x0F00u) >> 8u;
    unsigned char Vy = (opcode_ & 0x00F0u) >> 4u;

    if (registers_[Vx] != registers_[Vy]) {
        pc_ += 2;
    }
}

void 
Emulator::OP_Annn() {
    unsigned short addr = opcode_ & 0xFFFu;

    index_ = addr;
}

void 
Emulator::OP_Bnnn() {
    unsigned short addr = opcode_ & 0xFFFu;

    pc_ = registers_[0] + addr;
}

void 
Emulator::OP_Cxkk() {
    unsigned char Vx = (opcode_ & 0x0F00u) >> 8u;
    unsigned char byte = opcode_ & 0x00FFu;

    // registers_[Vx] = byte;
}

void 
Emulator::OP_Dxyn() {

}

void 
Emulator::OP_Ex9E() {
    unsigned char Vx = (opcode_ & 0x0F00u) >> 8u;
    unsigned char key = registers_[Vx];

    if (keypads_[key]) {
        pc_ += 2;
    }
}

void 
Emulator::OP_ExA1() {
    unsigned char Vx = (opcode_ & 0x0F00u) >> 8u;
    unsigned char key = registers_[Vx];

    if (!keypads_[key]) {
        pc_ += 2;
    }
}

void 
Emulator::OP_Fx07() {
    unsigned char Vx = (opcode_ & 0x0F00u) >> 8u;

    registers_[Vx] = delay_timer_;
}

void 
Emulator::OP_Fx0A() {
    unsigned char Vx = (opcode_ & 0x0F00u) >> 8u;

    for (int i = 0; i < 16; i++) {
        if (keypads_[i])
            registers_[Vx] = i;
        else if (i == 16)
            pc_ -= 2;
    }
}

void 
Emulator::OP_Fx15() {
    unsigned char Vx = (opcode_ & 0x0F00u) >> 8u;

    delay_timer_ = registers_[Vx];
}

void 
Emulator::OP_Fx18() {
    unsigned char Vx = (opcode_ & 0x0F00u) >> 8u;

    sound_timer_ = registers_[Vx];
}

void 
Emulator::OP_Fx1E() {
    unsigned char Vx = (opcode_ & 0x0F00u) >> 8u;

    index_ += registers_[Vx];
}

void 
Emulator::OP_Fx29() {
    unsigned char Vx = (opcode_ & 0x0F00u) >> 8u;
    unsigned char digit = registers_[Vx];

    index_ = FONTSET_START_ADDRESS + (5 * digit);
}

void 
Emulator::OP_Fx33() {
    unsigned char Vx = (opcode_ & 0x0F00u) >> 8u;
    unsigned char value = registers_[Vx];

    memories_[index_ + 2] = value % 10;
    value /= 10;

    memories_[index_ + 1] = value % 10;
    value /= 10;

    memories_[index_] = value % 10;
}

void 
Emulator::OP_Fx55() {
    unsigned char Vx = (opcode_ & 0x0F00u) >> 8u;

    for (int i = 0; i < Vx; ++i) {
        memories_[index_ + i] = registers_[i];
    }
}

void 
Emulator::OP_Fx65() {
    unsigned char Vx = (opcode_ & 0x0F00u) >> 8u;

    for (int i = 0; i <= Vx; ++i) {
        registers_[i] = memories_[index_ + i];
    }
}
