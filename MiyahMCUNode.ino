//importing all the necessary libraries, the ESP8266 MCU node wifi support library, the software serial, and the firebase implementation libraries
//to establish the connection between the ESP8266 and the Firebase database
#include <ESP8266WiFi.h> //to upload code on ESP8266 and use Wifi.Begin to connect the ESP8266 to wifi network
#include <SoftwareSerial.h> //to display serial for ESP8266
#include <FirebaseArduino.h> //to use .Firebase.setInt and Firebase.begin methods
#include <FirebaseHttpClient.h> //to connect to the most recent firebase website fingerprint

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

  pinMode(trigPin, OUTPUT); //define trigger pin as an output because it's transmitter 
  pinMode(echoPin, INPUT); //define echo pin as an input because it's reciever 
    Serial.begin(9600);

  //source for setting up the firebase-arduino connection https://www.instructables.com/Firebase-Integrate-With-ESP8266/

  //establish the wifi and firebase connection
pinMode(16, OUTPUT); //pin 16 is used to wake up the ESP8266 from deep sleep https://user-images.githubusercontent.com/53692075/111992062-00578880-8b3b-11eb-9eab-59f927277be4.png // https://randomnerdtutorials.com/esp8266-deep-sleep-with-arduino-ide/#:~:text=GPIO%2016%20must%20be%20connected,pin%20receives%20a%20LOW%20signal.   
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while(WiFi.status() != WL_CONNECTED) //refer to Arduino documentation https://www.arduino.cc/en/Reference/WiFiStatus
  {
    Serial.print(".");
    delay(500);
    }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  //begin the  firebase connection
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  //TuczpVT8m2O1PgPPnmMtM3YoWvs1 user ibrahim@ibrahim.com (blue green jump wires NodeMCU)
  //TEZLRyoqtwUuJHgRx4IhBWzFbC42 user ahmed@ahmed.com (red yellow jump wires NodeMCU)
  Firebase.setInt("users/T7R7dXGwH2SWIpnQbTNfRmi2PDh2/espDistance", 0);
  
}//end of setup method


//ultrasonic distance measurement logic
void loop() {//loop method is where the main code logic that's going to be repeated should be

//source for clearing trigPin and setting it HIGH http://electronoobs.com/eng_arduino_tut80.php


//loop to add the current level to the array
for (int i = 0; i < (sizeof(water_level_PH) / sizeof(water_level_PH[0])); i++){

//clear the trigPin
digitalWrite(trigPin, LOW); //set trig to low first to ensure it always starts as low
delayMicroseconds(2); //change any low number

//set the trigPin on HIGH state for 10µs (microseconds unit) send signle to trigger and start MCU through pulsing high and low 
digitalWrite(trigPin, HIGH);
delayMicroseconds(10); //this is a constant rule to delay 10 microseconds
digitalWrite(trigPin, LOW);

duration = pulseIn(echoPin, HIGH); //for how long echo remains as high and return microseconds duration

//calculates the distance, source  https://www.thegeekpub.com/235425/arduino-ultrasonic-sensor-tutorial/  &&  https://simple-circuit.com/arduino-hc-sr04-sensor-distance-meter/ && https://www.youtube.com/watch?v=pwWk20du8qA&ab_channel=ArafaMicrosys&loop=0
distance= duration*0.034/2;
//Divide by 2 to split the roundtrip distance between recieving and transmitting.
//340 based on averege speed of sound based on current humidity, air preassure temparature.
//Multiplty speed by 0.0001 to convert from meter/second centimeter/microsecond, result is 0.034

//prints the distance on the Ardunio IDE's serial monitor 
Serial.print("Distance: ");
Serial.println(distance);
Firebase.setInt("users/T7R7dXGwH2SWIpnQbTNfRmi2PDh2/espDistance",distance);











    //to calculate the average




    water_level_PH[i] = distance;
    Serial.print("Current index: ");
    Serial.print(i);
            Serial.print(", Index value: " );
       Serial.println(water_level_PH[i]);
       delay(2000); //water level distance delay

           }
    
     
    
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
       Firebase.setInt("users/T7R7dXGwH2SWIpnQbTNfRmi2PDh2/avgPH",ph_avg);
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
       Firebase.setInt("users/T7R7dXGwH2SWIpnQbTNfRmi2PDh2/avgPD",pd_avg);
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
       Firebase.setInt("users/T7R7dXGwH2SWIpnQbTNfRmi2PDh2/avgPW",pw_avg);
       pw_temp = 0;
        for(int j = 0; j < (pw_sizeofArray); j++){
         water_level_PW [j] = 0;
          }
          }

        }








           

}//end of loop method
