//importing all the necessary libraries, the ESP8266 MCU node wifi support library, the software serial, and the firebase implementation libraries to establish the connection between the ESP8266 and the Firebase database
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <FirebaseArduino.h>
#include <FirebaseHttpClient.h>

//set up firebase connection variables with link of the firebase databse  for FIREBASE_HOST and secret key authorization for FIREBASE_AUTH
#define FIREBASE_HOST "miyah-application-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "zVPxt4bgSASoD6hp5Z4eaYCVM8H0Uo9EJeHIYzAY" //firebase database seceret key
//set up wifi connection variables
#define WIFI_SSID "note20mhmd" //wifi network name 
#define WIFI_PASSWORD "20406080" //wifi password


//define pins for the ultrasonic sensor (trig and echo pins), and the variables for the water level calculations and data
const int trigPin = D5;
const int echoPin = D6;
long duration;  
int distance;  


//variables for calculating hourly, weekly and monthly water consumption statistics 
int water_level_PD [4]; //24
int pd_temp = 0;
int pd_avg = 0;
int pd_sizeofArray = (sizeof(water_level_PD) / sizeof(water_level_PD[0]));

int water_level_PW [4]; //7
int pw_temp = 0;
int pw_avg = 0;
int pw_sizeofArray = (sizeof(water_level_PW) / sizeof(water_level_PW[0]));
int pw_check = 0;

int ph_temp = 0;
int water_level_PH [4]; //1800
int ph_avg = 0;

int ph_sizeofArray = (sizeof(water_level_PH) / sizeof(water_level_PH[0]));



void setup() { //setup method only runs code once

  //establish a wirelesss connection between the MCUNode ESP8266 to the wifi network

  pinMode(trigPin, OUTPUT); //define trigger pin as output
  pinMode(echoPin, INPUT); //define echo pin as input
    Serial.begin(9600);

  //source for setting up the firebase-arduino connection https://www.instructables.com/Firebase-Integrate-With-ESP8266/

  //establish the wifi and firebase connection
pinMode(16, OUTPUT); //pin 16 is used to wake up the ESP8266 from deep sleep https://user-images.githubusercontent.com/53692075/111992062-00578880-8b3b-11eb-9eab-59f927277be4.png // https://randomnerdtutorials.com/esp8266-deep-sleep-with-arduino-ide/#:~:text=GPIO%2016%20must%20be%20connected,pin%20receives%20a%20LOW%20signal.   
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
    }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  //begin the  firebase connection
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.setInt("users/TuczpVT8m2O1PgPPnmMtM3YoWvs1/espDistance", 0);
  
}//end of setup method


//ultrasonic distance measurement logic
void loop() {//loop method is where the main code logic that's going to be repeated should be

//source for clearing trigPin and setting it HIGH http://electronoobs.com/eng_arduino_tut80.php


//loop to add the current level to the array
for (int i = 0; i < (sizeof(water_level_PH) / sizeof(water_level_PH[0])); i++){

//clear the trigPin
digitalWrite(trigPin, LOW);
delayMicroseconds(2);

//set the trigPin on HIGH state for 10µs (microseconds unit)
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);

//reads the echoPin and then returns the sound wave frequencies travel-time in microseconds 
duration = pulseIn(echoPin, HIGH);

//calculates the distance, source  https://www.thegeekpub.com/235425/arduino-ultrasonic-sensor-tutorial/  &&  https://simple-circuit.com/arduino-hc-sr04-sensor-distance-meter/
distance= duration*0.034/2;  

//prints the distance on the Ardunio IDE's serial monitor 
Serial.print("Distance: ");
Serial.println(distance);
Firebase.setInt("users/TuczpVT8m2O1PgPPnmMtM3YoWvs1/espDistance",distance);
    water_level_PH[i] = distance;
    Serial.print("Current index: ");
    Serial.print(i);
            Serial.print(", Index value: " );
       Serial.println(water_level_PH[i]);
       delay(2000); //water level distance delay

           }








    //to calculate the average
    
     
    
      for(int j = 0; j < (ph_sizeofArray); j++){
        Serial.print("ph_index ");
        Serial.print(j);
        Serial.print(" = ");
        Serial.println(water_level_PH [j]);
        

         ph_temp += water_level_PH [j];
         Serial.print("temp = ");
       Serial.println(ph_temp);
        }
        ph_avg  = ph_temp/(ph_sizeofArray);
        Serial.print("Average: ");
       Serial.println(ph_avg);
       Firebase.setInt("users/TuczpVT8m2O1PgPPnmMtM3YoWvs1/avgPH",ph_avg);
       ph_temp = 0;

       for(int i = 0; i < (pd_sizeofArray) ; i++){
        if (water_level_PD[i] == 0){
//          Serial.print(" number: ");
//          Serial.println(i);
          water_level_PD[i] = ph_avg;
          Serial.print("Index number: ");
          Serial.print(i);
          Serial.print("  Value: ");
       Serial.println(water_level_PD [i]);
          break;          }

        }
        if(water_level_PD[3] > 0){
          Serial.print(water_level_PD[3]);
                for(int j = 0; j < (pd_sizeofArray); j++){
        Serial.print("index ");
        Serial.print(j);
        Serial.print(" =");
        Serial.println(water_level_PD [j]);
        
         pd_temp += water_level_PD [j];
         Serial.print("pd_temp = ");
       Serial.println(pd_temp);
       pd_avg = pd_temp /  (pd_sizeofArray);
       pw_check =1;

      

       
        }
      Serial.print("Day avg= ");
       Serial.println(pd_avg);
       Firebase.setInt("users/TuczpVT8m2O1PgPPnmMtM3YoWvs1/avgPD",pd_avg);
       pd_temp = 0;
   
        for(int j = 0; j < (pd_sizeofArray); j++){
         water_level_PD [j] = 0;
          }
          }
          if(pw_check == 1){
           for(int i = 0; i < (pw_sizeofArray) ; i++){
        if (water_level_PW[i] == 0){
//          Serial.print(" number: ");
//          Serial.println(i);
          water_level_PW[i] = pd_avg;
          Serial.print("PW Index number: ");
          Serial.print(i);
          Serial.print("  Value: ");
       Serial.println(water_level_PW [i]);
       pw_check = 0;
          break;          }

        }
        if(water_level_PW[3] > 0){
        for(int j = 0; j < (pw_sizeofArray); j++){
        Serial.print("index ");
        Serial.print(j);
        Serial.print(" =");
        Serial.println(water_level_PW [j]);
        
         pw_temp += water_level_PW [j];
         Serial.print("pw_temp = ");
       Serial.println(pw_temp);
       pw_avg = pw_temp /  (pw_sizeofArray);

      

       
        }
      Serial.print("Week avg= ");
       Serial.println(pw_avg);
       Firebase.setInt("users/TuczpVT8m2O1PgPPnmMtM3YoWvs1/avgPW",pw_avg);
       pw_temp = 0;
        for(int j = 0; j < (pw_sizeofArray); j++){
         water_level_PW [j] = 0;
          }
          }

        }





           

}//end of loop method
