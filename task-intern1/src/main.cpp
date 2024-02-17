#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHTesp.h>
#include <ESP32Servo.h>
#include <RTClib.h>
#include <Wire.h>
#include <ezButton.h>
#include "pitches.h"

int countdown;
int i = 0;
int degree;

/* OLED Display */
#define SCREEN_WIDTH 128                                        // OLED width,  in pixels
#define SCREEN_HEIGHT 64                                        // 0OLED height, in pixels
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);  // create an OLED display object connected to I2C

/* Servo */
const int servoPin = 4;  // Servo Pin
Servo servo;             // Servo Object
int pos = 0;             // Servo Position

/* Push Button */
#define GREEN_BUTTON_PIN 19
#define RED_BUTTON_PIN 18
//#define BLUE_BUTTON_PIN 5

int green_button_state;
int red_button_state;
int blue_button_state;

/* LED */
#define GREEN_LED_PIN 16
#define RED_LED_PIN 17


int green_led_state = LOW;
int red_led_state = LOW;
int blue_led_state = LOW; 

// Navigasi halaman dan daftar
int halaman = 0;
int pointing = 0;

/* RTC Clock */
RTC_DS1307 rtc;
char namaHari[7][12] = {"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu"};

/* Speaker */
#define SPEAKER_PIN 2

int melody[] = {
  NOTE_CS4, NOTE_E4, NOTE_CS4, NOTE_CS4, NOTE_E4,
  NOTE_CS4, NOTE_CS4, NOTE_E4, NOTE_CS4, NOTE_DS4,
  NOTE_CS4, NOTE_CS4, NOTE_E4, NOTE_CS4,
  NOTE_B3,
  NOTE_CS4, NOTE_E4, NOTE_CS4, NOTE_CS4, NOTE_E4,
  NOTE_CS4, NOTE_DS4, NOTE_CS4, NOTE_E4,
  NOTE_B3,
  NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4,
  NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_FS4, NOTE_GS4, NOTE_GS4,
  NOTE_GS4, NOTE_E4, NOTE_FS4, NOTE_B4, NOTE_GS4, NOTE_GS4, NOTE_GS4, NOTE_GS4, NOTE_GS4,
  NOTE_FS4, NOTE_FS4, NOTE_FS4, NOTE_GS4, NOTE_FS4, NOTE_FS4, NOTE_FS4, NOTE_FS4, NOTE_FS4, NOTE_GS4, NOTE_FS4,
  NOTE_E4, NOTE_CS4, NOTE_CS4, NOTE_GS4, NOTE_GS4,
  NOTE_GS4, NOTE_GS4, NOTE_GS4, NOTE_GS4, NOTE_GS4, NOTE_GS4, NOTE_GS4, NOTE_GS4, NOTE_GS4,
  NOTE_GS4, NOTE_B4, NOTE_GS4, NOTE_GS4, NOTE_FS4, NOTE_FS4, NOTE_E4, NOTE_GS4,
  NOTE_FS4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_B4, NOTE_GS4, NOTE_GS4, NOTE_GS4, NOTE_FS4,
  NOTE_FS4, NOTE_FS4, NOTE_FS4, NOTE_FS4, NOTE_FS4, NOTE_FS4, NOTE_FS4, NOTE_FS4,
  NOTE_E4, NOTE_CS4, NOTE_CS4, NOTE_CS4, NOTE_CS4, NOTE_CS4,
  NOTE_GS3, NOTE_B3,
  NOTE_CS4, NOTE_CS4, NOTE_FS4, NOTE_GS4, NOTE_E4, NOTE_FS4,
  NOTE_B3,
  NOTE_E4, NOTE_FS4, NOTE_GS4, NOTE_FS4, NOTE_E4, NOTE_CS4, NOTE_E4, NOTE_GS4,
  NOTE_FS4, NOTE_E4, NOTE_FS4, NOTE_CS4, NOTE_B4, NOTE_GS4, NOTE_GS4, NOTE_FS4, NOTE_FS4,
  NOTE_E4, NOTE_CS4, NOTE_E4, NOTE_FS4, NOTE_GS4, NOTE_FS4, NOTE_E4,
  NOTE_FS4, NOTE_E4, NOTE_CS4, NOTE_CS4,
  NOTE_B3,
  NOTE_CS4, NOTE_CS4, NOTE_FS4, NOTE_GS4, NOTE_E4, NOTE_FS4, NOTE_FS4,
  NOTE_B3,
  NOTE_FS4, NOTE_GS4, NOTE_FS4, NOTE_E4, NOTE_CS4, NOTE_E4, NOTE_GS4, NOTE_FS4,
  NOTE_E4, NOTE_FS4, NOTE_CS4, NOTE_B4, NOTE_GS4, NOTE_GS4, NOTE_FS4, NOTE_FS4, NOTE_E4,
  NOTE_CS4, NOTE_B4, NOTE_GS4, NOTE_GS4, NOTE_FS4, NOTE_FS4, NOTE_E4,
  NOTE_CS4, NOTE_CS4,
  NOTE_GS3, NOTE_B3,
  NOTE_E4, NOTE_FS4, NOTE_GS4, NOTE_FS4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_FS4, NOTE_FS4,
  NOTE_E4, NOTE_FS4, NOTE_GS4, NOTE_FS4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_FS4|
  NOTE_CS4, NOTE_E4, NOTE_FS4, NOTE_GS4, NOTE_CS4,
  NOTE_E4, NOTE_E4, NOTE_FS4, NOTE_FS4, NOTE_E4, NOTE_FS4, NOTE_GS4,
  NOTE_FS4, NOTE_E4, NOTE_E4, NOTE_FS4, NOTE_CS4, NOTE_CS4,
  NOTE_FS4, NOTE_GS4, NOTE_B4, NOTE_GS4, NOTE_FS4, NOTE_E4, NOTE_E4, NOTE_FS4,
  NOTE_E4, NOTE_FS4, NOTE_GS4, NOTE_FS4, NOTE_E4, NOTE_E4, NOTE_FS4,
  NOTE_CS4, NOTE_CS4, NOTE_CS4, NOTE_CS4, NOTE_CS4, NOTE_E4, NOTE_FS4, NOTE_GS4, NOTE_CS4, NOTE_E4,
  NOTE_FS4, NOTE_E4, NOTE_FS4, NOTE_E4, NOTE_FS4, NOTE_GS4,
  NOTE_FS4, NOTE_E4, NOTE_E4, NOTE_FS4, NOTE_CS4, NOTE_CS4, NOTE_CS4, NOTE_E4, NOTE_E4,
  NOTE_FS4, NOTE_FS4, NOTE_GS4, NOTE_GS4,
  NOTE_E4, NOTE_FS4, NOTE_GS4, NOTE_FS4, NOTE_E4, NOTE_E4, NOTE_FS4, NOTE_CS4, NOTE_CS4,
  NOTE_CS4, NOTE_E4, NOTE_E4, NOTE_FS4, NOTE_FS4, NOTE_GS4,
  NOTE_GS4, NOTE_E4, NOTE_FS4, NOTE_GS4, NOTE_FS4,
  NOTE_E4, NOTE_E4, NOTE_FS4, NOTE_CS4, NOTE_CS4, NOTE_CS4, NOTE_E4, NOTE_E4,
  NOTE_FS4, NOTE_FS4, NOTE_GS4, NOTE_GS4,
  NOTE_E4, NOTE_FS4, NOTE_GS4, NOTE_FS4, NOTE_E4, NOTE_E4, NOTE_FS4, NOTE_CS4, NOTE_CS4,
  NOTE_CS4, NOTE_CS4, NOTE_CS4, NOTE_CS4, NOTE_E4, NOTE_FS4, NOTE_GS4, NOTE_CS4, NOTE_E4, NOTE_FS4,
  NOTE_CS4,
};

int durations[] = {
  2,2,4,2,2,4,2,2,4,2,2,2,2,2,2,2,2,4,2,2,4,2,2,2,2,8,8,4,4,8,8,8,8,8,8,8,2,8,8,4,2,8,8,4,8,8,8,4,4,8,8,8,8,8,4,8,4,8,8,8,8,4,2,8,8,8,8,4,8,8,8,8,8,8,2,8,4,8,8,8,8,8,2,4,8,8,2,4,8,8,8,8,8,8,2,8,8,4,4,4,8,8,2,4,4,2,4,2,2,4,8,8,8,2,2,8,4,8,4,4,4,4,8,8,8,8,2,8,8,4,8,8,4,2,8,4,8,4,4,4,4,2,2,2,2,4,8,8,8,2,2,2,4,8,4,4,4,4,8,8,8,8,2,8,8,4,8,8,4,2,8,8,4,8,8,4,2,2,4,2,8,8,4,8,8,4,8,8,2,8,8,4,8,8,4,8,8,2,8,8,4,4,4,8,8,2,8,8,4,8,8,8,4,8,2,4,4,8,8,4,4,8,2,8,8,4,4,4,8,8,4,8,8,8,8,8,8,4,4,4,4,8,2,8,8,4,8,8,8,4,8,4,4,4,4,4,4,4,2,8,8,4,8,8,8,4,8,4,4,4,4,4,4,4,2,8,8,4,8,8,8,4,8,4,4,4,4,4,4,4,2,8,8,4,8,8,8,4,8,4,8,8,8,8,8,8,4,4,4,4,2
};

void lagu(){
    oled.setTextSize(2);       // set text size
    oled.setTextColor(WHITE);  // set text color
    oled.setCursor(0, 2);      // set position to display (x,y)
    oled.clearDisplay();              // clear previous OLED
    oled.println("ENJOYYY");  // Display the string on OLED
    oled.display();     // display on OLED 
    int size = sizeof(durations) / sizeof(int);

  for (int note = 0; note < size; note++) {
    green_button_state = digitalRead(GREEN_BUTTON_PIN);
    if(green_button_state == LOW){
        break;
    }
    //to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int duration = 1000 / durations[note];
    tone(SPEAKER_PIN, melody[note], duration);

    //to distinguish the notes, set a minimum time between them.
    //the note's duration + 30% seems to work well:
    int pauseBetweenNotes = duration * 1.30;
    delay(pauseBetweenNotes);
    
    //stop the tone playing:
    noTone(SPEAKER_PIN);
  }
}

/* DHT Input (Suhu-Humidity) */
const int DHT_PIN = 15;
DHTesp dhtSensor;

/* LDR Input (Kecerahan) */
#define LDR_PIN 13
const float GAMMA = 0.7;
const float RL10 = 50;

const int SHORT_PRESS_TIME = 1000; // 1000 milliseconds
const int LONG_PRESS_TIME  = 1000; // 1000 milliseconds

ezButton button(5);  // create ezButton object that attach to pin 7;

unsigned long pressedTime  = 0;
unsigned long releasedTime = 0;
bool isPressing = false;
bool isLongDetected = false;

int menit=0;
int jam=0;


void button_press(){
    if(button.isPressed()){
    pressedTime = millis();
    isPressing = true;
    isLongDetected = false;
  }
  if(button.isReleased()) {
    isPressing = false;
    releasedTime = millis();

    long pressDuration = releasedTime - pressedTime;

    if( pressDuration < SHORT_PRESS_TIME ){
        if(halaman < 2 ) halaman++;
    }
  }
  if(isPressing == true && isLongDetected == false) {
    long pressDuration = millis() - pressedTime;

    if( pressDuration > LONG_PRESS_TIME ) {
      isLongDetected = true;
      if(halaman>0) halaman--;
    }
  }
}

void setting_alarm(){
    DateTime now = rtc.now();
    jam = jam % 24;
    menit = menit % 60;
    int realjam= now.hour();
    int realmenit= now.minute();
    oled.setTextSize(2);       // set text size
    oled.setTextColor(WHITE);  // set text color
    oled.setCursor(0, 2);      // set position to display (x,y)
    oled.clearDisplay();              // clear previous OLED
    String waktu = String(jam) +" : "+ String(menit);
    String realwaktu = "Now: " + String(realjam) + ":" + String(realmenit);
    oled.println(realwaktu);
    oled.println(waktu);
    oled.display();     // display on OLED 
}
void input_dht(){
    TempAndHumidity data = dhtSensor.getTempAndHumidity();
    String status="tidak terdata";
    int nowSuhu=data.temperature;
    int nowhumidity=data.humidity;
    if(nowSuhu<0){
        status="dingin";
    }else if(nowSuhu <40){
        status="netral";
    }else if(nowSuhu <80){
        status="hangat";
    }else{
        status="panas";
    }
    String temperatureString = String(data.temperature, 2) + "°C\n";
    String humidityString = String(data.humidity, 1) + "%\n";
    String gabung = temperatureString + " " + humidityString + " " + status;
    oled.setTextSize(2);       // set text size
    oled.setTextColor(WHITE);  // set text color
    oled.setCursor(0, 2);      // set position to display (x,y)
    oled.clearDisplay();              // clear previous OLED
    oled.println(gabung);  // Display the string on OLED
    oled.display();     // display on OLED 
    degree = float(nowSuhu+40)/float(165.0) * 180;
    servo.write(degree);
    if(nowSuhu < 43){
        digitalWrite(RED_LED_PIN, HIGH);
    }else{
        digitalWrite(GREEN_LED_PIN, HIGH);  
    }
    if(nowhumidity < 10 || nowhumidity > 90){
    tone(SPEAKER_PIN, 500);
    delay(500);
    noTone(SPEAKER_PIN);
    }
}
void input_ldr(){
    int analogValue = analogRead(LDR_PIN);
    float voltage = analogValue / 1024. * 5;
    float resistance = 2000 * voltage / (1 - voltage / 5);
    float lux = pow(RL10 * 1e3 * pow(10, GAMMA) / resistance, (1 / GAMMA));
    String kecerahan="Error coyyy";
    float skala=0;
    if(lux <= 1){
        kecerahan="Twilight";
        skala=1;
    }else if(lux<= 5){
        kecerahan="Minimal Street LIghting";
        skala=2;
    }else if(lux <= 10){
        skala=3;
        kecerahan="Sunset";
    }else if(lux <= 50){
        skala=4;
        kecerahan ="Familiy living room";
    }else if(lux <= 80){
        skala=5;
        kecerahan = "Hallway";
    }else if(lux <= 100){
        skala=6;
        kecerahan = "Very dark overcast day";
    }else if(lux <= 400){
        skala=7;
        kecerahan = "Office Lighting";
    }else if(lux <= 500){
        skala=8;
        kecerahan = "Sunrise";
    }else if(lux <= 1000){
        skala=9;
        kecerahan = "overcast day";
    }else if(lux <= 25000){
        skala=10;
        kecerahan = "Full daylight";
    }else if(lux<= 130000){
        skala=11;
        kecerahan = "direct Sun";
    }else{
        skala=12;
        kecerahan = " Cahaya Ilahi";
    }
    float degree_temp =float(skala/12.0)*180;
    degree=degree_temp;
    servo.write(degree);
    String ldrString = "LDR: " + String(lux, 2) + " lux\n " + kecerahan;
    oled.setTextSize(2);       // set text size
    oled.setTextColor(WHITE);  // set text color
    oled.setCursor(0, 2);      // set position to display (x,y)
    oled.clearDisplay();              // clear previous OLED
    oled.println(ldrString);  // Display the string on OLED
    oled.display();     // display on OLED 
    if(skala < 6){
        digitalWrite(RED_LED_PIN, HIGH);
    }else{
        digitalWrite(GREEN_LED_PIN, HIGH);  
    }
    if(skala <= 1 || skala >= 11){
    tone(SPEAKER_PIN, 500);
    delay(500);
    noTone(SPEAKER_PIN);
    }
}

void display(){
    
    DateTime now = rtc.now();
    int realjam=now.hour();
    int realmenit=now.minute();
    int realdetik=now.second();
    /* Example LDR */
    int analogValue = analogRead(LDR_PIN);
    float voltage = analogValue / 1024. * 5;
    float resistance = 2000 * voltage / (1 - voltage / 5);
    float lux = pow(RL10 * 1e3 * pow(10, GAMMA) / resistance, (1 / GAMMA));
    String ldrString = "LDR: " + String(lux, 2) + " lux\n";

    /* Example DHT Sensor */
    
    String servo = "Servo " + String(degree) + " derajat\n\n";

    /* Example RTC Clock */
    String waktu =  " " + String(now.day()) + '/' + String(now.month()) + '/' + String(now.year()) + ' ' + String(now.hour()) + ':' + String(now.minute()) + ':' + String(now.second()) + "\n\n";
    
    /* Example OLED */
    String oledDisplayString = servo + waktu  + ldrString;

    if((realjam==jam) && (realmenit == menit) && (realdetik == 0)){
    oled.setTextSize(2);       // set text size
    oled.setTextColor(WHITE);  // set text color
    oled.setCursor(0, 2);      // set position to display (x,y)
    oled.clearDisplay();              // clear previous OLED
    oled.println("ALARM!!!!");  // Display the string on OLED
    oled.display(); 
    tone(SPEAKER_PIN, 500);
    delay(10000);
    noTone(SPEAKER_PIN);
        // display on OLED 
    }
    else if(halaman == 0){
        

    oled.setTextSize(2);       // set text size
    oled.setTextColor(WHITE);  // set text color
    oled.setCursor(0, 2);      // set position to display (x,y)
    oled.clearDisplay();              // clear previous OLED
    oled.println(namaHari[now.dayOfTheWeek()] + waktu);  // Display the string on OLED
    oled.display();     // display on OLED 
    
    }else if(halaman == 1){
    digitalWrite(GREEN_LED_PIN, LOW);
    digitalWrite(RED_LED_PIN, LOW);  

    oled.setTextSize(2);       // set text size
    oled.setTextColor(WHITE);  // set text color
    oled.setCursor(0, 2);      // set position to display (x,y)
    oled.clearDisplay();              // clear previous OLED 
    if(pointing == 0){
        oled.setTextSize(2);  
        oled.println(">Input DHT");
        oled.println("Input LDR");
    }
    if(pointing == 1){
        oled.setTextSize(2);  
        oled.println(">Input LDR");
        oled.println("Setting Alarm");
    }
    if(pointing == 2){
        oled.setTextSize(2);  
        oled.println(">Setting Alarm");
        oled.println("Play Music");
    }
    if(pointing == 3){
        oled.setTextSize(2);  
        oled.println(">Play Music");
        oled.println("Input DHT");
    }
    oled.display();     // display on OLED 

}else if(halaman == 2){
    switch(pointing)
    {
        case 0:
        input_dht();
        break;
        case 1:
        input_ldr();
        break;
        case 2:
        setting_alarm();
        break;
        case 3:
        lagu();
        break;
    }
}
}

void setup() {
    Serial.begin(9600);  // Serial Monitor
    button.setDebounceTime(50);

    /* Setup OLED */
    if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {          // initialize OLED display with I2C address 0x3C
        Serial.println(F("failed to start SSD1306 OLED"));  // Handler
    }

    oled.clearDisplay();  // clear display

    oled.setTextSize(2);       // set text size
    oled.setTextColor(WHITE);  // set text color
    oled.setCursor(0, 2);      // set position to display (x,y)
    oled.println("SMART WATCH");   // set text
    oled.display();            // display on OLED
    delay(2000);
    /* Setup Servo */
    servo.attach(servoPin, 500, 2400);

    /* Setup Button */
    pinMode(GREEN_BUTTON_PIN, INPUT_PULLUP);  // set ESP32 pin to input pull-up mode
    pinMode(RED_BUTTON_PIN, INPUT_PULLUP);    // set ESP32 pin to input pull-up mode

    green_button_state = digitalRead(GREEN_BUTTON_PIN);
    red_button_state = digitalRead(RED_BUTTON_PIN);

    /* Setup LED */
    pinMode(GREEN_LED_PIN, OUTPUT);  // set ESP32 pin to output mode
    pinMode(RED_LED_PIN, OUTPUT);    // set ESP32 pin to output mode

    /* Setup RTC CLock */
    if (!rtc.begin()) {
        Serial.println("RTC TIDAK TERBACA");
    }
    if (!rtc.isrunning()) {
        Serial.println("RTC is NOT running!");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));  // update rtc dari waktu komputer
    }

    /* Setup Speaker */
    pinMode(SPEAKER_PIN, OUTPUT);

    /* Setup DHT Sensor */
    dhtSensor.setup(DHT_PIN, DHTesp::DHT22);

    /* Setup LDR */
    pinMode(LDR_PIN, INPUT);
}

void loop() {
    button.loop();
    DateTime now = rtc.now();

    /* Example control button */
    green_button_state = digitalRead(GREEN_BUTTON_PIN);  // read new state
    red_button_state = digitalRead(RED_BUTTON_PIN);      // read new state

    /* Example control LED and Speaker */
    if (green_button_state == LOW) {
        green_led_state = !green_led_state;
        // control LED arccoding to the toggled state
        if(halaman ==2 && pointing == 2){
            jam++;
        }else{
            pointing = (pointing + 4 - 1) % 4;
        }
    } else if (red_button_state == LOW) {
        red_led_state = !red_led_state;
          // control LED arccoding to the toggled state
        if(halaman == 2 && pointing == 2){
            menit++;
        }else{
            pointing = (pointing + 1) % 4;
        }

    }else {
        button_press();
    }
    display();
}