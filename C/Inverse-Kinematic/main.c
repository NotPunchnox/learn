#include <math.h>
#include <stdio.h>

double radiansToDegrees(double radians) {
  return radians * (180.0 / M_PI);
}

float AlKashi(float a, float b, float c) {
  return radiansToDegrees(acos((a*a+b*b-c*c)/(2*a*b)));
}

float Racine(float a, float b) {
  return sqrt(a*a + b*b);
}

void Algo(float x, float y, float z, float coxa, float femur, float tibia) {
  
  float taillePatte = Racine(y, x);
  float hypotenus   = Racine(z, taillePatte-coxa);
  
  float A1 = radiansToDegrees(atan((taillePatte-coxa) / z));
  float A2 = AlKashi(femur, hypotenus, tibia);
  
  float AngleFemur = A1 + A2;

  float A3 = AlKashi(tibia, femur, hypotenus);

  float AngleCoxa = radiansToDegrees(atan(y / x)) + 90;
  float AngleTibia = A3;

  printf("Angle coxa: %s", AngleCoxa);
  printf("Angle femur: %s", AngleFemur);
  printf("Angle tibia: %s", AngleTibia);

}

int main() {

  float x = 12.0;
  float y = 0.0;
  float z = 8.0;

  float coxa = 5.0;
  float femur = 6.3;
  float tibia = 13.0;

  Algo(x, y, z, coxa, femur, tibia);

  return 0;
  
}
