#include "AESLib.h"

AESLib aesLib;

void setup() {
    Serial.begin(9600);
    Serial.println("\nBooting...");  
  
  

  ///aesLib.decrypt(encMsg, key, my_iv);
}

void loop() {
  
  byte key[] = { 0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C };
    
  // put your setup code here, to run once:
  byte my_iv[N_BLOCK] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//  aesLib.gen_iv(my_iv);

  Serial.println("AES Key: ");
  int i;
  for (i = 0; i < sizeof(my_iv); i++)
  {
     Serial.print(my_iv[i], DEC); 
     Serial.print(",");
  }

  Serial.println("");

  String msg = "12.3";
  
  String encMsg = aesLib.encrypt(msg, key, my_iv);
  Serial.print("Encryption took: ");
  Serial.println("encMsg: " + encMsg);  
  delay(5000);
}
