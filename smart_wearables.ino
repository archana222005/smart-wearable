#include <Wire.h>
#include <MPU6050.h>

// ── Your phone number ──────────────────────────
String phoneNumber = "+919176764050";

// ── Geofence settings ──────────────────────────
float safe_lat    = 13.0827;
float safe_lon    = 80.2707;
float safe_radius = 0.5;

// ── Simulated GPS ──────────────────────────────
float current_lat = 13.0900;
float current_lon = 80.2800;

// ── Pins ───────────────────────────────────────
int buzzerPin = 8;
int ledGreen  = 12;
int ledRed    = 13;

// ── MPU6050 ────────────────────────────────────
MPU6050 mpu;
int16_t ax, ay, az, gx, gy, gz;

#define FALL_THRESHOLD    7000
#define IMPACT_THRESHOLD  25000
#define INACTIVITY_LIMIT  10000
#define MOVEMENT_SENS     1500

bool fallDetected = false;
bool smsSent      = false;
unsigned long lastMoveTime = 0;
float lastAx = 0;

// ─────────────────────────────────────────────
float calculateDistance(float lat1, float lon1,
                        float lat2, float lon2) {
  float dlat = lat2 - lat1;
  float dlon = lon2 - lon1;
  return sqrt(dlat * dlat + dlon * dlon) * 111.0;
}

// ── Simulated SMS function ────────────────────
void sendSMS(String message) {
  Serial.println("╔══════════════════════════════════╗");
  Serial.println("║         SMS ALERT TRIGGERED       ║");
  Serial.println("╠══════════════════════════════════╣");
  Serial.print  ("║ TO : ");
  Serial.println(phoneNumber);
  Serial.print  ("║ MSG: ");
  Serial.println(message);
  Serial.println("╚══════════════════════════════════╝");
}

void alertBuzzer(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(buzzerPin, HIGH);
    delay(300);
    digitalWrite(buzzerPin, LOW);
    delay(200);
  }
}

// ─────────────────────────────────────────────
void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();

  pinMode(buzzerPin, OUTPUT);
  pinMode(ledGreen,  OUTPUT);
  pinMode(ledRed,    OUTPUT);

  if (mpu.testConnection()) {
    Serial.println("✔ MPU6050 Connected OK");
  } else {
    Serial.println("✘ MPU6050 Connection FAILED");
  }

  Serial.println("==============================");
  Serial.println("  Smart Wearable System ON  ");
  Serial.println("==============================");

  // Light up green on start = safe
  digitalWrite(ledGreen, HIGH);
  lastMoveTime = millis();
}

// ─────────────────────────────────────────────
void loop() {

  // ── 1. Read accelerometer ──────────────────
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  float totalAccel = sqrt((float)ax*ax +
                          (float)ay*ay +
                          (float)az*az);

  Serial.print("Accel: ");
  Serial.print(totalAccel);

  // ── 2. Fall detection ──────────────────────
  if (totalAccel < FALL_THRESHOLD) {
    Serial.println("  << FREE FALL >>");
    fallDetected = true;
  } else if (fallDetected && totalAccel > IMPACT_THRESHOLD) {
    Serial.println("  << IMPACT AFTER FALL >>");
    digitalWrite(ledRed, HIGH);
    digitalWrite(ledGreen, LOW);
    alertBuzzer(5);
    sendSMS("EMERGENCY! Fall + impact detected. Check immediately.");
    fallDetected = false;
  } else {
    Serial.println("");
  }

  // ── 3. Inactivity detection ────────────────
  float accelChange = abs(ax - lastAx);
  if (accelChange > MOVEMENT_SENS) {
    lastMoveTime = millis();
  }
  lastAx = ax;

  if (millis() - lastMoveTime > INACTIVITY_LIMIT) {
    Serial.println("WARNING: No movement for 10s!");
    alertBuzzer(2);
    sendSMS("WARNING: No movement detected for 10 seconds.");
    lastMoveTime = millis();
  }

  // ── 4. Geofencing ──────────────────────────
  float distance = calculateDistance(safe_lat, safe_lon,
                                     current_lat, current_lon);

  Serial.print("Distance from safe zone: ");
  Serial.print(distance);
  Serial.println(" km");

  if (distance > safe_radius) {
    Serial.println("STATUS: !! OUTSIDE SAFE ZONE !!");
    digitalWrite(ledRed,   HIGH);
    digitalWrite(ledGreen, LOW);
    alertBuzzer(3);

    if (!smsSent) {
      sendSMS("ALERT! Left safe zone. Distance: "
              + String(distance) + " km from home.");
      smsSent = true;
    }

  } else {
    Serial.println("STATUS: SAFE inside zone");
    digitalWrite(ledRed,   LOW);
    digitalWrite(ledGreen, HIGH);
    digitalWrite(buzzerPin, LOW);
    smsSent = false;
  }

  Serial.println("------------------------------");
  delay(2000);
}
