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
        MOV,
        WHERE,
        NOT,
        NOR,
        SELECT_PATTERN,
        ALL,
    } opcode_t;

    typedef enum AREG{
        A,
        B,
        C,
        D,
        E,
        F,
        NEWS,
    } areg_t ;


    typedef enum DREG{
        R0,
        R1,
        R2,
        R3,
        R4,
        R5,
        R6,
        R7,
        R8,
        R9,
        R10,
        R11,
        R12,
        FLAG
    } dreg_t;
}


#endif //CPA_SIM_CPA_H
