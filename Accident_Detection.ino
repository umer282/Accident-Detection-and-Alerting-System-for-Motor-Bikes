#include "EloquentTinyML.h"
// sine_model.h contains the array you exported from the previous step
// with either xxd or tinymlgen
#include "Acc_Detection.h"
#include "eloquent_tinyml/tensorflow.h"


#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;


#define N_INPUTS 6
#define N_OUTPUTS 3
// in future projects you may need to tweak this value.
// it's a trial and error process
#define TENSOR_ARENA_SIZE 2*1024

Eloquent::TinyML::TensorFlow::TensorFlow<N_INPUTS, N_OUTPUTS, TENSOR_ARENA_SIZE> tf;



unsigned long startMillis;  
unsigned long currentMillis;
const unsigned long period = 60000;


#include <WiFi.h>    
#include <HTTPClient.h>
#include <UrlEncode.h>

const char* ssid = "Redmi Note 9";
const char* password = "umer282irshad";

// +international_country_code + phone number
// Portugal +351, example: +351912345678
String phoneNumber = "+923324118633";
String apiKey = "4932864";

#include <HardwareSerial.h>
#include <TinyGPS++.h>

#define RXD2 16
#define TXD2 17
TinyGPSPlus gps;

double latitude;
double longitude;

String double2string(double n, int ndec) {
    String r = "";

    int v = n;
    r += v;     // whole number part
    r += '.';   // decimal point
    int i;
    for (i=0;i<ndec;i++) {
        // iterate through each decimal digit for 0..ndec 
        n -= v;
        n *= 10; 
        v = n;
        r += v;
    }

    return r;
}

void sendMessage(String message){

  // Data to send with HTTP POST
  String url = "https://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&apikey=" + apiKey + "&text=" + urlEncode(message);    
  HTTPClient http;
  http.begin(url);

  // Specify content-type header
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  
  // Send HTTP POST request
  int httpResponseCode = http.POST(url);
  if (httpResponseCode == 200){
    Serial.print("Message sent successfully");
  }
  else{
    Serial.println("Error sending the message");
    Serial.print("HTTP response code: ");
    Serial.println(httpResponseCode);
  }

  // Free resources
  http.end();
}



float X_acc[5] = {0,0,0,0,0};
float Y_acc[5] = {0,0,0,0,0};
float Z_acc[5] = {0,0,0,0,0};
float X_gyro[5] = {0,0,0,0,0};
float Y_gyro[5] = {0,0,0,0,0};
float Z_gyro[5] = {0,0,0,0,0};


float x_acc = 0;
float y_acc = 0;
float z_acc = 0;

float x_gyro = 0;
float y_gyro = 0;
float z_gyro = 0;

int maxIndex = 0;


void setup() {
    Serial.begin(115200);
    tf.begin(model_data);

    startMillis = 0;
   

  Serial1.begin(9600, SERIAL_8N1, RXD2, TXD2); //gps baud

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());




    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }
  
}

    

void loop() {
  maxIndex =0;


   
   bool recebido = false;
  while (Serial1.available()) {
     char cIn = Serial1.read();
     recebido = gps.encode(cIn);
  }
   if (gps.location.isUpdated() && gps.altitude.isUpdated())
  {
  Serial.print("LAT=");  Serial.println(gps.location.lat(), 6);
  Serial.print("LONG="); Serial.println(gps.location.lng(), 6);
  }
  latitude = gps.location.lat();
  longitude = gps.location.lng();

  currentMillis = millis();  
  if (currentMillis - startMillis >= period)  //test whether the period has elapsed
  {sendMessage("https://www.google.com/maps/search/?api=1&query="+double2string(latitude,6)+"%2C"+double2string(longitude,6));
  startMillis = currentMillis;
  }

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  X_acc[0] = a.acceleration.x;
  Y_acc[0] = a.acceleration.y;
  Z_acc[0] = a.acceleration.z;

  X_gyro[0] = abs((g.gyro.x)*0.017);
  Y_gyro[0] = abs((g.gyro.y)*0.017);
  Z_gyro[0] = abs((g.gyro.z)*0.017);
  Serial.println("");
  delay(500);

  for(int i = 1 ; i<5 ; i++){
    
    mpu.getEvent(&a, &g, &temp);
    X_acc[i] = abs(a.acceleration.x - X_acc[i-1]);
    Y_acc[i] = abs(a.acceleration.y - Y_acc[i-1]);
    Z_acc[i] = abs(a.acceleration.z - Z_acc[i-1]);
  
    X_gyro[i] = abs(g.gyro.x);
    Y_gyro[i] = abs(g.gyro.y);
    Z_gyro[i] = abs(g.gyro.z);

    delay(500);
      }

   x_acc = (X_acc[0]+X_acc[1]+X_acc[2]+X_acc[3]+X_acc[4])/5;
   y_acc = (Y_acc[0]+Y_acc[1]+Y_acc[2]+Y_acc[3]+Y_acc[4])/5;
   z_acc = (Z_acc[0]+Z_acc[1]+Z_acc[2]+Z_acc[3]+Z_acc[4])/5;

   x_gyro = (X_gyro[0],X_gyro[1],X_gyro[2],X_gyro[3],X_gyro[4])/5;
   y_gyro = (Y_gyro[0],Y_gyro[1],Y_gyro[2],Y_gyro[3],Y_gyro[4])/5;
   z_gyro = (Z_gyro[0],Z_gyro[1],Z_gyro[2],Z_gyro[3],Z_gyro[4])/5;

   
   Serial.println("x_acc");
   Serial.println(x_acc);
   Serial.println("y_acc");
   Serial.println(y_acc);
   Serial.println("z_acc");
   Serial.println(z_acc);
   Serial.println("x_gyro");
   Serial.println(x_gyro);
Serial.println("y_gyro");
   Serial.println(y_gyro);
Serial.println("z_gyro");
   Serial.println(z_gyro);
   Serial.println("---------------------------------");




  float input[N_INPUTS] = {x_acc,y_acc,z_acc,x_gyro,y_gyro,z_gyro};
  float output[N_OUTPUTS] = {0,0,0};

  tf.predict(input, output);

  
  Serial.println("Index Zero");
  Serial.println(output[0]);
  Serial.println("Index One");
  Serial.println(output[1]);
  Serial.println("Index Two");
  Serial.println(output[2]);
  Serial.println("---------------------");



  
  for(int i =1 ; i<3 ; i++){
    if(output[i] > output[maxIndex]){
      maxIndex = i;
      }
    }

  if(maxIndex == 0){
    Serial.println("NORMAL");
    Serial.println('-----------');}
  else
    if(maxIndex == 1){
      Serial.println("RASH");
      Serial.println('-----------');
      sendMessage("Rash Driving has been detected on the link below:");
        sendMessage("https://www.google.com/maps/search/?api=1&query="+double2string(latitude,6)+"%2C"+double2string(longitude,6));}
    else
      {
        Serial.println("Accident");
        Serial.println('-----------');
        sendMessage("Accident has been detected on the link below:");
        sendMessage("https://www.google.com/maps/search/?api=1&query="+double2string(latitude,6)+"%2C"+double2string(longitude,6));
        

        
        
        }

        
   

  
}
