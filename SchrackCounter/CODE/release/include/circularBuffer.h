#ifndef _CIRCULAR_BUFFER_H
#define _CIRCULAR_BUFFER_H
#include <Arduino.h>


// <1bit in 8bit>
struct circularBuffer1bit8 {
private:
  uint16_t nOfCells        = 0;
  uint16_t headPosRead     = 0;
  uint16_t headPosWrite    = 0;
  uint8_t* circBufferArray = NULL;

public:
  circularBuffer1bit8(uint16_t nOfCells);  // Constructor.
  uint8_t write(uint8_t bitToWrite);       // Write.
  uint8_t read(uint8_t* readVal);          // Read.
};


// Constructor.
circularBuffer1bit8::circularBuffer1bit8(uint16_t nOfCells) {
  this->nOfCells = nOfCells;  // Set cell size.

  uint8_t nOfCellsP1 = 0;  // Add a full byte for any remaining bits.
  if(nOfCells % 8 > 1) {
    nOfCellsP1 = 1;
  }
  uint16_t circBufferArraySize = (nOfCells - nOfCells % 8) / 8 + nOfCellsP1;  // Take n of bytes. N = nOfCells/8 rounded up.
  uint8_t  circBufferArray[circBufferArraySize];                              // Create the array.
  this->circBufferArray = circBufferArray;                                    // Save pointer.
}


/**
 * @param bitToWrite One bit to write to the array.
 * @returns Error (0|1) - (ok|buffer full).
 * @brief Write a bit to the buffer.
 */
uint8_t circularBuffer1bit8::write(uint8_t bitToWrite) {
  if((headPosWrite + 1) % nOfCells == headPosRead) {  // Check head colision.
    return 1;                                         // Buffer full.
  }

  uint8_t subBytePos = headPosWrite & 0b111;                              // Position within the byte to write to.
  uint8_t readByte   = circBufferArray[(headPosWrite - subBytePos) / 8];  // Value of the byte to write to.
  circBufferArray[(headPosWrite - subBytePos) / 8] =
  ((bitToWrite & 1) << subBytePos) ^ (readByte & ~(1 << subBytePos));  // Write the bit to the byte.
  headPosWrite = (headPosWrite + 1) % nOfCells;                        // Move head.
  return 0;
}


/**
 * @param readVal Pointer to where to save the read bit to.
 * @returns Error (0|1) - (ok|buffer empty).
 * @brief Reads a bit from the buffer.
 */
uint8_t circularBuffer1bit8::read(uint8_t* readVal) {
  if(headPosWrite == headPosRead) {  // Check head colision.
    return 1;                        // Buffer empty.
  }

  uint8_t subBytePos = headPosRead & 0b111;  // Position within the byte to write to.

  *readVal = (circBufferArray[(headPosRead - subBytePos) / 8] & (1 << subBytePos)) >>
             subBytePos;  // Read the whole byte and mask out the bit.

  headPosRead = (headPosRead + 1) % nOfCells;  // Move head.
  return 0;
}
// </1bit in 8bit>

// ================================

// <1bit in 16bit>
struct circularBuffer1bit16 {
private:
  uint16_t  nOfCells        = 0;
  uint16_t  headPosRead     = 0;
  uint16_t  headPosWrite    = 0;
  uint16_t* circBufferArray = NULL;

public:
  circularBuffer1bit16(uint16_t nOfCells);  // Constructor.
  uint8_t write(uint8_t bitToWrite);        // Write.
  uint8_t read(uint8_t* readVal);           // Read.
};


// Constructor.
circularBuffer1bit16::circularBuffer1bit16(uint16_t nOfCells) {
  this->nOfCells = nOfCells;  // Set cell size.

  uint8_t nOfCellsP1 = 0;  // Add a full byte for any remaining bits.
  if(nOfCells % 16 > 1) {
    nOfCellsP1 = 1;
  }
  uint16_t circBufferArraySize = (nOfCells - nOfCells % 16) / 16 + nOfCellsP1;  // Take n of bytes. N = nOfCells/16 rounded up.
  uint16_t circBufferArray[circBufferArraySize];                                // Create the array.
  this->circBufferArray = circBufferArray;                                      // Save pointer.
}


/**
 * @param bitToWrite One bit to write to the array.
 * @returns Error (0|1) - (ok|buffer full).
 * @brief Write a bit to the buffer.
 */
uint8_t circularBuffer1bit16::write(uint8_t bitToWrite) {
  if((headPosWrite + 1) % nOfCells == headPosRead) {  // Check head colision.
    return 1;                                         // Buffer full.
  }

  uint8_t subBytePos = headPosWrite & 0b1111;                             // Position within the byte to write to.
  uint8_t readByte   = circBufferArray[(headPosWrite - subBytePos) / 8];  // Value of the byte to write to.
  circBufferArray[(headPosWrite - subBytePos) / 8] =
  ((bitToWrite & 1) << subBytePos) ^ (readByte & ~(1 << subBytePos));  // Write the bit to the byte.
  headPosWrite = (headPosWrite + 1) % nOfCells;                        // Move head.
  return 0;
}


/**
 * @param readVal Pointer to where to save the read bit to.
 * @returns Error (0|1) - (ok|buffer empty).
 * @brief Reads a bit from the buffer.
 */
uint8_t circularBuffer1bit16::read(uint8_t* readVal) {
  if(headPosWrite == headPosRead) {  // Check head colision.
    return 1;                        // Buffer empty.
  }

  uint8_t subBytePos = headPosRead & 0b1111;  // Position within the byte to write to.
  *readVal           = (circBufferArray[(headPosRead - subBytePos) / 8] & (1 << subBytePos)) >>
             subBytePos;                       // Read the whole byte and mask out the bit.
  headPosRead = (headPosRead + 1) % nOfCells;  // Move head.
  return 0;
}
// <1bit in 16bit>

// ================================

// <1bit in 32bit>
struct circularBuffer1bit32 {
private:
  uint16_t  nOfCells        = 0;
  uint16_t  headPosRead     = 0;
  uint16_t  headPosWrite    = 0;
  uint32_t* circBufferArray = NULL;

public:
  circularBuffer1bit32(uint16_t nOfCells);  // Constructor.
  uint8_t write(uint8_t bitToWrite);        // Write.
  uint8_t read(uint8_t* readVal);           // Read.
};


// Constructor.
circularBuffer1bit32::circularBuffer1bit32(uint16_t nOfCells) {
  this->nOfCells = nOfCells;  // Set cell size.

  uint8_t nOfCellsP1 = 0;  // Add a full byte for any remaining bits.
  if(nOfCells % 32 > 1) {
    nOfCellsP1 = 1;
  }
  uint16_t circBufferArraySize = (nOfCells - nOfCells % 32) / 32 + nOfCellsP1;  // Take n of bytes. N = nOfCells/32 rounded up.
  uint32_t circBufferArray[circBufferArraySize];                                // Create the array.
  this->circBufferArray = circBufferArray;                                      // Save pointer.
}


/**
 * @param bitToWrite One bit to write to the array.
 * @returns Error (0|1) - (ok|buffer full).
 * @brief Write a bit to the buffer.
 */
uint8_t circularBuffer1bit32::write(uint8_t bitToWrite) {
  if((headPosWrite + 1) % nOfCells == headPosRead) {  // Check head colision.
    return 1;                                         // Buffer full.
  }

  uint8_t subBytePos = headPosWrite & 0b11111;                            // Position within the byte to write to.
  uint8_t readByte   = circBufferArray[(headPosWrite - subBytePos) / 8];  // Value of the byte to write to.
  circBufferArray[(headPosWrite - subBytePos) / 8] =
  ((bitToWrite & 1) << subBytePos) ^ (readByte & ~(1 << subBytePos));  // Write the bit to the byte.
  headPosWrite = (headPosWrite + 1) % nOfCells;                        // Move head.
  return 0;
}


/**
 * @param readVal Pointer to where to save the read bit to.
 * @returns Error (0|1) - (ok|buffer empty).
 * @brief Reads a bit from the buffer.
 */
uint8_t circularBuffer1bit32::read(uint8_t* readVal) {
  if(headPosWrite == headPosRead) {  // Check head colision.
    return 1;                        // Buffer empty.
  }

  uint8_t subBytePos = headPosRead & 0b11111;  // Position within the byte to write to.
  *readVal           = (circBufferArray[(headPosRead - subBytePos) / 8] & (1 << subBytePos)) >>
             subBytePos;                       // Read the whole byte and mask out the bit.
  headPosRead = (headPosRead + 1) % nOfCells;  // Move head.
  return 0;
}
// <1bit in 32bit>

// ================================

// <1bit in 64bit>
struct circularBuffer1bit64 {
private:
  uint16_t  nOfCells        = 0;
  uint16_t  headPosRead     = 0;
  uint16_t  headPosWrite    = 0;
  uint64_t* circBufferArray = NULL;

public:
  circularBuffer1bit64(uint16_t nOfCells);  // Constructor.
  uint8_t write(uint8_t bitToWrite);        // Write.
  uint8_t read(uint8_t* readVal);           // Read.
};


// Constructor.
circularBuffer1bit64::circularBuffer1bit64(uint16_t nOfCells) {
  this->nOfCells = nOfCells;  // Set cell size.

  uint8_t nOfCellsP1 = 0;  // Add a full byte for any remaining bits.
  if(nOfCells % 64 > 1) {
    nOfCellsP1 = 1;
  }
  uint16_t circBufferArraySize = (nOfCells - nOfCells % 64) / 64 + nOfCellsP1;  // Take n of bytes. N = nOfCells/64 rounded up.
  uint64_t circBufferArray[circBufferArraySize];                                // Create the array.
  this->circBufferArray = circBufferArray;                                      // Save pointer.
}


/**
 * @param bitToWrite One bit to write to the array.
 * @returns Error (0|1) - (ok|buffer full).
 * @brief Write a bit to the buffer.
 */
uint8_t circularBuffer1bit64::write(uint8_t bitToWrite) {
  if((headPosWrite + 1) % nOfCells == headPosRead) {  // Check head colision.
    return 1;                                         // Buffer full.
  }

  uint8_t subBytePos = headPosWrite & 0b111111;                           // Position within the byte to write to.
  uint8_t readByte   = circBufferArray[(headPosWrite - subBytePos) / 8];  // Value of the byte to write to.
  circBufferArray[(headPosWrite - subBytePos) / 8] =
  ((bitToWrite & 1) << subBytePos) ^ (readByte & ~(1 << subBytePos));  // Write the bit to the byte.
  headPosWrite = (headPosWrite + 1) % nOfCells;                        // Move head.
  return 0;
}


/**
 * @param readVal Pointer to where to save the read bit to.
 * @returns Error (0|1) - (ok|buffer empty).
 * @brief Reads a bit from the buffer.
 */
uint8_t circularBuffer1bit64::read(uint8_t* readVal) {
  if(headPosWrite == headPosRead) {  // Check head colision.
    return 1;                        // Buffer empty.
  }

  uint8_t subBytePos = headPosRead & 0b111111;  // Position within the byte to write to.
  *readVal           = (circBufferArray[(headPosRead - subBytePos) / 8] & (1 << subBytePos)) >>
             subBytePos;                       // Read the whole byte and mask out the bit.
  headPosRead = (headPosRead + 1) % nOfCells;  // Move head.
  return 0;
}
// <1bit in 64bit>

#endif
