int LED = 12;
int BUTTON = 7;
int countdown = 0;

void setup()
{
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT);
  digitalWrite(BUTTON, HIGH);
  Serial.begin(115200);
}


void loop()
{
  if(countdown > 0)
  {
    countdown -= 1;
    digitalWrite(LED, HIGH);
    delay(500);
    digitalWrite(LED, LOW);
    delay(500);
  }
  else if(countdown == 0 && digitalRead(BUTTON) != HIGH)
  {
    countdown = 5;
    Serial.write("press button!\n");
  }
  //  while (Serial.available() > 0)
  //  {
  //      Serial.write(Serial.read());
  //  }
  

}
