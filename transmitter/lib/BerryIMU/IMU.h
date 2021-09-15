void detectIMU();
void enableIMU();
void readACC(unsigned char buff[]);
void readMAG(unsigned char buff[]);
void readGYR(unsigned char buff[]);

void writeTo(int device,  unsigned char address,  unsigned char val);
void readFrom(int device,  unsigned char address, int num,  unsigned char buff[]);

