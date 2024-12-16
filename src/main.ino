// Configuração dos pinos dos sensores ultrassônicos
const int TRIG_FRONT = 8;  // Pino TRIG do sensor frontal
const int ECHO_FRONT = 7;  // Pino ECHO do sensor frontal

// Sensores de linha
const int s1_front = 4;
const int s2_front = 5;

// Configuração dos pinos da ponte H
// Motor A
const int ENA = 9;    // Pino PWM para velocidade do Motor A
const int IN1 = 21;   // Pino de direção 1 para Motor A
const int IN2 = 20;   // Pino de direção 2 para Motor A

// Motor B
const int ENB = 10;    // Pino PWM para velocidade do Motor B
const int IN3 = 19;   // Pino de direção 1 para Motor B
const int IN4 = 18;   // Pino de direção 2 para Motor B

// Variáveis para armazenar as distâncias
float distanceFront = 0.0;
int distancia = 50;

// Variáveis de controle
bool frente = false;
bool encontrou = false;
bool linha = false;
bool in_line = false;
float PWM = 25;

// Controle de tempo
unsigned long tempoAnteriorLinha = 0;
unsigned long tempoAnteriorMotor = 0;
unsigned long intervaloLinha = 1;
unsigned long intervaloMotor = 500;

void setup() {
  // Configuração dos pinos da ponte H
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Configuração dos pinos dos sensores ultrassônicos
  pinMode(TRIG_FRONT, OUTPUT);
  pinMode(ECHO_FRONT, INPUT);
  pinMode(s1_front, INPUT);
  pinMode(s2_front, INPUT_PULLUP);

  // Inicializa a comunicação serial
  Serial.begin(9600);

  // Inicializa os motores parados
  stopMotors();
}
bool objeto = 0;
bool fim = 0;
void loop() {
  
  // Medir distâncias com os sensores
  if(objeto == 0){
  distanceFront = measureDistance(TRIG_FRONT, ECHO_FRONT);
  if(distanceFront <= distancia){
    objeto = 1;
  }
}
  //PWM = 120;//(-3.1/2) * distanceFront + 255;

  // Lógica do sensor de linha
  int leituraSensor = digitalRead(s1_front);
  if (leituraSensor == 1) {
    
      linha = true;
    
  } else {
   
    linha = false;
  }
if (linha){
  fim = 1;
  
  stopMotors();
}
  if (linha && encontrou) {
    in_line = true;
  }

  // Lógica de controle dos motores
  if (!in_line && !fim) {
    if (distanceFront <= distancia) {
      frente = true;
    } else {
      frente = false;
    }

    if (frente) {
      encontrou = true;
      
        moveForward(PWM);
        
      
    } else {
      turnRight(80);
      encontrou = true;
    }
  } else {
    stopMotors();
  }
}

// Função para medir a distância com um sensor ultrassônico
float measureDistance(int trigPin, int echoPin) {
  // Gera um pulso de 10µs no pino TRIG
  digitalWrite(trigPin, LOW);
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

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, speed);
}

// Função para virar o robô para a direita
void turnRight(int speed) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
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

