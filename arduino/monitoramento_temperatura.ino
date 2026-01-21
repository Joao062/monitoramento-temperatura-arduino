#include <IRremote.hpp>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// ====== Pinos ======
#define PIN_IR 2
#define PIN_LED 8
#define PIN_DHT 3
#define DHTTYPE DHT11

// ====== Objetos ======
LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(PIN_DHT, DHTTYPE);

// ====== Variáveis ======
float setpoint = 30.0;
float tempC = NAN, hum = NAN;

// Limites
const float SP_MIN = 10.0;
const float SP_MAX = 30.0;

// Timers
unsigned long tDHT = 0, tLCD = 0, tSerial = 0;
const unsigned long DT_DHT = 1000;
const unsigned long DT_LCD = 500;
const unsigned long DT_SERIAL = 2000;

// Mensagem inválida
unsigned long tMsgInvalidUntil = 0;

// ====== SOS Morse ======
const uint16_t U = 200;

const bool SOS_STATE[] = {
 1,0, 1,0, 1,0,
 1,0, 1,0, 1,0,
 1,0, 1,0, 1,0
};

const uint16_t SOS_DUR[] = {
 U,U, U,U, U,U*3,
 U*3,U, U*3,U, U*3,U*3,
 U,U, U,U, U,3000
};

const uint8_t SOS_LEN = sizeof(SOS_STATE)/sizeof(SOS_STATE[0]);
uint8_t sosIdx = 0;
unsigned long tSOSnext = 0;

void updateSOS(bool active) {
 unsigned long now = millis();

 if (!active) {
   digitalWrite(PIN_LED, LOW);
   sosIdx = 0;
   tSOSnext = now;
   return;
 }

 if (now >= tSOSnext) {
   digitalWrite(PIN_LED, SOS_STATE[sosIdx] ? HIGH : LOW);
   tSOSnext = now + SOS_DUR[sosIdx];
   sosIdx++;
   if (sosIdx >= SOS_LEN) sosIdx = 0;
 }
}

void setup() {
 Serial.begin(9600);
 pinMode(PIN_LED, OUTPUT);
 digitalWrite(PIN_LED, LOW);

 dht.begin();
 lcd.init();
 lcd.backlight();

 IrReceiver.begin(PIN_IR, ENABLE_LED_FEEDBACK);

 lcd.setCursor(0,0);
 lcd.print("SP inicial=30C");
}

void loop() {
 unsigned long now = millis();

 // ===== IR =====
 if (IrReceiver.decode()) {
   unsigned long code = IrReceiver.decodedIRData.decodedRawData;

   if (code == 0xE718FF00) setpoint++;
   else if (code == 0xAD52FF00) setpoint--;
   else tMsgInvalidUntil = now + 1000;

   if (setpoint < SP_MIN) setpoint = SP_MIN;
   if (setpoint > SP_MAX) setpoint = SP_MAX;

   IrReceiver.resume();
 }

 // ===== Leitura DHT =====
 if (now - tDHT >= DT_DHT) {
   tDHT = now;
   tempC = dht.readTemperature();
   hum = dht.readHumidity();
 }

 bool alertaQuente = (!isnan(tempC) && tempC > setpoint);
 bool alertaFrio = (!isnan(tempC) && tempC < SP_MIN);

 updateSOS(alertaQuente);

 // ===== LCD =====
 if (now - tLCD >= DT_LCD) {
   tLCD = now;
   lcd.clear();

   lcd.setCursor(0,0);
   lcd.print("T:");
   isnan(tempC) ? lcd.print("--.-") : lcd.print(tempC,1);
   lcd.print((char)223); lcd.print("C ");
   lcd.print("U:");
   isnan(hum) ? lcd.print("--") : lcd.print((int)hum);
   lcd.print("%");

   lcd.setCursor(0,1);
   lcd.print("SP:");
   lcd.print(setpoint,1);
   lcd.print((char)223);
   lcd.print(" ");

   if (now < tMsgInvalidUntil) lcd.print("Inv");
   else if (alertaQuente) lcd.print("ALTA");
   else if (alertaFrio) lcd.print("BAIXA");
   else lcd.print("OK");
 }

 // ===== Envio Serial =====
 if (now - tSerial >= DT_SERIAL) {
   tSerial = now;
   if (!isnan(tempC) && !isnan(hum)) {
     Serial.print("Temperatura:");
     Serial.print(tempC,1);
     Serial.print(";Umidade:");
     Serial.print(hum,1);
     Serial.print(";SPmin:");
     Serial.print(SP_MIN,1);
     Serial.print(";SPmax:");
     Serial.println(setpoint,1);
   }
 }
}
