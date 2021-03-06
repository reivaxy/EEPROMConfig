/**
 *  Class to save to and restore from EEPROM a data structure containing a configuration 
 *  that needs to be persisted across resets and power off
 *  Xavier Grosjean 2017
 *  MIT License 
 *
 */
 
#include <Arduino.h>
#include "XEEPROMConfig.h"

XEEPROMConfigClass::XEEPROMConfigClass(unsigned int version, const char* type, unsigned int dataSize) {
  Debug("XEEPROMConfigClass::XEEPROMConfigClass %d, %s, %d\n", version, type, dataSize);
  _version = version;
  _dataSize = dataSize;
  _data = (unsigned char*)malloc(dataSize);
  // Copy type making sure there is no overflow
  strncpy(_type, type, TYPE_MAX_LENGTH);
  _type[TYPE_MAX_LENGTH] = 0;
}

/**
 * Initialize the data structure from the EEPROM content.
 * If the version is not the one expected (EEPROM content is obsolete, or was never initialized), initialize the data object
 * or the type is not the one expected (flashing a module code on a board that has another type of module code)
 * from the default values, then save it to EEPROM
 *
 */
void XEEPROMConfigClass::init(void) {
  Debug("\n\nXEEPROMConfigClass::init\n");
  initFromEeprom();
  if ((_version != getVersion()) || (0 != strcmp(_type, getType()))) {
    Serial.println("\n\nEEprom not up to date");
    initFromDefault();
    saveToEeprom();    
  } else {
    Serial.println("\n\nEEprom is up to date.");
  }
} 

/**
 * Read EEPROM unsigned char by unsigned char to initialize the data structure 
 *
 */
void XEEPROMConfigClass::initFromEeprom(void) {
  Debug("XEEPROMConfigClass::initFromEeprom\n");
  unsigned int sizeConfig = getDataSize();
  uint8_t* ptrConfig = (uint8_t *)_getDataPtr();
  EEPROM.begin(sizeConfig);
  for(unsigned int i = 0; i < sizeConfig; i++) {
    *(ptrConfig ++) = EEPROM.read(i);
  }
}

/**
 * Save the data structure to EEPROM unsigned char by unsigned char  
 *
 */
void XEEPROMConfigClass::saveToEeprom(void) {
  Debug("XEEPROMConfigClass::saveToEeprom\n");
  unsigned int sizeConfig = getDataSize();
  unsigned char* ptrConfig = (unsigned char *)_getDataPtr();
  for(unsigned int i = 0; i < sizeConfig; i++) {
    EEPROM.write(i, *ptrConfig++);
  }
  EEPROM.commit();
}

/**
 * Save data structure to EEPROM unsigned char by unsigned char up to given address  
 *
 */
void XEEPROMConfigClass::saveToEeprom(unsigned char* to) {
  Serial.println("XEEPROMConfigClass::saveToEeprom(to) is not implemented yet.");
// TODO : implement !!
//  Debug("XEEPROMConfigClass::saveToEeprom\n");
//  unsigned int sizeConfig = getDataSize();
//  unsigned char* ptrConfig = (unsigned char *)_getDataPtr();
//  for(unsigned int i = 0; i < sizeConfig; i++) {
//    EEPROM.write(i, *ptrConfig++);
//  }
//  EEPROM.commit();
}

/**
 * Save data structure to EEPROM unsigned char by unsigned char 
 * from given address up to given address  
 *
 */
void XEEPROMConfigClass::saveToEeprom(unsigned char* from, unsigned char* to) {
  Serial.println("XEEPROMConfigClass::saveToEeprom(from, to) is not implemented yet.");
// TODO : implement !!
//  int count;
//  Debug("XEEPROMConfigClass::saveToEeprom\n");
//  unsigned int sizeConfig = getDataSize();
//  unsigned char* ptrConfig = (unsigned char *)_getDataPtr();
//  count = min(sizeConfig, to - ptrConfig);
//  for(int i = 0; i < sizeConfig; i++) {
//    EEPROM.write(i, *ptrConfig++);
//  }
//  EEPROM.commit();
}

/**
 * Set the version in the data structure
 *
 */
void XEEPROMConfigClass::setVersion(unsigned int version) {
  Debug("XEEPROMConfigClass::setVersion\n");
  // Version is an unsigned int at the begining of the data structure
  *(unsigned int *)_getDataPtr() = version;
}

/**
 * Get version from the data structure
 *
 */
unsigned int XEEPROMConfigClass::getVersion(void) {
  // Version is an unsigned int at the begining of the data structure
  unsigned int* versionPtr = (unsigned int *)_getDataPtr();
  Debug("XEEPROMConfigClass::getVersion: %d\n", *versionPtr);
  return *versionPtr;
}

/**
 * Set the type in the data structure
 *
 */
void XEEPROMConfigClass::setType(const char* type) {
  Debug("XEEPROMConfigClass::setType\n");
  char* typePtr = (char*)_getDataPtr() + sizeof(_version);   // is there any padding ?
  strncpy(typePtr, type, TYPE_MAX_LENGTH);
  *(typePtr + TYPE_MAX_LENGTH) = 0;
}

/**
 * Get the type from the data structure
 *
 */
char* XEEPROMConfigClass::getType(void) {
  Debug("XEEPROMConfigClass::getType\n");
  char* typePtr = (char*)_getDataPtr() + sizeof(_version);
  return typePtr;
}

/**
 * Get the data structure size (it was provided at init)
 *
 */
unsigned int XEEPROMConfigClass::getDataSize(void) {
  Debug("XEEPROMConfigClass::getDataSize: %d\n", _dataSize);
  return _dataSize;
}

/**
 * Init the data structure from the default values.
 * Subclass need to take care of initializing fields other than version and type
 *
 */
void XEEPROMConfigClass::initFromDefault(void) {
  Debug("XEEPROMConfigClass::initFromDefault\n");
  setVersion(_version);
  setType(_type);
}

/**
 * Return the pointer to the data structure object
 *
 */
XEEPROMConfigDataStruct* XEEPROMConfigClass::_getDataPtr(void) {
  Debug("XEEPROMConfigClass::_getDataPtr\n");
  return (XEEPROMConfigDataStruct*)_data;
}
