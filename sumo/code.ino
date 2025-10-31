/*
  Código para Robô de Sumô Arduino
  Controla dois motores com uma Ponte H e usa um sensor ultrassônico para detectar oponentes.
*/

// ---+---===[ CONFIGURAÇÕES ]===---+---

// Pinos do Sensor Ultrassônico
const int PINO_TRIG = 9;
const int PINO_ECHO = 10;

const int MOTOR_DIREITO_IN1 = 2;
const int MOTOR_DIREITO_IN2 = 3;
const int MOTOR_DIREITO_ENA = 6; // Pino PWM para controle de velocidade do motor direito

// Pinos da Ponte H para o Motor Esquerdo
const int MOTOR_ESQUERDO_IN3 = 4;
const int MOTOR_ESQUERDO_IN4 = 5;
const int MOTOR_ESQUERDO_ENB = 11; // Pino PWM para controle de velocidade do motor esquerdo

// Parâmetros de Comportamento
const int ESPERA_INICIAL_SEGUNDOS = 2;
const int AVANCO_INICIAL_MS = 500; // Avança por meio segundo
const int DISTANCIA_MAXIMA_CM = 50; // Distância máxima para detectar um oponente
const bool MOVIMENTO_DE_DEFESA_ATIVADO = true;
const int LIMITE_SEM_ALTERACAO_CM = 5; // Se a distância mudar menos que isso, aciona a defesa
const int TEMPO_RE_DEFESA_MS = 400;
const int TEMPO_GIRO_DEFESA_MS = 300;
const int TEMPO_PROCURA_GIRO_MS = 250; // Tempo que ele gira no próprio eixo procurando
const int TEMPO_PROCURA_AVANCO_MS = 300; // Tempo que ele avança se não achar nada

// ---+---===[ FIM DAS CONFIGURAÇÕES ]===---+---

// ---+---===[ PROTÓTIPOS DE FUNÇÃO ]===---+---
void controlMotor(int in1Pin, int in2Pin, int enablePin, int direction, int speed);
void avancar(int speed = 200);
void re(int speed = 200);
void girarDireita(int speed = 200);
void girarNoEixo(int speed = 200);
void pararMotores();
long lerDistanciaCm();
void movimentoDeDefesa();
void procurarOponente();
// ---+---===[ FIM DOS PROTÓTIPOS ]===---+---

long distanciaAnterior = 0;

void setup() {
  // Inicializa a comunicação serial para depuração
  Serial.begin(9600);

  // Configura os pinos do motor como saída
  pinMode(MOTOR_DIREITO_IN1, OUTPUT);
  pinMode(MOTOR_DIREITO_IN2, OUTPUT);
  pinMode(MOTOR_DIREITO_ENA, OUTPUT); // Adicionado
  pinMode(MOTOR_ESQUERDO_IN3, OUTPUT);
  pinMode(MOTOR_ESQUERDO_IN4, OUTPUT);
  pinMode(MOTOR_ESQUERDO_ENB, OUTPUT); // Adicionado

  // Configura os pinos do sensor ultrassônico
  pinMode(PINO_TRIG, OUTPUT);
  pinMode(PINO_ECHO, INPUT);

  // Para os motores no início para garantir
  pararMotores();

  Serial.println("Robô de Sumô Iniciando...");
  Serial.print("Aguardando ");
  Serial.print(ESPERA_INICIAL_SEGUNDOS);
  Serial.println(" segundos...");

  // Espera inicial configurável
  delay(ESPERA_INICIAL_SEGUNDOS * 1000);

  Serial.println("Avanço inicial!");
  avancar();
  delay(AVANCO_INICIAL_MS);
  pararMotores();
  distanciaAnterior = lerDistanciaCm();
}

void loop() {
  long distanciaAtual = lerDistanciaCm();
  Serial.print("Distância: ");
  Serial.print(distanciaAtual);
  Serial.println(" cm");

  // Se detecta um oponente dentro do alcance
  if (distanciaAtual > 0 && distanciaAtual <= DISTANCIA_MAXIMA_CM) {
    // Lógica de ataque e defesa
    Serial.println("Oponente detectado! Atacando.");

    // Verifica se o movimento de defesa deve ser ativado
    if (MOVIMENTO_DE_DEFESA_ATIVADO && abs(distanciaAtual - distanciaAnterior) < LIMITE_SEM_ALTERACAO_CM) {
        Serial.println("Sem alteração significativa de distância. Ativando defesa!");
        movimentoDeDefesa();
    } else {
        // Se não, avança para atacar
        avancar();
    }
    // Atualiza a distância anterior para a próxima iteração
    distanciaAnterior = distanciaAtual;
  } else {
    // Lógica de procura
    Serial.println("Nenhum oponente. Procurando...");
    procurarOponente();
  }

  delay(50); // Pequeno delay para não sobrecarregar o loop
}

// ---+---===[ FUNÇÕES DE MOVIMENTO ]===---+---

// Define motor directions for clarity
#define MOTOR_FORWARD  1
#define MOTOR_BACKWARD 0
#define MOTOR_STOP     2

// Função auxiliar para controlar um único motor com velocidade (PWM)
// in1Pin e in2Pin são os pinos de controle de direção da ponte H para um motor
// enablePin é o pino PWM para controle de velocidade
// direction pode ser MOTOR_FORWARD, MOTOR_BACKWARD ou MOTOR_STOP
// speed é um valor de 0 a 255 para o PWM
void controlMotor(int in1Pin, int in2Pin, int enablePin, int direction, int speed) {
  if (direction == MOTOR_FORWARD) {
    digitalWrite(in1Pin, HIGH);
    digitalWrite(in2Pin, LOW);
  } else if (direction == MOTOR_BACKWARD) {
    digitalWrite(in1Pin, LOW);
    digitalWrite(in2Pin, HIGH);
  } else { // MOTOR_STOP
    digitalWrite(in1Pin, LOW);
    digitalWrite(in2Pin, LOW);
  }
  analogWrite(enablePin, speed); // Aplica PWM ao pino de habilitação
}

void avancar(int speed) {
  controlMotor(MOTOR_DIREITO_IN1, MOTOR_DIREITO_IN2, MOTOR_DIREITO_ENA, MOTOR_FORWARD, speed);
  controlMotor(MOTOR_ESQUERDO_IN3, MOTOR_ESQUERDO_IN4, MOTOR_ESQUERDO_ENB, MOTOR_FORWARD, speed);
}

void re(int speed) {
  controlMotor(MOTOR_DIREITO_IN1, MOTOR_DIREITO_IN2, MOTOR_DIREITO_ENA, MOTOR_BACKWARD, speed);
  controlMotor(MOTOR_ESQUERDO_IN3, MOTOR_ESQUERDO_IN4, MOTOR_ESQUERDO_ENB, MOTOR_BACKWARD, speed);
}

void girarDireita(int speed) {
  controlMotor(MOTOR_DIREITO_IN1, MOTOR_DIREITO_IN2, MOTOR_DIREITO_ENA, MOTOR_BACKWARD, speed); // Motor direito para trás
  controlMotor(MOTOR_ESQUERDO_IN3, MOTOR_ESQUERDO_IN4, MOTOR_ESQUERDO_ENB, MOTOR_FORWARD, speed); // Motor esquerdo para frente
}

void girarNoEixo(int speed) {
    // Gira para a direita no próprio eixo
    girarDireita(speed);
}


void pararMotores() {
  controlMotor(MOTOR_DIREITO_IN1, MOTOR_DIREITO_IN2, MOTOR_DIREITO_ENA, MOTOR_STOP, 0);
  controlMotor(MOTOR_ESQUERDO_IN3, MOTOR_ESQUERDO_IN4, MOTOR_ESQUERDO_ENB, MOTOR_STOP, 0);
}

// ---+---===[ FUNÇÕES DO SENSOR ]===---+---

long lerDistanciaCm() {
  // Gera um pulso de 10 microssegundos no pino de trigger
  digitalWrite(PINO_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PINO_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PINO_TRIG, LOW);

  // Lê o tempo que o pino echo ficou em HIGH
  long duracao = pulseIn(PINO_ECHO, HIGH);

  // Calcula a distância em centímetros
  // Velocidade do som = 343 m/s ou 29.1 microssegundos por cm
  // A distância é de ida e volta, então dividimos por 2
  return duracao / 29 / 2;
}

// ---+---===[ FUNÇÕES DE COMPORTAMENTO ]===---+---

void movimentoDeDefesa() {
  Serial.println("Executando manobra de defesa...");
  // Dá ré virando para a direita
  // Motor direito parado, motor esquerdo para trás
  controlMotor(MOTOR_DIREITO_IN1, MOTOR_DIREITO_IN2, MOTOR_DIREITO_ENA, MOTOR_STOP, 0); // Motor direito parado
  controlMotor(MOTOR_ESQUERDO_IN3, MOTOR_ESQUERDO_IN4, MOTOR_ESQUERDO_ENB, MOTOR_BACKWARD, 200); // Motor esquerdo para trás com velocidade
  delay(TEMPO_GIRO_DEFESA_MS);

  // Continua dando ré por um tempo
  re();
  delay(TEMPO_RE_DEFESA_MS);

  // Para e se prepara para procurar novamente
  pararMotores();
  delay(200);
}

void procurarOponente() {
    Serial.println("Procurando oponente girando no eixo...");
    pararMotores();
    delay(100); // Pequena pausa antes de girar

    unsigned long startTime = millis();
    const unsigned long searchDuration = 2000; // Gira por 2 segundos procurando

    girarNoEixo(); // Começa a girar

    while (millis() - startTime < searchDuration) {
        long distancia = lerDistanciaCm();
        if (distancia > 0 && distancia <= DISTANCIA_MAXIMA_CM) {
            Serial.println("Oponente encontrado durante o giro! Avançando.");
            avancar(); // Avança imediatamente
            return; // Sai da função de procura
        }
        // Pequeno delay para não sobrecarregar o sensor e o loop
        delay(20); // Verifica a cada 20ms
    }

    // Se chegou aqui, não encontrou nada durante o giro completo
    Serial.println("Nenhum oponente encontrado após giro. Virando e avançando um pouco.");
    pararMotores();
    delay(100);
    girarDireita();
    delay(200); // Gira um pouco para a direita
    avancar();
    delay(TEMPO_PROCURA_AVANCO_MS); // Avança um pouco
    pararMotores();
}
