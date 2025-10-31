/*
  Código para Robô de Sumô Arduino
  Controla dois motores com uma Ponte H e usa um sensor ultrassônico para detectar oponentes.
*/

// ---+---===[ CONFIGURAÇÕES ]===---+---

// Pinos do Sensor Ultrassônico
const int PINO_TRIG = 9;
const int PINO_ECHO = 10;

const int IR_FRONT_SENSOR_PIN = 7; // Pino do sensor IR frontal para detecção da linha do ringue
const int IR_REAR_SENSOR_PIN = 8;  // Pino do sensor IR traseiro para detecção da linha do ringue

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
const int MIN_DISTANCE_CM = 5; // Distância mínima confiável para o sensor (objetos mais próximos que isso serão tratados como MIN_DISTANCE_CM)
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
void checkRingBoundary();
// ---+---===[ FIM DOS PROTÓTIPOS ]===---+---

long distanciaAnterior = 0;
bool isAttacking = false; // Variável para rastrear o estado de ataque

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
  pinMode(IR_FRONT_SENSOR_PIN, INPUT); // Configura o pino do sensor IR frontal como entrada
  pinMode(IR_REAR_SENSOR_PIN, INPUT);  // Configura o pino do sensor IR traseiro como entrada

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
  checkRingBoundary(); // Verifica o limite do ringue primeiro

  long distanciaAtual = lerDistanciaCm();
  Serial.print("Distância: ");
  Serial.print(distanciaAtual);
  Serial.println(" cm");

  // Se detecta um oponente dentro do alcance (incluindo os muito próximos)
  if (distanciaAtual <= DISTANCIA_MAXIMA_CM) {
    // Oponente detectado
    if (!isAttacking) { // Acabou de detectar um oponente, iniciar ataque
        Serial.println("Oponente detectado, iniciando ataque.");
        isAttacking = true;
    }

    // Lógica de ataque e defesa
    if (MOVIMENTO_DE_DEFESA_ATIVADO && abs(distanciaAtual - distanciaAnterior) < LIMITE_SEM_ALTERACAO_CM) {
        Serial.println("Sem alteração significativa de distância. Ativando defesa!");
        movimentoDeDefesa();
    } else {
        avancar(); // Continua avançando
    }
    distanciaAnterior = distanciaAtual;
  } else {
    // Nenhum oponente detectado
    if (isAttacking) { // Estava atacando, mas agora perdeu o oponente
        Serial.println("Oponente perdido! Dando ré e procurando.");
        re(); // Imediatamente dá ré
        delay(300); // Dá ré por um curto período
        pararMotores();
        isAttacking = false; // Não está mais atacando
        procurarOponente(); // Inicia a busca
    } else {
        // Não está atacando e nenhum oponente, apenas procura
        procurarOponente();
    }
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
  digitalWrite(PINO_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PINO_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PINO_TRIG, LOW);

  long duracao = pulseIn(PINO_ECHO, HIGH, 30000); // Timeout de 30ms (aprox. 5m) para evitar travamento
  long distancia = duracao / 29 / 2;

  // Se duracao for 0, significa que não houve eco (objeto muito longe ou erro).
  // Ou se a distância calculada for muito grande (erro de leitura do sensor).
  if (duracao == 0 || distancia > 400) { // 400cm é um limite razoável para a maioria dos HC-SR04
      return DISTANCIA_MAXIMA_CM + 1; // Retorna um valor que indica "fora de alcance"
  }
  // Se a distância for muito pequena (dentro da zona morta do sensor), ainda é um objeto.
  // Retornamos a distância mínima configurada para indicar que está muito próximo.
  if (distancia < MIN_DISTANCE_CM) {
      return MIN_DISTANCE_CM; // Trata como se estivesse na distância mínima
  }
  return distancia;
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

void checkRingBoundary() {
  // Assume HIGH significa que a borda do ringue foi detectada
  bool frontBoundaryDetected = (digitalRead(IR_FRONT_SENSOR_PIN) == HIGH);
  bool rearBoundaryDetected = (digitalRead(IR_REAR_SENSOR_PIN) == HIGH);

  if (frontBoundaryDetected) {
    Serial.println("Limite frontal do ringue detectado! Manobra de evasão.");
    pararMotores();
    delay(100);
    re(); // Dá ré
    delay(500); // Dá ré por meio segundo
    pararMotores();
    delay(100);
    girarDireita(200); // Gira para a direita para se afastar da borda
    delay(800); // Gira por um tempo
    pararMotores();
    // Após a manobra, o robô pode retomar a busca por oponentes
    // ou simplesmente retornar ao loop principal para decidir.
  } else if (rearBoundaryDetected) {
    Serial.println("Limite traseiro do ringue detectado! Defesa contra saída.");
    // Se o sensor traseiro detecta a borda, significa que o robô está sendo empurrado para fora pela frente.
    // Empurra agressivamente para frente para voltar ao ringue.
    pararMotores();
    delay(100);
    avancar(255); // Empurra para frente na velocidade máxima
    delay(700); // Empurra por um tempo
    pararMotores();
    delay(100);
    // Opcionalmente, gira para reorientar se ainda estiver perto da borda
    girarDireita(200);
    delay(400);
    pararMotores();
  }
}
