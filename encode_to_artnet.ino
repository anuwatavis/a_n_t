#include <Ethernet.h>
#define outputA 8
#define outputB 9
int counter = 0;
int aState;
int aLastState;
byte destination_Ip[] = {192, 168, 1, 3 };    
unsigned int localPort = 6454;      
const int DMX_Universe = 0; 
const int number_of_channels = 1; 
byte mac[] = {  144, 162, 218, 00, 16, 96  };
byte ip[] = {   192, 168, 1, 116 }; 

char ArtNetHead[8] = "Art-Net";
const int art_net_header_size = 17;

short OpOutput = 0x5000 ; //output

byte buffer_dmx[number_of_channels]; //buffer used for DMX data

EthernetUDP Udp;

//Artnet PACKET
byte  ArtDmxBuffer[(art_net_header_size + number_of_channels) + 8 + 1];

//-----
const unsigned long eventInterval = 100 ;
unsigned long previousTime = 0;
//-----
void setup() {
  pinMode (outputA, INPUT_PULLUP);
  pinMode (outputB, INPUT_PULLUP);
  aLastState = digitalRead(outputA);
  //initialise artnet header
  construct_arnet_packet();
  Ethernet.begin(mac, ip);
  Udp.begin(localPort);
}

void loop() {

  check_arduino_inputs();
  construct_arnet_packet();
    unsigned long currentTime = millis();
  if (currentTime - previousTime >= eventInterval) {
    Udp.beginPacket(destination_Ip, localPort);
    Udp.write(ArtDmxBuffer, (art_net_header_size + number_of_channels + 1)); 
    Udp.endPacket();
    previousTime = currentTime;
  }

}

void check_arduino_inputs()
{
  aState = digitalRead(outputA);
  if (aState != aLastState) {
    if (digitalRead(outputB) != aState) {
      counter ++;
    } else {
      counter --;
    }
  }
  aLastState = aState;
  buffer_dmx[0] = byte(counter);

}


void construct_arnet_packet()
{
  //preparation pour tests
  for (int i = 0; i < 7; i++)
  {
    ArtDmxBuffer[i] = ArtNetHead[i];
  }
  //Operator code low byte first
  ArtDmxBuffer[8] = OpOutput;
  ArtDmxBuffer[9] = OpOutput >> 8;
  //protocole
  ArtDmxBuffer[10] = 0;
  ArtDmxBuffer[11] = 14;
  //sequence
  ArtDmxBuffer[12] = 0;
  //physical
  ArtDmxBuffer[13] = 0;
  // universe
  ArtDmxBuffer[14] = DMX_Universe; //or 0
  ArtDmxBuffer[15] = DMX_Universe >> 8;
  //data length
  ArtDmxBuffer[16] = number_of_channels >> 8;
  ArtDmxBuffer[17] = number_of_channels;
  for (int t = 0; t < number_of_channels; t++)
  {
    ArtDmxBuffer[t + art_net_header_size + 1] = buffer_dmx[t];
  }
}
