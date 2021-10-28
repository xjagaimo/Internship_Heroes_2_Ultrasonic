// nama: Nathan Praja Kusuma
// sudut rotasi servo dan RPM Motor dipengaruhi oleh jarak benda

#include <Servo.h>      // library untuk servo

#define pinServo 9      // set servo ke pin 9
#define trigPinKiri 2   // set trigger kiri ke pin 2
#define echoPinKiri 3   // set echo kiri ke pin 3
#define trigPinKanan 7    // set trigger kanan ke pin 7
#define echoPinKanan 6    // set echo kanan ke pin 6
#define kiriLED 4     // set LED sensor kiri ke pin 0
#define kananLED 5      // set LED sensor kanan ke pin 5

#define pinMotorA 11    // set pin 11 sebagai pinMotorA PWM
#define pinMotorB 12    // set pin 12 sebagai pinMotorB digital

float left = 0, right = 0;  // declare variabel utk hasil read hc-sr04 kiri & kanan, nilai default awal = 0
int disMax = 300;     // set jarak maks untuk servo bereaksi
int halfMax = disMax/2;   // set setengah jarak maks

Servo kemudi;         // membuat objek servo 'kemudi'
int pos;            // variabel untuk menyimpan besaran sudut servo

void setup() {
  kemudi.attach(pinServo);    // set pin untuk servo
  pinMode(pinMotorA, OUTPUT); // set mode pinMotorA sbg output
  pinMode(pinMotorB, OUTPUT); // set mode pinMotorB sbg output
  pinMode(kiriLED, OUTPUT);   // set pin LED kiri sbg output
  pinMode(kananLED, OUTPUT);  // set pin LED kanan sbg output
  Serial.begin(9600);     
}

void loop() {
  left = readUltrasonicDistance(trigPinKiri, echoPinKiri) * 0.0344/2;   // set variabel left menampung hasil read sensor kiri dalam cm (jarak)
  right = readUltrasonicDistance(trigPinKanan, echoPinKanan) *0.0344/2;   // set variabel right menampung hasil read sensor kanan dalam cm (jarak)
  
  Serial.print("L = ");       // print nilai read sensor kiri
  Serial.println(left);
  Serial.print("R = ");       // print nilai read sensor kanan
  Serial.println(right);
  
  digitalWrite(pinMotorB, HIGH);  // set pinMotorB ke HIGH
  
  rotateServo(left, right);     // eksekusi function rotasi servo
}

long readUltrasonicDistance(int triggerPin, int echoPin){
  pinMode(triggerPin, OUTPUT);    // setting triggerpin
  digitalWrite(triggerPin, LOW);  // set triggerPin LOW sebelum dinyalakan
  delayMicroseconds(2);       // jeda 2 microsekon
  // trigger pin HIGH selama 10 microsekon, kemudian LOW
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);      // set echoPin sbg pin input utk menerima data
  // untuk membaca echo pin dan memberikan data travel time dalam microsekon
  return pulseIn(echoPin, HIGH);
}

void rotateServo(float left, float right) {
  if (left > disMax && right > disMax) {  // set posisi default servo (90)
    pos = 90;
    kemudi.write(pos);
    digitalWrite(kiriLED, LOW);       // LED kiri off
    digitalWrite(kananLED, LOW);      // LED kanan off
    analogWrite(pinMotorA, 0);        // set pinMotorA ke 0 (RPM tinggi)
  }
      
  if (left <= disMax && left > halfMax) { // set posisi servo 45 jika terdapat benda di kiri pada jarak antara disMax dan halfMax (belok kanan)
    pos = 45;
    kemudi.write(pos);
    digitalWrite(kiriLED, LOW);       // LED kiri off
    analogWrite(pinMotorA, 85);       // set pinMotorA ke 85 (RPM sedang)
  }
  else if (left <= halfMax) {       // set posisi servo 0 jika terdapat benda di kiri pada jarak kurang dari halfMax (belok kanan tajam)
    pos = 0;
    kemudi.write(pos);
    digitalWrite(kiriLED, HIGH);      // LED kiri on
    analogWrite(pinMotorA, 170);      // set pinMotorA ke 170 (RPM lambat)
  }
      
  if (right <= disMax && right > halfMax) { // set posisi servo 135 jika terdapat benda di kanan pada jarak antara disMax dan halfMax (belok kiri)
    pos = 135;
    kemudi.write(pos);
    digitalWrite(kananLED, LOW);      // LED kanan off
    analogWrite(pinMotorA, 85);       // set pinMotorB ke 85 (RPM sedang)
  }
  else if (right <= halfMax) {        // set posisi servo 180 jika terdapat benda di kanan pada jarak kurang dari halfMax (belok kiri tajam)
    pos = 180;
    kemudi.write(pos);
    digitalWrite(kananLED, HIGH);     // LED kanan on
    analogWrite(pinMotorA, 170);      // set pinMotorB ke 170 (RPM lambat)
  }
}
