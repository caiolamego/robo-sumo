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

void setup(void);
void loop(void);
static float measureDistance(int, int);
static void moveForward(int);
static static void turnRight(int);
static void turnLeft(int);
static void stopMotors(void);

static float distanceFront=0.0;
static int distancia=50;

static bool frente=false;
static bool encontrou=false;
static bool linha=false;
static bool in_line=false;
static float PWM=25;

static unsigned long tempoAnteriorLinha=0;
static unsigned long tempoAnteriorMotor=0;
static unsigned long intervaloLinha=1;
static unsigned long intervaloMotor=500;

void setup()
{

  // Configuração dos pinos da ponte H
  pinMode(MOTOR0_VELOCITY_PWM_PIN, OUTPUT);
  pinMode(MOTOR0_DIRECTION1_PIN, OUTPUT);
  pinMode(MOTOR0_DIRECTION1_PIN, OUTPUT);
  pinMode(MOTOR1_VELOCITY_PWM_PIN, OUTPUT);
  pinMode(MOTOR1_DIRECTION0_PIN, OUTPUT);
  pinMode(MOTOR1_DIRECTION1_PIN, OUTPUT);

  // Configuração dos pinos dos sensores ultrassônicos
  pinMode(TRIGNOMETRIC_FRONT_PIN, OUTPUT);
  pinMode(ECHO_FRONT_PIN, INPUT);
  pinMode(SENSOR0_PIN, INPUT);
  pinMode(SENSOR1_PIN, INPUT_PULLUP);

  // Inicializa a comunicação serial
  Serial.begin(SERIAL_NUMBER);

  // Inicializa os motores parados
  stopMotors();
}

void loop()
{
  register bool objeto=0;
  register bool fim=0;

  // Medir distâncias com os sensores
  if(!objeto)
  {
    distanceFront=measureDistance(TRIGNOMETRIC_FRONT_PIN, ECHO_FRONT_PIN);
    if(distanceFront<=distancia){
      objeto=1;
    }
  }
  //PWM=120;//(-3.1/2) * distanceFront + 255;

  // Lógica do sensor de linha
  int leituraSensor=digitalRead(SENSOR0_PIN);
  if(leituraSensor==1)
    linha=true;
  else
    linha=false;
  if(linha)
  {
    fim=1;
    stopMotors();
  }
  if(linha&&encontrou)
    in_line=true;

  // Lógica de controle dos motores
  if((!in_line)&&(!fim))
  {
    if (distanceFront<=distancia)
      frente=true;
    else
      frente=false;

    if(frente)
    {
      encontrou=true;
      moveForward(PWM);
    }
    else
    {
      turnRight(80);
      encontrou=true;
    }
  }
  else
    stopMotors();
}

// Função para medir a distância com um sensor ultrassônico
static float measureDistance(int trigPin, int echoPin)
{
  // Gera um pulso de 10µs no pino TRIG
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Mede a duração do pulso ECHO em microssegundos
  long duration=pulseIn(echoPin, HIGH);

  // Calcula a distância em centímetros
  return duration * 0.034 / 2; // Velocidade do som: 0.034 cm/µs
}

// Função para mover o robô para frente
static void moveForward(int speed)
{
  digitalWrite(MOTOR0_DIRECTION1_PIN, HIGH);
  digitalWrite(MOTOR0_DIRECTION1_PIN, LOW);
  analogWrite(MOTOR0_VELOCITY_PWM_PIN, speed);

  digitalWrite(MOTOR1_DIRECTION0_PIN, HIGH);
  digitalWrite(MOTOR1_DIRECTION1_PIN, LOW);
  analogWrite(MOTOR1_VELOCITY_PWM_PIN, speed);
}

// Função para virar o robô para a direita
static void turnRight(int speed)
{
  digitalWrite(MOTOR0_DIRECTION1_PIN, LOW);
  digitalWrite(MOTOR0_DIRECTION1_PIN, HIGH);
  analogWrite();

  digitalWrite(MOTOR1_DIRECTION0_PIN, HIGH);
  digitalWrite(MOTOR1_DIRECTION1_PIN, LOW);
  analogWrite(MOTOR1_VELOCITY_PWM_PIN, speed);
}

// Função para virar o robô para a esquerda
static void turnLeft(int speed)
{
  digitalWrite(MOTOR0_DIRECTION1_PIN, LOW);
  digitalWrite(MOTOR0_DIRECTION1_PIN, HIGH);
  analogWrite();

  digitalWrite(MOTOR1_DIRECTION0_PIN, LOW);
  digitalWrite(MOTOR1_DIRECTION1_PIN, HIGH);
  analogWrite(MOTOR1_VELOCITY_PWM_PIN, speed);
}

// Função para parar os motores
static void stopMotors(void)
{
  digitalWrite(MOTOR0_DIRECTION1_PIN, LOW);
  digitalWrite(MOTOR0_DIRECTION1_PIN, LOW);
  analogWrite(MOTOR0_VELOCITY_PWM_PIN, 0);

  digitalWrite(MOTOR1_DIRECTION0_PIN, LOW);
  digitalWrite(MOTOR1_DIRECTION1_PIN, LOW);
  analogWrite(MOTOR1_VELOCITY_PWM_PIN, 0);
}

