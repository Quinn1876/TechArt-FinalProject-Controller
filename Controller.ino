#include "Arduino.h"
#include "Servo.h"
#include "MirrorServo.hpp"

#define NUM_PATTERNS 3

using namespace TechArt;
void button_handler();
void zero();
void run_state_machine();
void run_servo_test();
void run_pattern();
void test_smoke_machine();

// HARDWARE UNO

Hardware::Button startButton(5);
Hardware::Relay smokeMachine(3);

MirrorServo servos[10] = {
    MirrorServo(2, 90),
    MirrorServo(4, 90),
    MirrorServo(6, 90),
    MirrorServo(7, 90),
    MirrorServo(8, 90),
    MirrorServo(12, 90),
    MirrorServo(13, 90),
    MirrorServo(A1, 90),
    MirrorServo(A2, 90),
    MirrorServo(A3, 90),
};

int angles[NUM_PATTERNS][10] = {
  {
    0,0,0,0,0,
    0,0,0,-20,0
  },
  {
    0,0,-40,-3,0,
    22,0,0,0,0
  },
  {
    0,-5,-3,0,-16,
    -18,23,0,0,0,
  }
};

int lazers[5] = {
    0,
    0,
    0,
    0,
    0,
};

// Show
Pattern patterns[NUM_PATTERNS] = {
  Pattern(400, angles[0], lazers),
  Pattern(400, angles[1], lazers),
  Pattern(400, angles[2], lazers),
};


// Communication
I2C::Controller controller;

void zero() {
  for (auto & servo : servos) {
    servo.setAngle(0);
  }
}

enum StateMachine {
  WaitingForShow,
  OpeningDoor,
  PlayShow,
  LetPeopleOut
};

volatile StateMachine state_g = WaitingForShow;
int pattern_index_g = 0;

void setup() {
    for (auto& servo : servos) {
        servo.initialize();
    }
    startButton.registerOnPressed(*button_handler);
    controller.initialize();
    smokeMachine.initialize();
    Serial.begin(9600);
    // pattern[0].applyPatternServos(servos);
}

void loop() {
  // run_state_machine();
  // run_servo_test();
  // run_pattern();
  test_smoke_machine();
}

void run_pattern() {
  static int lights1[5] = {
    0,0,0,0,0
  };
  static int angles1[10] = {
    0,-5,-3,0,-16,
    -18,23,0,0,0,
  };
  static Pattern pattern(1000, angles1, lights1);
  pattern.applyPatternServos(servos);  
  pattern.applyPatternDelay();

  // patterns[1].applyPatternServos(servos);  
  // patterns[1].applyPatternDelay();
  
}

void test_smoke_machine() {
  smokeMachine.on();
  delay(180.0_s);
  // smokeMachine.off();
  while(1);
}

void run_servo_test() {
  for (auto& servo : servos ) {
    servo.setAngle(10);
  }
  delay(1.0_s);
  for (auto& servo : servos ) {
    servo.setAngle(-10);
  }
  delay(1.0_s);
}

void run_state_machine() {
  switch (state_g) {
    case WaitingForShow: { zero(); delay(200); } break;
    case OpeningDoor: {
      pattern_index_g = 0;
      state_g = PlayShow;
      controller.openDoor();volatile 
      delay(10.0_s);
    } break;
    case PlayShow: {
      if (pattern_index_g == NUM_PATTERNS) {
        state_g = LetPeopleOut;
        break;
      }
      controller.sendPattern(pattern_index_g);
      patterns[pattern_index_g].applyPatternServos(servos);
      patterns[pattern_index_g].applyPatternDelay();
      pattern_index_g++;
    } break;
    case LetPeopleOut: {
      controller.openDoor();
      delay(10.0_s);
    } break;
    default:
      state_g = WaitingForShow;
  }
}

void button_handler() {
  if (state_g == WaitingForShow) {
    state_g = OpeningDoor;
  }
  Serial.println("Hi");
}
