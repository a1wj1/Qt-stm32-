/*
1 �����¶Ȳɼ�����
2 ��ȡ״̬�Ĵ������ж��¶Ȳɼ��Ƿ����
3 ��ȡ�¶ȸ�λ�ֽ�
4 ��ȡ�¶ȵ�λ�ֽ�
5 �¶ȸ�ʽת��
*/
float GetTemperature() 
{
float temperature;
// 1.��ʼ�����¶�
  if(Start_I2C())
	{
	}
  Wire.write(byte(0x80));     // sends instruction byte
  Wire.write(byte(0x90));     // Measure temperature
  Wire.endTransmission();     // stop transmitting

  // 2. read status
  do
  {
    Wire.beginTransmission(13); // transmit to device #13 (0x0D)   // 0x0D                     
    Wire.write(byte(0xB0));     // sends instruction byte  -> read
    Wire.write(byte(0x8F));     // status register address
    Wire.endTransmission();     // stop transmitting
    Wire.requestFrom(13, 1, true);    // request 1 bytes from slave device #13
    while(Wire.available())    // slave may send less than requested
      statusnow = Wire.read(); // receive a byte as character
  }
  while((statusnow | byte(0xFE)) != byte(0xFF));

  // 3. read temperature high address
  Wire.beginTransmission(13); // transmit to device #13 (0x0D)   // 0x0D                           
  Wire.write(byte(0xB0));     // sends instruction byte  -> read
  Wire.write(byte(0x92));     // temperature register high address
  Wire.endTransmission();     // stop transmitting
  Wire.requestFrom(13, 1, true);    // request 1 bytes from slave device #13
  while(Wire.available())    // slave may send less than requested
  {
    temphigh = Wire.read(); // receive a byte as character
  }
  // 4. read temperature low address
  Wire.beginTransmission(13); // transmit to device #13 (0x0D)   // 0x0D
  Wire.write(byte(0xB0));     // sends instruction byte  -> read
  Wire.write(byte(0x93));     // temperature register low address 1
  Wire.endTransmission();     // stop transmitting
  Wire.requestFrom(13, 1, true);    // request 1 bytes from slave device #13
  while(Wire.available())    // slave may send less than requested
  {
    templow  = Wire.read(); // receive a byte as character
  }
  // 5. trans temperature;
  temperature = (temphigh<< 8) | templow;
  IF(temphigh & 0x20 == 0x20)
    temperature = temperature - 16384;
  temperature = temperature /32 ;


}


