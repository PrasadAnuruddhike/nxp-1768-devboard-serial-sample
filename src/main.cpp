#include <mbed.h>

#define SERIAL_BAUDRATE             9600
#define SERIAL_BUFFER_LENGTH        8

static BufferedSerial serial_port(USBTX, USBRX, SERIAL_BAUDRATE);

int msg_receive_flag = 0;
uint8_t read_buf[SERIAL_BUFFER_LENGTH];

void callback_serial()
{
  msg_receive_flag = 1;
}

void zhark_serial_init()
{
  printf("Serial Initialization .. \n");
  serial_port.set_baud(SERIAL_BAUDRATE);
  serial_port.set_format(8, BufferedSerial::None, 1);
  // serial_port.sigio(callback(callback_serial));
  serial_port.sigio(callback_serial);
}

void read_data_packet()
{
  if(serial_port.readable())
  {
    ThisThread::sleep_for(30ms);
    memset(read_buf, '\0', SERIAL_BUFFER_LENGTH);

    serial_port.read(read_buf, SERIAL_BUFFER_LENGTH);

    char trash[1];
    while(serial_port.readable())
    {
        serial_port.read(&trash, 1);
    }

    printf("\n-------received data-----\n");
    for(int i = 0; i < SERIAL_BUFFER_LENGTH; i++)
    {
        printf("%2X ", read_buf[i]);
    }

    printf("\n-------------------------\n");
    msg_receive_flag = 0;
  }
}

void msg_check()
{
  if(msg_receive_flag == 1)
  {
    read_data_packet();
  }
}

void send_message()
{
  uint8_t send_data[SERIAL_BUFFER_LENGTH];
  send_data[0]    =   0x48;
  send_data[1]    =   0x45;
  send_data[2]    =   0x4C;
  send_data[3]    =   0x4C;
  send_data[4]    =   0x4F;
  send_data[5]    =   0x00;
  send_data[6]    =   0x0D;
  send_data[7]    =   0x0A;


  // ThisThread::sleep_for(5s);
  wait_us(2000000);
  serial_port.write(send_data, SERIAL_BUFFER_LENGTH);
}   

int main() {

  // put your setup code here, to run once:
  printf("Test Serial \n");
  zhark_serial_init();

  while(1) {
    // put your main code here, to run repeatedly:
    msg_check();
    send_message();
  }
}