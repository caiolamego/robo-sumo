/** @source main.ino
/**
/** @ShortDescription
/**
/** @author Copyright(C) 2024
/** Luciano A. do B. S. Machado
/** 
/** @version 1.1.0
/**
/** @License Copyright (C) 2024
/** Luciano A. do B. S. Machado
/** 
/** Permission is granted to use, copy, modify, and distribute this software 
/** for any purpose, provided that proper attribution to the author is maintained.
/**
/** This software is provided "as is", without warranty of any kind, express or
/** implied, including but not limited to the warranties of merchantability,
/** fitness for a particular purpose, and noninfringement. In no event shall
/** the author be liable for any claim, damages, or other liability, whether in
/** an action of contract, tort, or otherwise, arising from, out of, or in
/** connection with the software or its use.
/************************************************************************/

#include <Arduino.h>

#define SERIAL_NUMBER 9600
#define TRIGNOMETRIC_FRONT_PIN 8
#define ECHO_FRONT_PIN 7
#define SENSOR0_PIN 4
#define SENSOR1_PIN 5
#define MOTOR0_VELOCITY_PWM_PIN 9
#define MOTOR0_DIRECTION0_PIN 21
#define MOTOR0_DIRECTION1_PIN 20
#define MOTOR1_VELOCITY_PWM_PIN 10
#define MOTOR1_DIRECTION0_PIN 19
#define MOTOR1_DIRECTION1_PIN 18

// State enumeration
enum RobotState { START, TURN_RIGHT, FORWARD, BACKWARD };

// Motor Class
class Motor {
public:
    Motor(int velocityPin, int direction0Pin, int direction1Pin)
        : velocityPin(velocityPin), direction0Pin(direction0Pin), direction1Pin(direction1Pin) {
        pinMode(velocityPin, OUTPUT);
        pinMode(direction0Pin, OUTPUT);
        pinMode(direction1Pin, OUTPUT);
    }

    void moveForward(int speed) {
        analogWrite(velocityPin, speed);
        digitalWrite(direction0Pin, HIGH);
        digitalWrite(direction1Pin, LOW);
    }

    void moveBackward(int speed) {
        analogWrite(velocityPin, speed);
        digitalWrite(direction0Pin, LOW);
        digitalWrite(direction1Pin, HIGH);
    }

    void stop() {
        analogWrite(velocityPin, 0);
    }

private:
    int velocityPin;
    int direction0Pin;
    int direction1Pin;
};

// Sensor Class
class Sensor {
public:
    Sensor(int pin) : pin(pin) {
        pinMode(pin, INPUT);
    }

    bool isActivated() {
        return digitalRead(pin) == LOW;
    }

private:
    int pin;
};

// Ultrasonic Sensor Class
class UltrasonicSensor {
public:
    UltrasonicSensor(int trigPin, int echoPin) : trigPin(trigPin), echoPin(echoPin) {
        pinMode(trigPin, OUTPUT);
        pinMode(echoPin, INPUT);
    }

    bool detectObject() {
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);

        long duration = pulseIn(echoPin, HIGH);
        long distance = (duration * 0.034) / 2; // Convert to cm

        return distance > 0 && distance < 20; // Object detected within 20 cm
    }

private:
    int trigPin;
    int echoPin;
};

// State Machine Class
class StateMachine {
public:
    StateMachine() : currentState(START) {}

    void transition(RobotState newState) {
        currentState = newState;
    }

    RobotState getCurrentState() {
        return currentState;
    }

private:
    RobotState currentState;
};

// Global objects
Motor motor0(MOTOR0_VELOCITY_PWM_PIN, MOTOR0_DIRECTION0_PIN, MOTOR0_DIRECTION1_PIN);
Motor motor1(MOTOR1_VELOCITY_PWM_PIN, MOTOR1_DIRECTION0_PIN, MOTOR1_DIRECTION1_PIN);
Sensor sensor0(SENSOR0_PIN);
Sensor sensor1(SENSOR1_PIN);
UltrasonicSensor ultrasonic(TRIGNOMETRIC_FRONT_PIN, ECHO_FRONT_PIN);
StateMachine stateMachine;

void setup(void) {
    Serial.begin(SERIAL_NUMBER);
}

void loop(void) {
    switch (stateMachine.getCurrentState()) {
    case START:
        motor0.moveBackward(200);
        motor1.moveForward(200);
        stateMachine.transition(TURN_RIGHT);
        break;

    case TURN_RIGHT:
        if (ultrasonic.detectObject()) {
            motor0.stop();
            motor1.stop();
            motor0.moveForward(200);
            motor1.moveForward(200);
            stateMachine.transition(FORWARD);
        }
        break;

    case FORWARD:
        if (sensor0.isActivated() || sensor1.isActivated()) {
            motor0.stop();
            motor1.stop();
            motor0.moveBackward(200);
            motor1.moveBackward(200);
            stateMachine.transition(BACKWARD);
        }
        break;

    case BACKWARD:
        if (!sensor0.isActivated() && !sensor1.isActivated()) {
            motor0.stop();
            motor1.stop();
            motor0.moveBackward(200);
            motor1.moveForward(200);
            stateMachine.transition(TURN_RIGHT);
        }
        break;
    }
}
