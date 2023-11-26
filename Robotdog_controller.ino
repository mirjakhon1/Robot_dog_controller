#include <nRF24L01.h>
#include <printf.h>
#include <RF24_config.h>

#include <SPI.h>
#include <RF24.h>

#define joy_x A0    // x axis
#define joy_y A1    // y axis
#define joy_button 8 // joystick button

#define A 2         // forward button
#define B 3         // right
#define C 4         // back
#define D 5         // left

#define E 6
#define F 7

int x_val_joy = 0;
int y_val_joy = 0;
int button_joy_val = 0;

int forward_button = 0;
int right_button = 0;
int left_button = 0;
int back_button = 0;

int E_button = 0;
int F_button = 0;

//ce, csn pins
RF24 radio(9, 10);

void setup() {
  // spi radio communication 
  Serial.begin(9600);
  radio.begin();
  radio.setPALevel(RF24_PA_MIN);
  radio.setChannel(0x30);
  radio.openWritingPipe(0xF0F0F0F0E1LL);
  radio.enableDynamicPayloads();
  radio.powerUp();
  radio.stopListening();

  // gamepad control
  pinMode(joy_button, INPUT);
  pinMode(A, INPUT);
  pinMode(B, INPUT);
  pinMode(C, INPUT);
  pinMode(D, INPUT);
  pinMode(E, INPUT);
  pinMode(F, INPUT);
}

void loop() {
  x_val_joy = analogRead(joy_x);          // default 327, left 0, right 663
  y_val_joy = analogRead(joy_y);          // default 323, forward 663, back 0
  button_joy_val = digitalRead(joy_button);

  forward_button = digitalRead(A);        // increase speed
  right_button = digitalRead(B);          // increase turn speed
  back_button = digitalRead(C);           // decrease speed
  left_button = digitalRead(D);           // decrease turn speed

  E_button = digitalRead(E);

  if      (y_val_joy > 370){ y_val_joy = 1;}
  else if (y_val_joy < 300){ y_val_joy = -1;}
  else                     { y_val_joy = 0;}

  if      (x_val_joy > 460){ x_val_joy = 1;}
  else if (x_val_joy < 200){ x_val_joy = -1;}
  else                     { x_val_joy = 0;}

  // y_val_joy++;
  // x_val_joy++;
  String dataString = String(y_val_joy) + "," 
                      + String(x_val_joy) + "," 
                      + String(forward_button) + ","
                      + String(back_button) + ","
                      + String(right_button) + ","
                      + String(left_button) + ","
                      + String(E_button);

  radio.write(dataString.c_str(), dataString.length() + 1);
  delay(10);
}