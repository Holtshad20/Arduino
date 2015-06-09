/*  
  char msg[17];
  int addr=0; //first address
  byte b=1; // access the first address from the memory
  char temp[sizeof(somedata)];
  unsigned long t_1;
  t_1=micros();
  while (b!=0) 
  {
    b = i2c_eeprom_read_byte(0x50, addr); //access an address from the memory
    temp[addr]=(char)b;
    addr++; //increase address
  }
  t_1=micros()-t_1;
  */
//  char somedata[] = "this is data fr"; // data to write
//  i2c_eeprom_write_page(0x50, 0, (byte *)somedata, sizeof(somedata)); // write to EEPROM 

