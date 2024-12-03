// Configuração dos pinos dos sensores ultrassônicos
const int TRIG_FRONT = 12;  // Pino TRIG do sensor frontal
const int ECHO_FRONT = 13;  // Pino ECHO do sensor frontal
const int TRIG_LEFT = 11;   // Pino TRIG do sensor esquerdo
const int ECHO_LEFT = 10;   // Pino ECHO do sensor esquerdo
const int TRIG_RIGHT = 9;   // Pino TRIG do sensor direito
const int ECHO_RIGHT = 8;   // Pino ECHO do sensor direito


// sensor de linha
const int sensorPin = 2;

// Configuração dos pinos da ponte H
// Motor A
const int ENA = 6;    // Pino PWM para velocidade do Motor A
const int IN1 = 22;   // Pino de direção 1 para Motor A
const int IN2 = 23;   // Pino de direção 2 para Motor A

// Motor B
const int ENB = 5;    // Pino PWM para velocidade do Motor B
const int IN3 = 24;   // Pino de direção 1 para Motor B
const int IN4 = 25;   // Pino de direção 2 para Motor B

// Variáveis para armazenar as distâncias
float distanceFront = 0.0;
float distanceLeft = 0.0;
float distanceRight = 0.0;
int distancia = 10;


void setup() {
  // Configuração dos pinos da ponte H
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  //sensor de linha
  pinMode(sensorPin, INPUT); 

  // Configuração dos pinos dos sensores ultrassônicos
  pinMode(TRIG_FRONT, OUTPUT);
  pinMode(ECHO_FRONT, INPUT);
  pinMode(TRIG_LEFT, OUTPUT);
  pinMode(ECHO_LEFT, INPUT);
  pinMode(TRIG_RIGHT, OUTPUT);
  pinMode(ECHO_RIGHT, INPUT);

  // Inicializa a comunicação serial
  Serial.begin(9600);

  // Inicializa os motores parados
  stopMotors();
}
bool frente;
bool esquerda;
bool direita;

void loop() {
  // Medir distâncias com os sensores
  distanceFront = measureDistance(TRIG_FRONT, ECHO_FRONT);
  distanceLeft = measureDistance(TRIG_LEFT, ECHO_LEFT);
  distanceRight = measureDistance(TRIG_RIGHT, ECHO_RIGHT);

// Exibir as distâncias no monitor serial
//  Serial.print("Frontal: ");
//  Serial.print(distanceFront);
//  Serial.print(" cm | Esquerda: ");
//  Serial.print(distanceLeft);
//  Serial.print(" cm | Direita: ");
//  Serial.println(distanceRight);

  // Lógica de controle dos motores
  if ( distanceFront <= distancia) {
    frente = 1;
    
  }else{
     frente = 0;
  }
   if ( distanceLeft <= distancia) {
   esquerda = 1;
  }else{
    esquerda = 0;
  }
  if ( distanceRight <= 20) {
    direita = 1;
 
  }else{
    direita = 0;
  }

  // Lógica para o detector de linha
  if (digitalRead(sensorPin) == HIGH) { // Linha detectada
    moveBackward(200); // Dar ré
    delay(500);        // Pequeno atraso para recuar
    stopMotors();      // Parar após dar ré
  } else {
    // Lógica original para movimentação
    if (frente == 1) {
      moveForward(200);
    } else if (frente == 0) {
      stopMotors();
    }
    if (direita == 1) {
      if (frente == 0) {
        turnRight(200);
      }
    }
    if (esquerda == 1) {
      if (frente == 0) {
        turnLeft(200);
      }
    }
  }

  delay(200); // Pequeno atraso para estabilidade
}

// Função para medir a distância com um sensor ultrassônico
float measureDistance(int trigPin, int echoPin) {
  // Gera um pulso de 10µs no pino TRIG
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Mede a duração do pulso ECHO em microssegundos
  long duration = pulseIn(echoPin, HIGH);

  // Calcula a distância em centímetros
  return duration * 0.034 / 2; // Velocidade do som: 0.034 cm/µs
}

// Função para mover o robô para frente
void moveForward(int speed) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, speed);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, speed);
}

// Função para mover o robô para trás
void moveBackward(int speed) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, speed);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, speed);
}

// Função para virar o robô para a direita
void turnRight(int speed) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, speed);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, speed);
}

// Função para virar o robô para a esquerda
void turnLeft(int speed) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, speed);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, speed);
}

// Função para parar os motores
void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 0);
}

