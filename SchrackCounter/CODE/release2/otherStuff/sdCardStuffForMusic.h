#include <Arduino.h>
#include "schrackButtonPins.h"

#include <SPI.h>
#include <SD.h>

/**
 * @param spiSdCs Chip select for the sd card.
 * @brief Sets up the sd.h lib.
 */
void sdSetup(uint8_t spiClk, uint8_t spiMiso, uint8_t spiMosi, uint8_t spiSdCs) {
  SPI.begin(spiClk, spiMiso, spiMosi, spiSdCs);
  while(!SD.begin(spiSdCs, SPI)) {
    log_e("Sd card begin error. Next att in 1s.");
    delay(1000);
  }
}


/**
 * @param tableName Filepath.
 * @returns Error (0|1|2) - (ok|doesn't exist|not a file).
 * @brief Checks if a file exists and is a file .
 */
uint8_t checkFile(const char* tableName) {
  // Doesn't exist.
  if(!SD.exists(tableName)) {
    log_e("File doesn't exist.");
    return 1;
  }

  File tableFile = SD.open(tableName, FILE_READ);

  // Not a file.
  if(tableFile.isDirectory() == 1) {
    tableFile.close();
    log_e("Not a file.");
    return 2;
  }

  tableFile.close();
  return 0;
}


/**
 * @param tableName Name of the source file.
 * @param tableNameLen Name of the copy file.
 * @brief Creates a copy of a file named <tableNameCopy>.
 */
void cloneFile(const char* tableName, char* tableNameCopy) {
  File sourceFile = SD.open(tableName, FILE_READ);       // Open the source file.
  File cloneFile  = SD.open(tableNameCopy, FILE_WRITE);  // Create the copy file.

  for(uint16_t i = 0; i < sourceFile.size(); i++) {
    uint8_t tempByte = sourceFile.read();
    if(tempByte != -1) {
      if(cloneFile.write(tempByte) == -1) {
        sourceFile.close();
        cloneFile.close();
        return;
      }
    } else {
      sourceFile.close();
      cloneFile.close();
      return;
    }
  }

  sourceFile.close();
  cloneFile.close();
}


/**
 * @param tableName Name of the clone file.
 * @brief Deletes a file.
 */
void cloneFileDelete(const char* tableName) {
  SD.remove(tableName);
}


/**
 * @param tableName Table to edit.
 * @param tableNameLen  Filename length.
 * @param fileSize  File size.
 * @param CursorPosCellStart  Pos to start writing to.
 * @param cursorPosCellEnd  Pos to start reading from.
 * @param value String to insert at the start.
 * @param valueLen  Length of the inserted string.
 * @returns Error (0|1) - (ok|read byte error).
 * @brief Takes a position in a file, inserts a string and shifts the rest ofthe file do be right after the string.
 */
uint8_t ShiftBytesInAFile(const char* tableName,
                          uint8_t     tableNameLen,
                          uint8_t     fileSize,
                          uint16_t    CursorPosCellStart,
                          uint16_t    cursorPosCellEnd,
                          const char* value,
                          uint16_t    valueLen) {
  const char* extensionName = ".NATTMP";
  char        tableNameCopy[tableNameLen + 7];  // Array for the new name.

  for(uint8_t i = 0; i < tableNameLen - 1; i++) {  // Copy in the filename.
    tableNameCopy[i] = tableName[i];
  }
  for(uint8_t i = 0; i < 8; i++) {  // Add the extension name and a \0.
    tableNameCopy[i + tableNameLen - 1] = extensionName[i];
  }

  cloneFile(tableName, tableNameCopy);  // Make a copy.
  File tableFile     = SD.open(tableName, FILE_WRITE);
  File tableFileCopy = SD.open(tableNameCopy, FILE_READ);

  tableFile.seek(CursorPosCellStart);          // Move write cursor to start of the cell.
  tableFileCopy.seek(cursorPosCellEnd);        // Move read cursor to end of the cell.
  tableFile.write((uint8_t*)value, valueLen);  // Write in the value.

  for(uint8_t i = 0; i < fileSize - cursorPosCellEnd; i++) {  // Go through the rest of the read file.
    uint8_t tempByte[1];
    tableFileCopy.read(tempByte, 1);  // Read a value.
    if(tempByte[0] != -1) {
      tableFile.write(tempByte, 1);  // No error. Write the value.
    } else {
      log_e("Read byte error");
      tableFile.close();
      tableFileCopy.close();
      return 1;
    }
  }

  tableFile.close();
  tableFileCopy.close();

  cloneFileDelete(tableNameCopy);  // Delete the copy.
  return 0;
}


/**
 * @param tableFile The file object.
 * @param fileSize Size of the file in bytes.
 * @param row Target row.
 * @param column Target column.
 * @param lastCharWasCR Windows CR LF fix. Was last char CR (0|1) - (wasn't CR|was CR).
 * @param posReached Was the cell found (0|1).
 * @param createCells  Allow creation of new cells.
 * @param tableName  Name of the file. Needed only if creatin cells is enabled.
 * @param tableNameLen  Length of the file name. Needed only if creatin cells is enabled.
 * @returns Cursor position of the first char of the target cell. 0 if cell not found.
 * @brief Finds the first position of a cell. Leaves the cursor there.
 */
uint16_t cellStartPos(File        tableFile,
                      uint16_t    fileSize,
                      uint16_t    row,
                      uint16_t    column,
                      uint8_t*    lastCharWasCR,
                      uint8_t*    posReached,
                      uint8_t     createCells,
                      const char* tableName,
                      uint8_t     tableNameLen) {
  uint16_t rowI    = 0;  // Row counter.
  uint16_t columnI = 0;  // Column counter.
  posReached[0]    = 0;
  for(uint16_t fileBytePos = 0; fileBytePos < fileSize; fileBytePos++) {
    if(rowI == row && columnI == column) {  // Check if pos reached.
      log_e("posReached!");
      posReached[0] = 1;                    // Pos reached, set flag.
      return tableFile.position();
    }

    if(createCells == 1) {  // Create new cells.
      if((row == rowI - 1 && column != 0) ||
         (row == rowI && column == columnI - 1)) {  // If the cell was overshot by one, create a new cell  //DEBUG - HERE
        // create cell
        tableFile.seek(tableFile.position() - 1);
        if(ShiftBytesInAFile(tableName, tableNameLen, fileSize, tableFile.position(), tableFile.position(), ",", 1) !=
           0) {
          posReached[0] = 1;                    // Pos reached, set flag.
          return tableFile.position();
        }
      }
    } else if(rowI > row || (rowI == row && columnI > column)) {  // Check if the cell was overshot. It doesn't exist.
      break;                                                      // Don't set the flag.
    }

    if(lastCharWasCR[0] > 0) {  // Decrement the "was CR" counter.
      lastCharWasCR[0]--;
    }

    uint8_t readByte[1];
    tableFile.read(readByte, 1);  // Read a byte.
    switch(readByte[0]) {         // Check for a cell end.
      case '\r':                  // Carriage return.
        rowI++;                   // Advance row counter.
        columnI          = 0;     // Reset column counter.
        lastCharWasCR[0] = 2;     // Was CR.
        break;

      case '\n':                     // Line feed
        if(lastCharWasCR[0] == 0) {  // Fix for windows encoding. Ignore LF if it's after CR.
          rowI++;                    // Advance row counter.
          columnI = 0;               // Reset column counter.
        }
        break;

      case ',':  // End of a cell.
        //THIS
        columnI++;  // Increase column counter.
        break;

      case '"':  // "Escape char".
        //THIS
        break;
    }
  }

  return 0;
}


/**
 * @param tableName Filepath of the file.
 * @param tableNameLen Length of the filename.
 * @param row Row.
 * @param column  Column.
 * @param value Value to write
 * @param valueLen Length of the value array.
 * @param createCells Allow creating new cells (!1|1).
 * @returns Error (0|1|2|3|4) - (ok|file not found|cell not found|target cell read byte error|negative cell length).
 * @brief Writes a value to a cell.
 */
uint8_t writeCell(const char* tableName,
                  uint8_t     tableNameLen,
                  uint16_t    row,
                  uint16_t    column,
                  const char* value,
                  uint16_t    valueLen,
                  uint8_t     createCells) {
  valueLen--; // Remove the \0.

  if(checkFile(tableName) != 0) {  // Check if the file exists and is a file.
    return 1;
  }

  File    tableFile = SD.open(tableName, FILE_READ);  // Open the file.
  uint8_t fileSize  = tableFile.size();               // Size of the file in bytes.

  uint8_t  lastCharWasCR[1]   = {0};
  uint8_t  posReached[1]      = {0};
  uint16_t CursorPosCellStart = cellStartPos(tableFile, fileSize, row, column, lastCharWasCR, posReached, createCells,
                                             tableName, tableNameLen);  // Get cell pos.

  fileSize = tableFile.size();  // Update the file size.

  if(posReached[0] != 1) {  // Flag not set, return. The cell was not found.
    log_e("Cell doesn't exist.");
    tableFile.close();
    return 2;
  }

  uint16_t cursorPosCellEnd = CursorPosCellStart;
  for(uint8_t whileVal = 0xff; whileVal > 0; whileVal--) {  // Limted length loop. Search for the end of the cell.
    uint8_t readByte[1];
    if(tableFile.read(readByte, 1) == -1) {  // Try to read a byte.
      log_e("Read byte error");
      tableFile.close();
      return 3;
    }

    if(readByte[0] == ',' || readByte[0] == '\r' || readByte[0] == '\n') {  // End of cell.
      if(lastCharWasCR[0] == 1 && readByte[0] == '\n') {                    // Ignore a LF it the previous char was CR.
        lastCharWasCR[0] = 0;                                               // Reset flag.
        CursorPosCellStart++;                                               // Move cell start by one.
      } else {                                                              // End of cell reached.
        cursorPosCellEnd = tableFile.position();                            // Next cell start pos.
        break;
      }
    }
  }

  tableFile.close();

  if(cursorPosCellEnd - CursorPosCellStart < 0) {  // Something went very wrong.
    log_e("Negative cell length");
    return 4;
  }


  uint16_t cellLen = cursorPosCellEnd - CursorPosCellStart - 1;

  if(cellLen != valueLen) {  // Cell size changed.
    if(ShiftBytesInAFile(tableName, tableNameLen, fileSize, CursorPosCellStart, cursorPosCellEnd - 1, value, valueLen) ==
       1) {  //HERE
      log_e("Read byte error");
      return 3;
    }

  } else {                                       // Cell size didn't change
    tableFile = SD.open(tableName, FILE_WRITE);  // Open the file.
    tableFile.seek(CursorPosCellStart);          // Write.
    tableFile.write((uint8_t*)value, valueLen);

    tableFile.seek(fileSize);  // So it apparently deletes everything after the cursor.
  }


  tableFile.close();
  return 0;
}


#if 1
/**
 * @param tableName Filepath of the file.
 * @param row Row.
 * @param column  Column.
 * @param outBuffer Buffer to write to.
 * @param outBufferLen  Length of the buffer. Will be set to how many chars were read.
 * @returns Error (0|1|2|3) - (ok|file not found|cell not found|read byte error).
 * @brief Read a csv file by cells. End file with an empty line.
 */
uint8_t readCell(char* tableName, uint16_t row, uint16_t column, uint8_t* outBuffer, uint16_t* outBufferLen) {
  if(checkFile(tableName) != 0) {  // Check if the file exists and is a file.
    return 1;
  }

  File    tableFile = SD.open(tableName, FILE_READ);  // Open the file.
  uint8_t fileSize  = tableFile.size();               // Size of the file in bytes.

  #if 0
  uint16_t rowI          = 0;  // Row counter.
  uint16_t columnI       = 0;  // Column counter.
  uint8_t  lastCharWasCR = 0;  // Windows CR LF fix.
  uint8_t  posReached    = 0;  // Flag if the position exists.
  for(uint16_t fileBytePos = 0; fileBytePos < fileSize; fileBytePos++) {
    if(rowI == row && columnI == column) {  // Check if pos reached.
      posReached = 1;                       // Set flag and break.
      break;
    }

    if(rowI > row || columnI > column) {  // Check if the cell was missed. It doesn't exist
      break;                              // Don't set the flag.
    }

    if(lastCharWasCR > 0) {  // Decrement the "was CR" counter.
      lastCharWasCR--;
    }

    uint8_t readByte[1];
    tableFile.read(readByte, 1);  // Read a byte.
    switch(readByte[0]) {         // Check for a cell end.
      case '\r':                  // Carriage return.
        rowI++;                   // Advance row counter.
        columnI       = 0;        // Reset column counter.
        lastCharWasCR = 2;        // Was CR.
        break;

      case '\n':                  // Line feed
        if(lastCharWasCR == 0) {  // Fix for windows encoding. Ignore LF if it's after CR.
          rowI++;                 // Advance row counter.
          columnI = 0;            // Reset column counter.
        }
        break;

      case ',':  // End of a cell.
        //THIS
        columnI++;  // Increase column counter.
        break;

      case '"':  // "Escape char".
        //THIS
        break;
    }
  }
  #endif

  uint8_t lastCharWasCR[1] = {0};  // Windows CR LF fix.
  uint8_t posReached[1]    = {0};  // Flag if the position exists.
  cellStartPos(tableFile, fileSize, row, column, lastCharWasCR, posReached, 0, 0, 0);

  fileSize = tableFile.size();  // Size of the file in bytes.

  if(posReached[0] != 1) {  // Flag not set, return. The cell was not found.
    log_e("Cell doesn't exist.");
    tableFile.close();
    return 2;
  }

  for(uint16_t i = 0; i < outBufferLen[0]; i++) {  // Go through the cell. Length limited to buffer length.
    uint8_t readByte[1];
    if(tableFile.read(readByte, 1) == -1) {  // Try to read a byte.
      log_e("Read byte error");
      tableFile.close();
      return 3;
    }

    if(readByte[0] == ',' || readByte[0] == '\r' || readByte[0] == '\n') {  // End of cell.
      if(lastCharWasCR[0] == 1 && readByte[0] == '\n') {                    // Ignore a LF it the previous char was CR.
        lastCharWasCR[0] = 0;                                               // Reset flag.
      } else {
        outBufferLen[0] = i;  // Reached end, write length of the cell.
        break;
      }
    } else {
      outBuffer[i] = readByte[0];  // Copy char from cell to out.
    }
  }

  tableFile.close();  // Search ended, close the file.
  return 0;
}
#endif

#if 0
void setup() {
  Serial.begin(115200);
  delay(2000);

  sdSetup(PIN_SPI_SD_CS);

  // File.
  String  filenameString = "/temp.csv";
  uint8_t filenameLen    = filenameString.length() + 1; // Include the \0.
  char    filename[filenameLen];
  filenameString.toCharArray(filename, filenameLen);

  String  val1String = "b2";
  uint8_t val1Len    = val1String.length() + 1; // Include the \0. It will be removed quite soon.
  char    val1[val1Len];
  val1String.toCharArray(val1, val1Len);

  String  val2String = "mrrrp mrrow";
  uint8_t val2Len    = val2String.length() + 1; // Include the \0. It will be removed quite soon.
  char    val2[val2Len];
  val2String.toCharArray(val2, val2Len);

  String  val3String = "nyew cell :3";
  uint8_t val3Len    = val3String.length() + 1; // Include the \0. It will be removed quite soon.
  char    val3[val3Len];
  val3String.toCharArray(val3, val3Len);

  uint16_t bufferReadLen[1] = {32};  // Create a buffer to write to.
  uint8_t  bufferRead[bufferReadLen[0]];
  memset(bufferRead, 0, bufferReadLen[0]);


  writeCell(filename, filenameLen, 0, 1, val1, val1Len, 0); // Write a shorter val.

  writeCell(filename, filenameLen, 0, 2, val2, val2Len, 0); // Write a longer val.

  writeCell(filename, filenameLen, 1, 3, val3, val3Len, 1); // Write a new val.

  readCell(filename, 1, 2, bufferRead, bufferReadLen);  // Read a cell.

  Serial.println("Cell value START");  // Print the cell out.
  for(uint16_t i = 0; i < bufferReadLen[0]; i++) {
    Serial.print((char)bufferRead[i]);
  }
  Serial.print("\n");
  Serial.println("Cell value END");


  return;
}

void loop() {
  delay(100);
}
#endif
