// RANDOM NOTES:
// source: https://pulsegrow.com/blogs/learn/vpd#calculate
// About VPD - TL;DR
// saturation vapor pressure (svp) = max amount of water vapor air can hold at certain temp
// hotter = SVP increases || colder =  SVP descreases
// actual vapor pressure (avp) = amount of water vapor in air
// AVP / SVP x 100 = RH%
//The maximum the AVP can be is the current SVP.

// VPD = SVP – AVP || how much more room there is in the air for more water vapor

// step 1: figure out SVP >   SVP = 610.78 x e^(T / (T +238.3) x 17.2694))
// -- -- T = degrees C
// -- -- SVP = pascals (div by 1000 for kpa)
// -- -- e = Euler's Number = 2.71828
// step 2: calculate VPD >    VPD = SVP x (1 – RH/100)
// step 3: figure out Leaf SVP
// -- -- same as ASVP but typically 1-3ªC cooler (2-5ºF)
// step 4: Leaf VPD = LSVP – (ASVP x RH/100)


// ideal VPD values
// general rule for plant growth, is around 0.8 – 1.2 kPa (kilopascals)
// -- -- clones = around 0.8 kPa.
// -- -- veg = around 1.0 kPa
// -- -- flower = around 1.2kPa – 1.5kPa

//svp = 610.78 x pow(e, (temp_c / (temp_c +238.3) x 17.2694)))


// esp8266 temp / humid / VPD sensor with LEDs
#include <DHT.h>;
#define DHTPIN 5
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

float asvp;
float lsvp;
float lvpd;
float avpd;
float hum;
float temp;
float temp_c;
float leaf_temp;
float vpd_air;
float vpd_leaf;

float e = 2.71828;

const int red_led = 14; // D5
const int orange_led = 12; // D6
const int green_led = 13; // D7
const int blue_led = 15; // D8

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(red_led, OUTPUT);
  pinMode(orange_led, OUTPUT);
  pinMode(green_led, OUTPUT);
  pinMode(blue_led, OUTPUT);
}

void loop() {
  delay(2000);
  hum = dht.readHumidity();
  temp = dht.readTemperature(true);
  temp_c = dht.readTemperature();
  asvp = (610.78 * pow(e, (temp_c / (temp_c + 238.3) * 17.2694)))/1000;
  lsvp = (610.78 * pow(e, ((temp_c - 1) / ((temp_c - 1) + 238.3) * 17.2694)))/1000;
  avpd = asvp * (1 - (hum/100));
  lvpd = lsvp - (asvp * (hum/100));

  if (avpd >= .4 && avpd < .8 ) {
    // clone = blue
      digitalWrite(red_led, LOW);
      digitalWrite(orange_led, LOW);
      digitalWrite(green_led, LOW);
      digitalWrite(blue_led, HIGH);
  } else if (avpd >= .8 && avpd <= 1.2 ) {
    // veg = green
      digitalWrite(red_led, LOW);
      digitalWrite(orange_led, LOW);
      digitalWrite(green_led, HIGH);
      digitalWrite(blue_led, LOW);
  } else if (avpd >= 1.2 && avpd <= 1.6 ) {
    // flower = orange
      digitalWrite(red_led, LOW);
      digitalWrite(orange_led, HIGH);
      digitalWrite(green_led, LOW);
      digitalWrite(blue_led, LOW);
  } else {
    // danger = red
      digitalWrite(red_led, HIGH);
      digitalWrite(orange_led, LOW);
      digitalWrite(green_led, LOW);
      digitalWrite(blue_led, LOW);
  }

  Serial.print("Humidity: ");
  Serial.println(hum);
  Serial.print("Temp F: ");
  Serial.println(temp);
  Serial.print("Temp C: ");
  Serial.println(temp_c);
  Serial.print("asvp: ");
  Serial.println(asvp);
  Serial.print("lsvp: ");
  Serial.println(lsvp);
  Serial.print("avpd: ");
  Serial.println(avpd);
  Serial.print("lvpd: ");
  Serial.println(lvpd);
}
