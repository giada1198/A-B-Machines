// =============================================================================
// A/B MACHINES PHOTO BOOTH CONTROLLER 10/07/2018
// Giada Sun <syuanchs@andrew.cmu.edu>
//
// ## SETUP ##
// 1. Tools > Board > Arduino Nano
// 2. Tools > Processor > ATmega328P (Old Bootloader)
// 3. Tools > Port > /dev/cu.usbserial
// 4. Assign an UNIQUE number so the Mac is able to discren each controller
// 5. Upload to Arduino
// =============================================================================

String SN = "1";
int LED = 12;
int BUTTON = 7;

String stage = "off";
String input;
bool isPress = false;
int countdown = 0;

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
  if(stage == "blink") {
    countdown -= 1;
    if(countdown == 0) {
      stage = "off";
      digitalWrite(LED, LOW);
    }
  }
  
  // Serial Write
  if(digitalRead(BUTTON) == HIGH)
  {
    if(isPress) {
      isPress = false;
    }
  } else if(isPress == false) {
    Serial.println(SN + "_press");
    isPress = true;
  }
  
  // Serial Read
  while(Serial.available()) {
    input = Serial.readString(); // read the incoming data as string
    Serial.print("[READ] " + input);
    if(stage != "on" && input == SN + "_on\n") {
      stage = "on";
      Serial.println("[ACTION] LIGHT ON");
      digitalWrite(LED, HIGH);
    }
    else if(stage != "off" && input == SN + "_off\n") {
      stage = "off";
      Serial.println("[ACTION] LIGHT OFF");
      digitalWrite(LED, LOW);
    }
    else if(stage != "blink" && input == SN + "_blink\n") {
      stage = "blink";
      countdown = 50;
      Serial.println("[ACTION] LIGHT BLINKS");
      digitalWrite(LED, HIGH);
    }
  }
  delay(10);
}
