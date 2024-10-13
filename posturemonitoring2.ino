#include <Arduino.h>
#include <WiFi.h>
#include <FirebaseESP32.h>

// Provide the token generation process info.
#include <addons/TokenHelper.h>

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "HUAWEI-2.4G-q83J"
#define WIFI_PASSWORD "BjQq8Ex8"

// For the following credentials, see examples/Authentications/SignInAsUser/EmailPassword/EmailPassword.ino

/* 2. Define the API Key */
#define API_KEY "AIzaSyBN__ZIEuKehpGp36zE06LPdpx0G5zxZrE"

/* 3. Define the RTDB URL */
#define DATABASE_URL "https://posturemonitoring-6ab73-default-rtdb.asia-southeast1.firebasedatabase.app/" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "arvin.dlrys@gmail.com"
#define USER_PASSWORD "12345678"


// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int count = 0;
int x,y,z,p;


void setup()
{

  Serial.begin(9600);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

  // Comment or pass false value when WiFi reconnection will control by your code or third party library e.g. WiFiManager
  Firebase.reconnectNetwork(true);

  // Since v4.4.x, BearSSL engine was used, the SSL buffer need to be set.
  // Large data transmission may require larger RX buffer, otherwise connection issue or data read time out can be occurred.
  fbdo.setBSSLBufferSize(4096 /* Rx buffer size in bytes from 512 - 16384 */, 1024 /* Tx buffer size in bytes from 512 - 16384 */);

  //////////////////////////////////////////////////////////////////////////////////////////////
  // Please make sure the device free Heap is not lower than 80 k for ESP32 and 10 k for ESP8266,
  // otherwise the SSL connection will fail.
  //////////////////////////////////////////////////////////////////////////////////////////////

  Firebase.begin(&config, &auth);

  Firebase.setDoubleDigits(5);
  
}

void loop()
{
  x=random(100,360);
  y=random(100,360);
  z=random(100,360);
  p=random(90,180);
    
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 1000 || sendDataPrevMillis == 0)) 
  {
    sendDataPrevMillis = millis();
    //Firebase.setInt(fbdo, main, 5);
    Firebase.setInt(fbdo, "/posture/p", p);
    Firebase.setInt(fbdo, "/gyro/x", x);
    Firebase.setInt(fbdo, "/gyro/y", y);
    Firebase.setInt(fbdo, "/gyro/z", z);
    delay(200);

    Serial.printf("Get int a--  %s\n", Firebase.getInt(fbdo, "/posture/p") ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str());
     p=fbdo.to<int>();
    Serial.printf("Get int b--  %s\n", Firebase.getInt(fbdo, "/gyro/x") ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str());
     x=fbdo.to<int>();
     Serial.printf("Get int b--  %s\n", Firebase.getInt(fbdo, "/gyro/y") ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str());
     y=fbdo.to<int>();
     Serial.printf("Get int b--  %s\n", Firebase.getInt(fbdo, "/gyro/z") ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str());
     z=fbdo.to<int>();

  Serial.println();  
  Serial.print("posture: ");
  Serial.println(p);
  Serial.print("x: ");
  Serial.println(x);
  Serial.print("y: ");
  Serial.println(y);
  Serial.print("z: ");
  Serial.print(z);

  Serial.println();
  Serial.println("------------------");
  Serial.println();
  

  delay(2500);
  }
}
