#define outputA 8
#define outputB 9
int counter = 0;
int aState;
int aLastState;
void setup() {
  pinMode (outputA, INPUT_PULLUP);
  pinMode (outputB, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);

  Serial.begin (115200);
  Serial.println("start");
  // Reads the initial state of the outputA
  Serial.println(digitalRead(outputA));
  aLastState = digitalRead(outputA);
}
void loop() {
  aState = digitalRead(outputA); // Reads the "current" state of the outputA
  // If the previous and the current state of the outputA are different, that means a Pulse has occured
  if (aState != aLastState) {
    // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
    if (digitalRead(outputB) != aState) {
      Serial.print("CC: ");
      counter ++;
    } else {
      Serial.print("CCW: ");
      counter --;
    }
    Serial.print("Position: ");
    Serial.println(counter);
  }
  aLastState = aState; // Updates the previous state of the outputA with the current state
  if (digitalRead(7) == 0 && digitalRead(6) == 1) {
    Serial.println("RESET STATE");
  }
  if (digitalRead(6) == 0) {
    Serial.println("SOLINOID OFF");
  }
}
