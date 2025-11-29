#include <Arduino.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#include <ESP8266mDNS.h>




#define IR1 D5
#define IR2 D6
#define IR3 D7


// #define LED1 D1
// #define LED2 D2
// #define LED3 D8




//QR Format
//WIFI:S:MySSID;T:WPA;P:MyPassW0rd;; 
 
/* Put your SSID & Password */
// user name: admin
// password : admin
// const char* ssid = "parking4lcd";  // Enter SSID here
// const char* password_sta = "parking4lcd";  //Enter Password here

/* Put IP Address details */
// IPAddress local_ip(192,168,1,1);
// IPAddress gateway(192,168,1,1);
// IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);


#define EMPTY 0
#define BOOKED 1
#define PARKED 2

int slot1_status = EMPTY;
int slot2_status = EMPTY;
int slot3_status = EMPTY;
int slot4_status = EMPTY;


String slot1 = "BOOK NOW";
String slot2 = "BOOK NOW";
String slot3 = "BOOK NOW";

int login = 0;

unsigned long time_stamp = 0;
ESP8266WiFiMulti WiFiMulti;


void setup() {

  delay(500);
  Serial.begin(115200);

  Wire.begin();
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("  Smart ");
  lcd.setCursor(3,1);
  lcd.print(" Parking");




  //myservo.write(170);
  Serial.println();
  Serial.println();
  Serial.println();

 
  WiFi.mode(WIFI_STA);
  WiFi.begin("POCO F5", "1234abcd");


  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(WiFi.localIP());
  

  // WiFi.softAP(ssid, password_sta);
  // WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  server.on("/", handle_OnConnect);
  server.on("/act", userAcction);
  server.on("/account", userAccountPage);
  
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("\n\nServer started.");
  

  
  
  pinMode(IR1,INPUT);
  pinMode(IR2,INPUT);
  pinMode(IR3,INPUT);
  
  
  // pinMode(LED1,OUTPUT);
  // pinMode(LED2,OUTPUT);

  time_stamp = 1000+millis();

  delay(5000);
  
}


void handle_OnConnect() {
  int pas = 0;
  int usr = 0;
  for (uint8_t i = 0; i < server.args(); i++) {
    if(server.argName(i)== "usr"){
      if(server.arg(i)== "admin"){
        usr  =1;
      }
    }
    else if(server.argName(i)== "pas"){
      if(server.arg(i)== "admin"){
        pas  =1;
      }
    }
  }

  if(usr ==1 && pas ==1){
    login = 1;
    server.sendHeader("Location", String("/account"), true);
    server.send( 302, "text/plain", "");
  }
  else if(server.args() >= 1){
    server.send(200, "text/html", getLoginTemplate("Login Error!"));
  }
  else{
    server.send(200, "text/html", getLoginTemplate(""));
  }
}

void handle_NotFound(){
  server.sendHeader("Location", String("/"), true);
  server.send( 302, "text/plain", "");
}



void userAccountPage(){
  if( login == 0){
    server.sendHeader("Location", String("/"), true);
    server.send( 302, "text/plain", "");
  }
  server.send(200, "text/html", getTemplate());
}


void userAcction(){
  if( login == 0){
    server.sendHeader("Location", String("/"), true);
    server.send( 302, "text/plain", "");
  }
  for (uint8_t i = 0; i < server.args(); i++) {
    if(server.argName(i)== "S1"){
      if(server.arg(i)== "BOOK NOW"){
        slot1 = "BOOKED";
      }
    }
    if(server.argName(i)== "S2"){
      if(server.arg(i)== "BOOK NOW"){
        slot2 = "BOOKED";
      }
    }
    if(server.argName(i)== "S3"){
      if(server.arg(i)== "BOOK NOW"){
        slot3 = "BOOKED";
      }
    }
    
  }
  server.sendHeader("Location", String("/account"), true);
  server.send( 302, "text/plain", "");
  //server.send(200, "text/html", getTemplate());
}


unsigned long time_stamp_door = 0;
void loop() {
  server.handleClient();
  MDNS.update();

  //if(slot1 == "BOOKED"){
    if(digitalRead(IR1) == 0){
      delay(100);
      if(digitalRead(IR1) == 0){
        slot1 = "PARKED";
        delay(100);
      }
    }
  //}

  
  //if(slot2 == "BOOKED"){
    if(digitalRead(IR2) == 0){
      delay(100);
      if(digitalRead(IR2) == 0){
        slot2 = "PARKED";
        delay(100);
      }
    }
  //}
  
  //if(slot3 == "BOOKED"){
    if(digitalRead(IR3) == 0){
      delay(100);
      if(digitalRead(IR3) == 0){
        slot3 = "PARKED";
        delay(100);
      }
    }
  //}

  





  if(slot1 == "PARKED"){
    if(digitalRead(IR1) == 1){
      delay(100);
      if(digitalRead(IR1) == 1){
        slot1 = "BOOK NOW";
        delay(100);
      }
    }
  }
  if(slot2 == "PARKED"){
    if(digitalRead(IR2) == 1){
      delay(100);
      if(digitalRead(IR2) == 1){
        slot2 = "BOOK NOW";
        delay(100);
      }
    }
  }
  if(slot3 == "PARKED"){
    if(digitalRead(IR3) == 1){
      delay(100);
      if(digitalRead(IR3) == 1){
        slot3 = "BOOK NOW";
        delay(100);
      }
    }
  }
 

  // if(slot1 == "BOOKED"){
  //   digitalWrite(LED1,HIGH);
  // }
  // else{
  //   digitalWrite(LED1,LOW);
  // }
  
  // if(slot2 == "BOOKED"){
  //   digitalWrite(LED2,HIGH);
  // }
  // else{
  //   digitalWrite(LED2,LOW);
  // }
  
  // if(slot3 == "BOOKED"){
  //   //digitalWrite(LED3,HIGH);
  // }
  // else{
  //   //digitalWrite(LED3,LOW);
  // }

 
 

  if(time_stamp+1000 < millis()){
    
      time_stamp = millis();
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("S1:");
      Serial.print("Slot1:");
      Serial.print(slot1);
      Serial.println("]");

      Serial.print("Slot2:");
      Serial.print(slot2);
      Serial.println("]");

      Serial.print("Slot3:");
      Serial.print(slot3);
      Serial.println("]");

      Serial.print("index:");
      Serial.println(slot1.indexOf("BOOKED"));

      if(slot1.indexOf("BOOKED") != -1){
        lcd.print("B");
      }
      else if(slot1.indexOf("PARKED") != -1){
        lcd.print("P");
      }
      else if (slot1.indexOf("BOOK NOW") != -1){
        lcd.print("E");
      }


      lcd.setCursor(7,0);
      lcd.print("S2:");
      if(slot2.indexOf("BOOKED") != -1){
        lcd.print("B");
      }
      else if(slot2.indexOf("PARKED") != -1){
        lcd.print("P");
      }
      else if (slot2.indexOf("BOOK NOW") != -1){
        lcd.print("E");
      }

      lcd.setCursor(0,1);
      lcd.print("S3:");
      if(slot3.indexOf("BOOKED") != -1){
        lcd.print("B");
      }
      else if(slot3.indexOf("PARKED") != -1){
        lcd.print("P");
      }
      else if (slot3.indexOf("BOOK NOW") != -1){
        lcd.print("E");
      }



     
  }

 
      
}





String getTemplate(){

 String color1    = "";
 String color2    = "";
 String color3    = "";
 String color4    = "";


 
 
  if(slot1 == "BOOK NOW"){
    color1 = "alertsafe";
  }
  else if(slot1 == "BOOKED"){
    color1 = "alertwarning";
  }
  else if(slot1 == "PARKED"){
    color1 = "alertdanger";
  }

  if(slot2 == "BOOK NOW"){
    color2 = "alertsafe";
  }
  else if(slot2 == "BOOKED"){
    color2 = "alertwarning";
  }
  else if(slot2 == "PARKED"){
    color2 = "alertdanger";
  }


  if(slot3 == "BOOK NOW"){
    color3 = "alertsafe";
  }
  else if(slot3 == "BOOKED"){
    color3 = "alertwarning";
  }
  else if(slot3 == "PARKED"){
    color3 = "alertdanger";
  }

 
  
  String temp  = "<!DOCTYPE html>"
  "<html>"
  "<head>"
  "<title>IOT Parking</title>"
  "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
  "<meta http-equiv=\"refresh\" content=\"2\">"
  "<style>"
  ".login-page {"
  "  width: 320px;"
  "  padding: 8% 0 0;"
  "  margin: auto;"
  "}"
  ".form {"
  "  position: relative;"
  "  z-index: 1;"
  "  background: #FFFFFF;"
  "  max-width: 360px;"
  "  margin: 0 auto 100px;"
  "  padding: 45px;"
  "  text-align: center;"
  "  box-shadow: 0 0 20px 0 rgba(0, 0, 0, 0.2), 0 5px 5px 0 rgba(0, 0, 0, 0.24);"
  "}"
  ".form input {"
  "  font-family: \"Roboto\", sans-serif;"
  "  outline: 0;"
  "  background: #f2f2f2;"
  "  width: 100%;"
  "  border: 0;"
  "  margin: 0 0 15px;"
  "  padding: 15px;"
  "  box-sizing: border-box;"
  "  font-size: 14px;"
  "}"
  ".form button {"
  "  font-family: \"Roboto\", sans-serif;"
  "  text-transform: uppercase;"
  "  outline: 0;"
  "  width: 100%;"
  "  border: 0;"
  "  padding: 15px;"
  "  color: #FFFFFF;"
  "  font-size: 14px;"
  "  -webkit-transition: all 0.3 ease;"
  "  transition: all 0.3 ease;"
  "  cursor: pointer;"
  "}"
  ".alertsafe {"
  "  background: #43A047;"
  "}"
  ".alertdanger {"
  "  background: #E04347;"
  "}"
  ".alertwarning {"
  "  background: #42a5f5;"
  "}"
  ".container {"
  "  position: relative;"
  "  z-index: 1;"
  "  max-width: 300px;"
  "  margin: 0 auto;"
  "}"
  ".container:before, .container:after {"
  "  content: \"\";"
  "  display: block;"
  "  clear: both;"
  "}"
  ".container .info {"
  "  margin: 50px auto;"
  "  text-align: center;"
  "}"
  ".container .info h1 {"
  "  margin: 0 0 15px;"
  "  padding: 0;"
  "  font-size: 36px;"
  "  font-weight: 300;"
  "  color: #1a1a1a;"
  "}"
  ".map_img{"
  "  display:flex;"
  "  justify-content:center;"
  "  max-width: 320px;"
  "  height:auto;"
  "  margin-bottom:20px;"
  "}"
  ".container .info span {"
  "  color: #4d4d4d;"
  "  font-size: 12px;"
  "}"
  ".container .info span a {"
  "  color: #000000;"
  "  text-decoration: none;"
  "}"
  ".container .info span .fa {"
  "  color: #EF3B3A;"
  "}"
  "body {"
  "  background: #76b852; /* fallback for old browsers */"
  "  background: -webkit-linear-gradient(right, #76b852, #8DC26F);"
  "  background: -moz-linear-gradient(right, #76b852, #8DC26F);"
  "  background: -o-linear-gradient(right, #76b852, #8DC26F);"
  "  background: linear-gradient(to left, #76b852, #8DC26F);"
  "  font-family: \"Roboto\", sans-serif;"
  "  -webkit-font-smoothing: antialiased;"
  "  -moz-osx-font-smoothing: grayscale;      "
  "}"
  "</style>"
  "</head>"
  ""
  "<div class=\"login-page\">"
  "<img class=\"map_img\" src=\"https://i.ibb.co/PhGj2ts/IMG-6076.jpg\">"
  "  <div class=\"form\">"
  "    <form class=\"login-form\" method=\"get\" action=\"/act\" >"
  "\t\t<h5>Slot 1:</h5>"
  "\t\t<button  type=\"submit\" name=\"S1\" value=\""+String(slot1)+"\" class=\""+String(color1)+"\">"+String(slot1)+"</button></br>"
  "\t\t<h5>Slot 2:</h5>" 
  "\t\t<button  type=\"submit\" name=\"S2\" value=\""+String(slot2)+"\" class=\""+String(color2)+"\">"+String(slot2)+"</button></br>" 
  "\t\t<h5>Slot 3:</h5>"
  "\t\t<button  type=\"submit\" name=\"S3\" value=\""+String(slot3)+"\" class=\""+String(color3)+"\">"+String(slot3)+"</button></br>"
  "    </form>"
  "  </div>"
  "</div>"
  "</body>"
  "</html>";
  return temp;
}




String getLoginTemplate(String mesg){


  
  String temp  = "<!DOCTYPE html>"
  "<html>"
  "<head>"
  "<title>IOT Parking</title>"
  "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
  "<style>"
  ".login-page {"
  "  width: 320px;"
  "  padding: 8% 0 0;"
  "  margin: auto;"
  "}"
  ".form {"
  "  position: relative;"
  "  z-index: 1;"
  "  background: #FFFFFF;"
  "  max-width: 360px;"
  "  margin: 0 auto 100px;"
  "  padding: 45px;"
  "  text-align: center;"
  "  box-shadow: 0 0 20px 0 rgba(0, 0, 0, 0.2), 0 5px 5px 0 rgba(0, 0, 0, 0.24);"
  "}"
  ".form input {"
  "  font-family: \"Roboto\", sans-serif;"
  "  outline: 0;"
  "  background: #f2f2f2;"
  "  width: 100%;"
  "  border: 0;"
  "  margin: 0 0 15px;"
  "  padding: 15px;"
  "  box-sizing: border-box;"
  "  font-size: 14px;"
  "}"
  ".form button {"
  "  font-family: \"Roboto\", sans-serif;"
  "  text-transform: uppercase;"
  "  outline: 0;"
  "  width: 100%;"
  "  border: 0;"
  "  padding: 15px;"
  "  color: #FFFFFF;"
  "  font-size: 14px;"
  "  -webkit-transition: all 0.3 ease;"
  "  transition: all 0.3 ease;"
  "  cursor: pointer;"
  "}"
  ".alertsafe {"
  "  background: #43A047;"
  "}"
  ".alertwarning {"
  "  background: #E04347;"
  "}"
  ".container {"
  "  position: relative;"
  "  z-index: 1;"
  "  max-width: 300px;"
  "  margin: 0 auto;"
  "}"
  ".container:before, .container:after {"
  "  content: \"\";"
  "  display: block;"
  "  clear: both;"
  "}"
  ".container .info {"
  "  margin: 50px auto;"
  "  text-align: center;"
  "}"
  ".container .info h1 {"
  "  margin: 0 0 15px;"
  "  padding: 0;"
  "  font-size: 36px;"
  "  font-weight: 300;"
  "  color: #1a1a1a;"
  "}"
  ".container .info span {"
  "  color: #4d4d4d;"
  "  font-size: 12px;"
  "}"
  ".container .info span a {"
  "  color: #000000;"
  "  text-decoration: none;"
  "}"
  ".container .info span .fa {"
  "  color: #EF3B3A;"
  "}"
  "body {"
  "  background: #76b852; /* fallback for old browsers */"
  "  background: -webkit-linear-gradient(right, #76b852, #8DC26F);"
  "  background: -moz-linear-gradient(right, #76b852, #8DC26F);"
  "  background: -o-linear-gradient(right, #76b852, #8DC26F);"
  "  background: linear-gradient(to left, #76b852, #8DC26F);"
  "  font-family: \"Roboto\", sans-serif;"
  "  -webkit-font-smoothing: antialiased;"
  "  -moz-osx-font-smoothing: grayscale;      "
  "}"
  "</style>"
  "</head>"
  ""
  "<div class=\"login-page\">"
  "  <div class=\"form\" action=\"/\" >"
  "    <form class=\"login-form\" >"
  "\t\t<h4>"+String(mesg)+"</h4>"
  "\t\t<h5>User Name</h5>"
  "\t\t<input name=\"usr\"  type=\"text\"/>"
  "\t\t<h5>Password</h5>"
  "\t\t<input name=\"pas\" type=\"password\"/>"
  "\t\t<button class=\"alertsafe\">Login</button>" 
  "    </form>"
  "  </div>"
  "</div>"
  "</body>"
  "</html>";
  return temp;
}
