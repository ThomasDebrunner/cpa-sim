//
// Created by najiji on 19.05.18.
//

#ifndef CPA_SIM_CPA_DEF_H
#define CPA_SIM_CPA_DEF_H

#include <cstdint>

typedef int16_t dac_t;
typedef uint8_t addr_t;


namespace SCAMP {
    typedef enum {
        RPIX,
        ADD,
        SUB,
        NORTH,
        EAST,
        SOUTH,
        WEST,
        IN,
        INV,
        MOV,
        WHERE,
        NOT,
        NOR,
        SELECT_PATTERN,
        ALL,
        DIV2,
    } opcode_t;

    typedef enum AREG{
        A = 1,
        B = 2,
        C = 3,
        D = 4,
        E = 5,
        F = 6,
        NEWS = 7,
    } areg_t ;


    typedef enum DREG{
        R0 = 101,
        R1 = 102,
        R2 = 103,
        R3 = 104,
        R4 = 105,
        R5 = 106,
        R6 = 107,
        R7 = 108,
        R8 = 109,
        R9 = 110,
        R10 = 111,
        R11 = 112,
        R12 = 113,
        FLAG = 114
    } dreg_t;
}


#endif //CPA_SIM_CPA_H
