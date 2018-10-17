// =============================================================================
// A/B MACHINES PHOTO BOOTH CONTROLLER 10/17/2018
// Giada Sun <syuanchs@andrew.cmu.edu>
//
// ## SETUP ##
// 1. Tools > Board > Arduino Nano
// 2. Tools > Processor > ATmega328P (Old Bootloader)
// 3. Tools > Port > /dev/cu.usbserial
// 4. Assign the photo booth number so the Mac is able to discren each controller
// 5. Upload to Arduino
// =============================================================================


int LED = 12;
int BUTTON = 7;

bool isDebug = false;
int number = 1;

String stage = "off";
String input;
bool isPress = false;
int countdown = 0;
unsigned long countdownTime;

void setup()
{
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT);
  digitalWrite(BUTTON, HIGH);
  Serial.begin(9600);
}

void loop()
{
  
  // Light Blinks
  if(stage == "blink")
  {
    if( millis() > countdownTime && countdown == 0)
    {
      stage = "off";
      digitalWrite(LED, LOW);
    }
    else if( millis() > countdownTime && countdown%2 == 1 )
    {
      countdown -= 1;
      countdownTime = millis() + 500;
      digitalWrite(LED, HIGH);
    }
    else if( millis() > countdownTime && countdown%2 == 0 )
    {
      countdown -= 1;
      countdownTime = millis() + 500;
      digitalWrite(LED, LOW);
    }
  }
  
  // Serial Write
  if(digitalRead(BUTTON) == HIGH)
  {
    if(isPress)
    {
      isPress = false;
    }
  }
  else if(isPress == false)
  {
    Serial.print(String(number) + "_press");
    isPress = true;
  }
  
  // Serial Read
  while(Serial.available())
  {
    input = Serial.readString(); // read the incoming data as string
    Serial.println("[arduino read] " + input);
    if(stage != "on" && input == String(number) + "_on")
    {
      stage = "on";
      if(isDebug)
      {
        Serial.println("[action] light on");
      }
      digitalWrite(LED, HIGH);
    }
    else if(stage != "off" && input == String(number) + "_off")
    {
      stage = "off";
      if(isDebug)
      {
        Serial.println("[action] light off");
      }
      digitalWrite(LED, LOW);
    }
    else if(stage != "blink" && input == String(number) + "_blink")
    {
      stage = "blink";
      countdown = 8;
      countdownTime = millis() + 500;
      if(isDebug)
      {
        Serial.println("[action] light blink");
      }
      digitalWrite(LED, HIGH);
    }
  }
  delay(5);
}
