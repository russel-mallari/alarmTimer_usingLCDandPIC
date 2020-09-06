#ifndef MACROS_H
#define MACROS_H

#define checkBit(regAddress, bitNum)        ((regAddress & (1U << bitNum)) >> bitNum)
#define writeBitField(regAddress, bitMask, value)       (regAddress = ((regAddress & ~(bitMask)) | (value)))
#define setBit(regAddress, bitNum)          (regAddress = (regAddress | (1U << bitNum)))
#define clearBit(regAddress, bitNum)        (regAddress = (regAddress & ~(1U << bitNum)))
#define toggleBit(regAddress, bitNum)       (regAddress = (regAddress ^ (1U << bitNum)))

#endif // MACROS_H