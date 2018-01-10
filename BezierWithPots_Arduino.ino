#include <AccelStepper.h>
#include <MultiStepper.h>
#include <Servo.h>

MultiStepper steppers; 

AccelStepper stepper1(1, 2, 3);
AccelStepper stepper2(1, 10, 11);
AccelStepper stepper3(1, 5, 6);

int ratioCount = 0;

typedef struct Point {
  float x, y;
} Point;

int count = 0;
float spiralRatio;
float angle = 2 * PI *  spiralRatio;
float h = 0;
int startButton = 24;
int aa, bb, cc, dd, ee, ff, gg, hh;

int AnalogPin13 = A13; 
int AnalogPin12 = A12; 
int AnalogPin11 = A11;
int AnalogPin10 = A10;
int AnalogPin1 = A1;
int AnalogPin0 = A0;
int AnalogPin4 = A4;
int AnalogPin3 = A3;

int Value1, Value2, Value3, Value4, Value5, Value6, Value7, Value8;

void setup() {
  Serial.begin(9600);

  // randomSeed(1337);

  pinMode(9, INPUT_PULLUP);

  pinMode(A13, INPUT);
  pinMode(A12, INPUT);
  pinMode(A11, INPUT);
  pinMode(A10, INPUT);
  pinMode(A1, INPUT);
  pinMode(A0, INPUT);
  pinMode(A4, INPUT);
  pinMode(A3, INPUT);
  pinMode(A5, INPUT_PULLUP);
  pinMode(24, INPUT_PULLUP);

  stepper1.setMaxSpeed(1800);
  stepper1.setAcceleration(1000);

  stepper2.setMaxSpeed(1800);
  stepper2.setAcceleration(1000);

  steppers.addStepper(stepper1);
  steppers.addStepper(stepper2);

  stepper1.setMaxSpeed(1800);
  stepper2.setMaxSpeed(1800);

  stepper3.setMaxSpeed(1000);
  stepper3.setAcceleration(1000);
}

void loop() {
  findPotValues();
  startButton = digitalRead(24);
  
  while(startButton == 1) {
    findPotValues();
    startButton = digitalRead(24);
    
    aa = analogRead(A13);
    bb = analogRead(A12);
    cc = analogRead(A11);
    dd = analogRead(A10);
    ee = analogRead(A1);
    ff = analogRead(A3);
    gg = analogRead(A4);
    hh = analogRead(A0);

    aa = aa * 10;
    bb = bb * 10;
    cc = cc * 11;
    dd = dd * 11;
    ee = ee * 11;
    ff = ff * 11;
    gg = gg * 10;
    hh = hh * 10;
  }

  unsigned long seed = seedOut(31);

  ratioCount = 0;
  count = 0;
  randomSeed(seed);

  float boxWidth = 8000;
  float boxHeight = 5000;

  Point a = { 
    .x = aa, .y = bb };
  Point b = { 
    .x = cc, .y = dd };
  Point c = { 
    .x = ee, .y = ff };
  Point d = { 
    .x = gg, .y = hh };

  int nAngle = 0;
  float increment = .01;
  int radius = 80;

  drawBezierCircles(&a, &b, &c, &d);
  delay(1000);
}

void interpolate(Point *result, Point *start, Point *end, float ratio) {
  float diffX = end->x - start->x;
  float diffY = end->y - start->y;

  result->x = start->x + ratio * diffX;
  result->y = start->y + ratio * diffY;
}

Point bezierPoint(Point *result, Point *p1, Point *p2, Point *p3, Point *p4, float ratio) {
  Point midA; 
  interpolate(&midA, p1, p2, ratio);
  Point midB; 
  interpolate(&midB, p2, p3, ratio);
  Point midC; 
  interpolate(&midC, p3, p4, ratio);

  Point centerA; 
  interpolate(&centerA, &midA, &midB, ratio);
  Point centerB; 
  interpolate(&centerB, &midB, &midC, ratio);

  interpolate(result, &centerA, &centerB, ratio);
}

void drawBezierCircles(Point *p1, Point *p2, Point *p3, Point *p4) {
  float inc = 1.0f / 200.0f;

  int r = random(300, 1000);
  stepper3.moveTo(200);
  stepper3.runToPosition();
  float ratio;
  float spiralRatio;
  Point currentPoint;
  bezierPoint(&currentPoint, p1, p2, p3, p4,  ratio - inc);
  Point nextPoint;
  bezierPoint(&nextPoint, p1, p2, p3, p4, ratio);

  float angle = 2 * PI * spiralRatio;
  Point center;
  interpolate(&center, &currentPoint, &nextPoint, spiralRatio);
  float modr = 200 * cos(4 * 2 * PI * (ratio + spiralRatio * inc)) + r;

  long position[2];

  for (float ratio = inc; ratio < 1; ratio += inc) {
    Point currentPoint; 
    bezierPoint(&currentPoint, p1, p2, p3, p4, ratio - inc);
    Point nextPoint; 
    bezierPoint(&nextPoint, p1, p2, p3, p4, ratio);

    for (float spiralRatio = 0; spiralRatio < 1; spiralRatio += 1.0f / 64.0f) {
      float angle = 2 * PI * spiralRatio;
      Point center; 
      interpolate(&center, &currentPoint, &nextPoint, spiralRatio);

      float modr = 200 * cos(4 * 2 * PI * (ratio + spiralRatio * inc)) + r;

      stepper1.setMaxSpeed(800);
      stepper2.setMaxSpeed(800);

      ///////////////////////////////////////////////////////////
      if (count == 0) {
        stepper1.setMaxSpeed(1800);
        stepper2.setMaxSpeed(1800);
      }
      else {

      }

      long position[2];
      position[0] = center.x; //+ modr * sin(angle);//uncomment for circles
      position[1] = center.y; //+ modr * cos(angle);//uncomment for circles

      steppers.moveTo(position);
      steppers.runSpeedToPosition();
      stepper3.moveTo(0);
      stepper3.runToPosition();

      ///////////////////////////////////////////////////////////////////
      h = h + .05;
      count += 1;
    }
    
    ratioCount++;
  }
}

void findPotValues() {
  Value1 = analogRead(AnalogPin13);
  Value2 = analogRead(AnalogPin12);
  Value3 = analogRead(AnalogPin11);
  Value4 = analogRead(AnalogPin10);
  Value5 = analogRead(AnalogPin1);
  Value6 = analogRead(AnalogPin0);
  Value7 = analogRead(AnalogPin4);
  Value8 = analogRead(AnalogPin3);

  Serial.print(Value1, DEC); 
  Serial.print(",");
  Serial.print(Value2, DEC);
  Serial.print(",");
  Serial.print(Value3, DEC);
  Serial.print(",");

  Serial.print(Value4, DEC);
  Serial.print(",");
  Serial.print(Value5, DEC);
  Serial.print(",");
  Serial.print(Value6, DEC);
  Serial.print(",");
  Serial.print(Value7, DEC);
  Serial.print(",");
  Serial.print(Value8, DEC);
  Serial.print(",");
  Serial.println();
}