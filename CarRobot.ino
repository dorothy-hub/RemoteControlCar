const int l1=12;
const int l2=13;
const int r1=15;
const int r2=16;
void setup() {
  // put your setup code here, to run once:
pinMode(l1, OUTPUT);
pinMode(l2, OUTPUT);
pinMode(r1, OUTPUT);
pinMode(r2, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
forward();
delay (3000);
stop_();
delay(1000); 
turn();
delay(2000);
}

void forward () {
  //
  digitalWrite(l1, HIGH);
  digitalWrite(l2, LOW);
  digitalWrite(r1, HIGH);
  digitalWrite(r2, LOW);
}

void stop_() {
  digitalWrite(l1, LOW);
  digitalWrite(l2, LOW);
  digitalWrite(r1, LOW);
  digitalWrite(r2, LOW);
}

void turn () {
  digitalWrite(l1, HIGH);
  digitalWrite(l2, LOW);
  digitalWrite(r1, HIGH);
  digitalWrite(r2, LOW);
}
