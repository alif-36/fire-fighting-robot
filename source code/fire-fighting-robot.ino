#include <Servo.h>
#include <SoftwareSerial.h>

Servo myservo;
SoftwareSerial gsm(8, 9);

int pos = 0;
boolean fire = false;
boolean alertSent = false;

#define Left A2
#define Right 7
#define Forward A3

#define LM1 2
#define LM2 3
#define RM1 4
#define RM2 5
#define ENA A0
#define ENB A1
#define pump 6
#define SPEED 150

void sendAlert()
{
  gsm.println("AT");
  delay(1000);

  gsm.println("AT+CMGF=1");
  delay(1000);

  gsm.println("AT+CMGS=\"+8801XXXXXXXXX\"");  // use your number
  delay(1000);

  gsm.print("Fire detected! Fire fighting robot is responding.");
  delay(500);

  gsm.write(26);
  delay(3000);
}

void setup()
{
  pinMode(Left, INPUT);
  pinMode(Right, INPUT);
  pinMode(Forward, INPUT);
  pinMode(LM1, OUTPUT);
  pinMode(LM2, OUTPUT);
  pinMode(RM1, OUTPUT);
  pinMode(RM2, OUTPUT);
  pinMode(pump, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  digitalWrite(pump, HIGH);

  myservo.attach(11);
  myservo.write(90);

  gsm.begin(9600);
  delay(2000);
}

void put_off_fire()
{
  delay(500);

  digitalWrite(LM1, LOW);
  digitalWrite(LM2, LOW);
  digitalWrite(RM1, LOW);
  digitalWrite(RM2, LOW);

  digitalWrite(pump, LOW);
  delay(500);

  for (pos = 50; pos <= 130; pos += 1)
  {
    myservo.write(pos);
    delay(10);
  }

  for (pos = 130; pos >= 50; pos -= 1)
  {
    myservo.write(pos);
    delay(10);
  }

  digitalWrite(pump, HIGH);
  myservo.write(90);

  fire = false;
  alertSent = false;
}

void loop()
{
  myservo.write(90);
  digitalWrite(pump, HIGH);

  if (digitalRead(Left) == 1 &&
      digitalRead(Right) == 1 &&
      digitalRead(Forward) == 1)
  {
    analogWrite(ENA, 0);
    analogWrite(ENB, 0);
  }

  else if (digitalRead(Forward) == 0)
  {
    digitalWrite(LM1, HIGH);
    digitalWrite(LM2, LOW);
    analogWrite(ENA, SPEED);

    digitalWrite(RM1, HIGH);
    digitalWrite(RM2, LOW);
    analogWrite(ENB, SPEED);

    fire = true;

    if (alertSent == false)
    {
      sendAlert();
      alertSent = true;
    }

    delay(200);

    analogWrite(ENA, 0);
    analogWrite(ENB, 0);
  }

  else if (digitalRead(Left) == 0)
  {
    digitalWrite(LM1, HIGH);
    digitalWrite(LM2, LOW);
    analogWrite(ENA, 150);

    digitalWrite(RM1, LOW);
    digitalWrite(RM2, LOW);
    analogWrite(ENB, 0);

    delay(300);

    analogWrite(ENA, 0);
    analogWrite(ENB, 0);
  }

  else if (digitalRead(Right) == 0)
  {
    digitalWrite(LM1, LOW);
    digitalWrite(LM2, LOW);
    analogWrite(ENA, 0);

    digitalWrite(RM1, HIGH);
    digitalWrite(RM2, LOW);
    analogWrite(ENB, 150);

    delay(300);

    analogWrite(ENA, 0);
    analogWrite(ENB, 0);
  }

  delay(300);

  while (fire == true)
  {
    put_off_fire();
  }
}