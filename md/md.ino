#include <Wire.h>
#include "SSD1306Wire.h"
#include "ThingSpeak.h"
#include "WiFi.h"

//----------- Enter you Wi-Fi Details---------//
char ssid[] = "Gggrr"; //SSID
char pass[] = "Bheem8933"; // Password
//-------------------------------------------//

WiFiClient  Client;

SSD1306Wire display(0x3c, 21, 22); // SDA: 21, SCL: 22 for ESP32

const int hs=2;
const int buzz=25;
const int ledr=5;
const int ledg=4;



int data =0;  //stores input data of the hall sensor
int bagcount=0;


unsigned long myChannelField =  2421491; // Channel ID
const int ChannelField = 2; // Which channel to write data
const int ChannelField1=1;
const char * myWriteAPIKey = "BP040RBNFEN5X761"; // Your write API Key

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

   WiFi.mode(WIFI_STA);
  ThingSpeak.begin(Client);

  pinMode(hs,INPUT);
  pinMode(buzz,OUTPUT);
  pinMode(ledr,OUTPUT);
  pinMode(ledg,OUTPUT);


  display.init();
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_16);

  // Display initial bag count
  displayBagCount();
}

void loop() {
  //put your main code here, to run repeatedly:

  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    while (WiFi.status() != WL_CONNECTED)
    {
      WiFi.begin(ssid, pass);
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");
  }

 
  display.clear();  // clear the display

  
  Serial.print("Bags Count: ");
  Serial.println(bagcount);

   data=digitalRead(hs);
   Serial.println(data);
   
  if(data==LOW){
     digitalWrite(buzz,1);
     digitalWrite(ledg,0);
      digitalWrite(ledr,1);
     Serial.println("metal detected");
 
      bagcount++;
      displayBagCount();
     displayMetalDetected();
  }
  else{
    Serial.println("metal not detected");
    digitalWrite(buzz,0);
    digitalWrite(ledr,0);
    digitalWrite(ledg,1);
     clear();
    // displayBagCount();
   
  }
  ThingSpeak.writeField(myChannelField, ChannelField, data, myWriteAPIKey);
   ThingSpeak.writeField(myChannelField, ChannelField1, bagcount, myWriteAPIKey);
  
  delay(100);
}

void displayBagCount() {
 
  display.clear();

  display.drawString(128 / 2, 32 / 2, "Bags: " + String(bagcount));

  display.display();                                                      
}

void displayMetalDetected() {
 
  display.clear();

  // Display "Metal Detected" in the center of the OLED screen
  display.drawString(128 / 2, 32 / 2, "Metal Detected");

 
  display.display();
  delay(200); 
}


void clear() {

  display.clear();

  // Display "Metal Detected" in the center of the OLED screen
  display.drawString(128 / 2, 32 / 2, "CLEAR");

  
  display.display();
  delay(200);
}
