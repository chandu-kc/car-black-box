#ifndef EXT_EEPROM
#define EXT_EEPROM

#define SLAVE_READ		0xA1     //uniq id for eeprom,and access that
#define SLAVE_WRITE		0xA0





void write_ext_eeprom(unsigned char address1,  unsigned char data);
unsigned char read_ext_eeprom(unsigned char address1);

#endif