#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
#include <EEPROM.h>
#include "GravityTDS.h"

#define TdsSensorPin A3

GravityTDS gravityTds;
int pHSense = A0;
int samples = 10;
float adc_resolution = 1024.0;
float temperature = 25,tdsValue = 0;

LiquidCrystal_I2C lcd(0x27, 16, 2);
void setup()
{
    gravityTds.setPin(TdsSensorPin);
    gravityTds.setAref(5.0);  //reference voltage on ADC, default 5.0V on Arduino UNO
    gravityTds.setAdcRange(1024);  //1024 for 10bit ADC;4096 for 12bit ADC
    gravityTds.begin();  //initialization
    
    Serial.begin(9600);
    lcd.begin();
    lcd.backlight();
    lcd.clear();
    delay(100);
    //lcd.println("cimpleo pH Sense");
}

float ph (float voltage) {
    return 7 + ((2.5 - voltage) / 0.18);
}

void loop()
{
    int measurings=0;
    
    float result;
  
    for (int i = 0; i < samples; i++)
    {
        measurings += analogRead(pHSense);
        Serial.println(analogRead(pHSense));
        delay(10);

    }
    //temperature = readTemperature();  //add your temperature sensor and read it
    gravityTds.setTemperature(temperature);  // set the temperature and execute temperature compensation
    gravityTds.update();  //sample and calculate 
    tdsValue = gravityTds.getTdsValue();  // then get the value
  

    lcd.setCursor(0,0);
    lcd.print("PH");
    lcd.setCursor(8,0);
    lcd.print("TD[ppm]");
    
    lcd.setCursor(0,1);
    float voltage = 5 / adc_resolution * measurings/samples;
    result=ph(voltage);
    lcd.print(result);
    
    lcd.setCursor(8,1);
    lcd.print(tdsValue);
    delay(3000);
}
