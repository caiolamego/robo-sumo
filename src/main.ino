// Configuração dos pinos dos sensores ultrassônicos
const int TRIG_FRONT = 15;  // Pino TRIG do sensor frontal (emite pulsos ultrassônicos)
const int ECHO_FRONT = 14;  // Pino ECHO do sensor frontal (recebe o eco dos pulsos)

// Sensores de linha
const int s1_front = 16;  // Pino do sensor de linha frontal 1
const int s2_front = 5;   // Pino do sensor de linha frontal 2

// Configuração dos pinos da ponte H (controla os motores)
// Motor A
const int ENA = 9;    // Pino PWM para controlar a velocidade do Motor A
const int IN1 = 21;   // Pino de direção 1 para Motor A
const int IN2 = 20;   // Pino de direção 2 para Motor A

// Motor B
const int ENB = 10;   // Pino PWM para controlar a velocidade do Motor B
const int IN3 = 19;   // Pino de direção 1 para Motor B
const int IN4 = 18;   // Pino de direção 2 para Motor B

// Variáveis para armazenar as distâncias
float distanceFront = 0.0; // Armazena a distância média medida pelo sensor frontal
const int distancia = 150;  // Distância limite para detectar um objeto (em cm)

// Variáveis de controle
bool frente = false;    // Indica se o robô deve avançar
bool encontrou = false; // Indica se um objeto foi detectado
bool linha = false;     // Indica se o sensor detectou uma linha
bool in_line = false;   // Indica se o robô está sobre a linha
float PWM = 180;        // Valor do PWM para controlar a velocidade dos motores
int reset = 0;          // Flag para reiniciar os estados do robô
bool objeto = false; // Indica se um objeto está próximo
bool fim = false;    // Indica se o robô chegou ao fim do percurso
bool parar = 0;
// Controle de tempo
unsigned long tempoAnteriorLinha = 0; // Tempo anterior para verificar linha
unsigned long tempoAnteriorMotor = 0; // Tempo anterior para controle do motor
unsigned long intervaloLinha = 1;     // Intervalo mínimo entre leituras de linha (ms)
unsigned long intervaloMotor = 500;   // Intervalo mínimo entre ajustes de motor (ms)

// Função de configuração
void setup() {
  // Configuração dos pinos da ponte H
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Configuração dos pinos dos sensores ultrassônicos
  pinMode(TRIG_FRONT, OUTPUT);  // Configura TRIG como saída
  pinMode(ECHO_FRONT, INPUT);   // Configura ECHO como entrada
  pinMode(s1_front, INPUT);     // Configura sensor de linha 1 como entrada
  pinMode(s2_front, INPUT_PULLUP); // Configura sensor de linha 2 com resistor pull-up

  // Inicializa a comunicação serial
  Serial.begin(9600);

  // Inicializa os motores parados
  stopMotors();
}

// Variáveis adicionais

void loop() {
  // Enquanto não houver objeto próximo
  while (!objeto) {
     objeto = verificaObjeto(TRIG_FRONT, ECHO_FRONT, distancia);

   
    turnRight(80); // Se não houver objeto, continua girando à direita
  }

  // Verifica o estado do sensor de linha
  int leituraSensor = digitalRead(s1_front);
  linha = (leituraSensor == 1); // Atualiza a flag "linha"

  // Se detectar uma linha, para o robô
  if (linha) {
    fim = true;
    stopMotors();
  }

  // Lógica de controle do movimento
  if (!in_line && !fim) {
    frente = (objeto == true); // Define o estado "frente"

    if (frente) {
      encontrou = true;
      moveForward(PWM); // Avança para frente
    } else {
      turnRight(80); // Continua girando à direita
      encontrou = true;
    }
  } else {
    retornar();
    // Para os motores se estiver na linha ou fim do percurso
    reset = 1;    // Sinaliza para reiniciar os estados
  }

  // Reinicia estados quando necessário
  if (reset == 1) {
    objeto = false;
    frente = false;
    encontrou = false;
    linha = false;
    in_line = false;
    reset = 0;
    fim = false; 
    parar = 0;
  }
  
}

void retornar() {
  float Distance = calculaDistanciaMedia(TRIG_FRONT, ECHO_FRONT);
  float Distance_anterior = Distance;
  bool objetonafrente = verificaObjeto(TRIG_FRONT, ECHO_FRONT, distancia);
  if (!objetonafrente && !parar){
    moveBackwards(255);
    delay(500);
    stopMotors(); 
    parar = 1;
  }else if(!parar){
    while(Distance < Distance_anterior + 5){
      Distance = calculaDistanciaMedia(TRIG_FRONT, ECHO_FRONT);
      moveBackwards(80);
    }
    parar = 1;
    while(verificaObjeto(TRIG_FRONT, ECHO_FRONT, distancia)){
      turnRight(80);
    }
    stopMotors(); 
  }





  
}// Função que retorna a distância média em cm
float calculaDistanciaMedia(int trigPin, int echoPin) {
  float totalDistance = 0;

  // Mede 5 valores consecutivos de distância
  for (int i = 0; i < 5; i++) {
    totalDistance += measureDistance(trigPin, echoPin);
    delay(50); // Pequeno atraso entre medições
  }

  return totalDistance / 5.0; // Retorna a média das medições
}

// Função que verifica se há um objeto dentro de uma distância limite
bool verificaObjeto(int trigPin, int echoPin, float distanciaLimite) {
  float distanciaMedia = calculaDistanciaMedia(trigPin, echoPin);
  return (distanciaMedia <= distanciaLimite);
}

// Função para medir a distância com sensor ultrassônico
float measureDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH); // Mede a duração do pulso ECHO
  return duration * 0.034 / 2;           // Calcula a distância (em cm)
}

// Função para mover o robô para frente
void moveForward(int speed) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, speed);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, speed);
}

// Função para mover o robô para tras
void moveBackwards(int speed) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, speed);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, speed);
}


// Função para virar à direita
void turnRight(int speed) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, speed);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, speed);
}

// Função para virar à esquerda
void turnLeft(int speed) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, speed);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, speed);
}

// Função para parar os motores
void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 255);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 255);
}