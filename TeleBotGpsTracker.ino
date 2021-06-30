#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include "CTBot.h";
CTBot myBot ;
//Tethring config
String ssid  = "Nama WiFi Tethring";
String pass  = "Password WiFi Tethring";
//telegram config
String token = "Token bot Telegram";
const int id = xxxxxxxxxx ; //id User Telegram yang dapat mengakses bot
String peringatan1 = "Siapa anda? Jangan coba - coba masuk, atau anda akan berurusan dengan pihak kepolisian.";
String peringatan2 = "Kami telah mencatat ID Telegram dan Username anda, kami akan segera memproses ke jalur hukum jika anda memaksa masuk dalam bot ini.";
//gps config
float latitude , longitude;
int year , month , date, hour , minute , second;
String date_str , time_str , lat_str , lng_str;
int pm;
TinyGPSPlus gps;  
SoftwareSerial ss(4, 5);
//relay config
static const uint8_t relayMesin   = D5;
static const uint8_t relayAlarm   = D6;
//static const uint8_t relayLampu   = D7;
//static const uint8_t relayArduino = D8; 

void setup() {
  //pin convig
  pinMode(relayMesin, OUTPUT);
  pinMode(relayAlarm, OUTPUT);
  //pinMode(relayLampu, OUTPUT);
  //pinMode(relayArduino, OUTPUT);
  digitalWrite(relayMesin, HIGH);
  digitalWrite(relayAlarm, HIGH);
  //digitalWrite(relayLampu, HIGH);
  //digitalWrite(relayArduino, HIGH);
  //serial config
  Serial.begin(115200);
  ss.begin(9600);
  Serial.println("Starting System. Connecting to WiFi");

  //wifi config
  myBot.wifiConnect(ssid, pass);
    //telegram config
  myBot.setTelegramToken(token);
  if(myBot.testConnection())
    Serial.println("Connencting Succesfully. System Running");
  else
    Serial.println("Error");

}

void loop() {
  //GPS Data Perintah
  while (ss.available() > 0) //while data is available
    if (gps.encode(ss.read())) //read gps data
    {
      if (gps.location.isValid()) //check whether gps location is valid
      {
        latitude = gps.location.lat();
        lat_str = String(latitude , 6); // latitude location is stored in a string
        longitude = gps.location.lng();
        lng_str = String(longitude , 6); //longitude location is stored in a string
      }
      if (gps.date.isValid()) //check whether gps date is valid
      {
        date_str = "";
        date = gps.date.day();
        month = gps.date.month();
        year = gps.date.year();
        if (date < 10)
          date_str = '0';
        date_str += String(date);// values of date,month and year are stored in a string
        date_str += " / ";

        if (month < 10)
          date_str += '0';
        date_str += String(month); // values of date,month and year are stored in a string
        date_str += " / ";
        if (year < 10)
          date_str += '0';
        date_str += String(year); // values of date,month and year are stored in a string
      }
      if (gps.time.isValid())  //check whether gps time is valid
      {
        time_str = "";
        hour = gps.time.hour();
        minute = gps.time.minute();
        second = gps.time.second();
        minute = (minute + 30); // converting to IST
        if (minute > 59)
        {
          minute = minute - 60;
          hour = hour + 1;
        }
        hour = (hour + 5) ;
        if (hour > 23)
          hour = hour - 24;   // converting to IST
        if (hour >= 12)  // checking whether AM or PM
          pm = 1;
        else
          pm = 0;
        hour = hour % 12;
        if (hour < 10)
          time_str = '0';
        time_str += String(hour); //values of hour,minute and time are stored in a string
        time_str += " : ";
        if (minute < 10)
          time_str += '0';
        time_str += String(minute); //values of hour,minute and time are stored in a string
        time_str += " : ";
        if (second < 10)
          time_str += '0';
        time_str += String(second); //values of hour,minute and time are stored in a string
        if (pm == 1)
          time_str += " PM ";
        else
          time_str += " AM ";
      }
    }
    String link = "https://www.google.com/maps/place/"+lat_str+","+lng_str;
  //Telegram Data Perintah
  TBMessage msg ;
  static const int idp = msg.sender.id;
  String kodtlp = msg.sender.languageCode;
  if(myBot.getNewMessage(msg)){
  Serial.println("Pesan Masuk : " + msg.text);
  
  String  pesan = msg.text;
  if(pesan == "Motormati"){
    if(msg.sender.id == id){
      myBot.sendMessage(id, "Motor dipateni");
      digitalWrite(relayMesin, HIGH);
    }else{
      myBot.sendMessage(id, "Ada penyusup...!");
      myBot.sendMessage(id, "Username penyusup : " + msg.sender.username);
      myBot.sendMessage(id, "Nama penyusup : " + msg.sender.firstName + msg.sender.lastName);
      myBot.sendMessage(msg.sender.id, peringatan1);
      myBot.sendMessage(msg.sender.id, peringatan2);
    }
  }else if(pesan == "Motornyala"){
    if(msg.sender.id == id){
      myBot.sendMessage(id, "Motor urip");
      digitalWrite(relayMesin, LOW);
    }else{
      myBot.sendMessage(id, "Ada penyusup...!");
      myBot.sendMessage(id, "Username penyusup : " + msg.sender.username);
      myBot.sendMessage(id, "Nama penyusup : " + msg.sender.firstName + msg.sender.lastName);
      myBot.sendMessage(msg.sender.id, peringatan1);
      myBot.sendMessage(msg.sender.id, peringatan2);
    }
  }else if(pesan == "Posisi?"){
    if(msg.sender.id == id){
      myBot.sendMessage(id, "Posisi Motor ana ning : " + link);
    }else{
      myBot.sendMessage(id, "Ada penyusup");
    }
  }else if(pesan == "Alarmurip"){
    if(msg.sender.id == id){
      myBot.sendMessage(id, "Alarm Urip");
      digitalWrite(relayAlarm, LOW);
    }else{
      myBot.sendMessage(id, "Ada penyusup...!");
      myBot.sendMessage(id, "Username penyusup : " + msg.sender.username);
      myBot.sendMessage(id, "Nama penyusup : " + msg.sender.firstName + msg.sender.lastName);
      myBot.sendMessage(msg.sender.id, peringatan1);
      myBot.sendMessage(msg.sender.id, peringatan2);
    }
  }else if(pesan == "Alarmmati"){
    if(msg.sender.id == id){
      myBot.sendMessage(id, "Alarm Mati");
      digitalWrite(relayAlarm, HIGH);
    }else{
      myBot.sendMessage(id, "Ada penyusup...!");
      myBot.sendMessage(id, "Username penyusup : " + msg.sender.username);
      myBot.sendMessage(id, "Nama penyusup : " + msg.sender.firstName + msg.sender.lastName);
      myBot.sendMessage(msg.sender.id, peringatan1);
      myBot.sendMessage(msg.sender.id, peringatan2);
    }
  }else if(pesan == msg.text){
     myBot.sendMessage(id, "Ada penyusup...!!!" );
     myBot.sendMessage(id, "Username penyusup : " + msg.sender.username);
     myBot.sendMessage(id, "Nama penyusup : " + msg.sender.firstName + msg.sender.lastName);
     myBot.sendMessage(msg.sender.id, "Anda penyusup...!!!");
     myBot.sendMessage(msg.sender.id, peringatan1);
     myBot.sendMessage(msg.sender.id, peringatan2);
     myBot.sendMessage(msg.sender.id, "Username Anda adalah : " + msg.sender.username);
     myBot.sendMessage(msg.sender.id, "Nama Anda adalah : " + msg.sender.firstName + msg.sender.lastName);
  }
 }
 Serial.println("Latitude = "+lat_str + " longitude = "+lng_str);
 }
