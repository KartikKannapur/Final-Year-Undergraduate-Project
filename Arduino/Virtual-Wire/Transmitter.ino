#include <VirtualWire.h> //Pin 12 on the arduino
void setup()
{
 vw_setup(2000); // Bits per sec
}
void loop()
{
 const char *msg = "hello";
 vw_send((uint8_t *)msg, strlen(msg));
 delay(400);
}
