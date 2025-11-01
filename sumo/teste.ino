// Pinos do Sensor Ultrassônico HC-SR04
// Este sensor mede a distância até o oponente.
const int PINO_TRIG = 9;  // Pino Trigger: Envia o pulso de som.
const int PINO_ECHO = 10; // Pino Echo: Recebe o pulso de som refletido.

// Pinos dos Sensores Infravermelhos (IR) de Linha
// Estes sensores detectam a linha branca na borda do ringue.
const int IR_FRONT_SENSOR_PIN = 7; // Sensor IR frontal: Ajuda a evitar que o robô caia para frente.
const bool INVERTER_IR_FRONT_SENSOR = false; // Defina como 'true' se o sensor frontal retornar LOW na linha e HIGH fora dela.
const int IR_REAR_SENSOR_PIN = 8;  // Sensor IR traseiro: Ajuda a evitar que o robô seja empurrado para fora por trás.
const bool INVERTER_IR_REAR_SENSOR = false;  // Defina como 'true' se o sensor traseiro retornar LOW na linha e HIGH fora dela.

// Pinos da Ponte H L298N para o Motor Direito
// A Ponte H controla a direção e a velocidade dos motores.
const int MOTOR_DIREITO_IN1 = 2; // Pino de controle de direção 1 do motor direito.
const int MOTOR_DIREITO_IN2 = 3; // Pino de controle de direção 2 do motor direito.

// Pinos da Ponte H L298N para o Motor Esquerdo
const int MOTOR_ESQUERDO_IN3 = 4; // Pino de controle de direção 1 do motor esquerdo.
const int MOTOR_ESQUERDO_IN4 = 5; // Pino de controle de direção 2 do motor esquerdo.

void setup() {
  // Inicializa a comunicação serial para depuração.
  Serial.begin(9600);

  // Configura os pinos dos motores como saída.
  pinMode(MOTOR_DIREITO_IN1, OUTPUT);
  pinMode(MOTOR_DIREITO_IN2, OUTPUT);
  pinMode(MOTOR_ESQUERDO_IN3, OUTPUT);
  pinMode(MOTOR_ESQUERDO_IN4, OUTPUT);

  // Configura os pinos dos sensores IR como entrada.
  pinMode(IR_FRONT_SENSOR_PIN, INPUT);
  pinMode(IR_REAR_SENSOR_PIN, INPUT);

  // Configura os pinos do sensor ultrassônico.
  pinMode(PINO_TRIG, OUTPUT);
  pinMode(PINO_ECHO, INPUT);

  Serial.println("Setup concluído. Motores e sensores inicializados.");
}

void loop() {
  // Gira os motores para frente indefinidamente.
  digitalWrite(MOTOR_DIREITO_IN1, HIGH);
  digitalWrite(MOTOR_DIREITO_IN2, LOW);
  digitalWrite(MOTOR_ESQUERDO_IN3, HIGH);
  digitalWrite(MOTOR_ESQUERDO_IN4, LOW);

  // Lê os valores dos sensores IR.
  int frontIR = digitalRead(IR_FRONT_SENSOR_PIN);
  int rearIR = digitalRead(IR_REAR_SENSOR_PIN);

  // Inverte a lógica se necessário.
  if (INVERTER_IR_FRONT_SENSOR) {
    frontIR = !frontIR;
  }
  if (INVERTER_IR_REAR_SENSOR) {
    rearIR = !rearIR;
  }

  // Lê a distância do sensor ultrassônico.
  digitalWrite(PINO_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PINO_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PINO_TRIG, LOW);
  long duration = pulseIn(PINO_ECHO, HIGH);
  float distance = duration * 0.034 / 2;

  // Imprime os valores dos sensores no monitor serial.
  Serial.print("Sensor IR Frontal: ");
  Serial.print(frontIR);
  Serial.print(" | Sensor IR Traseiro: ");
  Serial.print(rearIR);
  Serial.print(" | Distância (cm): ");
  Serial.println(distance);

  // Pequeno atraso para não sobrecarregar o monitor serial.
  delay(100);
}
