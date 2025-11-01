/*
  PROJETO: Robô de Sumô Autônomo

  OBJETIVO:
  Este código controla um robô de sumô autônomo projetado para competir em um
  ringue. O robô usa sensores para detectar oponentes e as bordas do ringue,
  tomando decisões para atacar, defender e evitar cair.

  COMPONENTES NECESSÁRIOS:
  - Placa Arduino (Uno, Nano, etc.)
  - Ponte H L298N (ou similar) para controle dos motores
  - 2 Motores DC com caixa de redução (para as rodas)
  - 1 Sensor Ultrassônico HC-SR04 (para detectar oponentes)
  - 2 Sensores Infravermelhos (IR) de linha (um frontal, um traseiro, para
  detectar a borda do ringue)
  - Bateria (para alimentar o Arduino e os motores)
  - Fios jumper, protoboard (opcional)

  COMO MONTAR O ROBÔ (Conexões Elétricas):

  1.  **ALIMENTAÇÃO DA PONTE H (L298N):**
      - Conecte o pino VIN (ou 12V) da Ponte H ao polo positivo da sua bateria.
      - Conecte o pino GND da Ponte H ao polo negativo da sua bateria E ao GND
  do Arduino.
      - (Opcional) Se sua bateria for > 9V, conecte o pino 5V da Ponte H ao 5V
  do Arduino para alimentar o Arduino (verifique a documentação da sua Ponte H).

  2.  **MOTORES DC:**
      - Conecte os dois fios de um motor aos pinos OUT1 e OUT2 da Ponte H.
      - Conecte os dois fios do outro motor aos pinos OUT3 e OUT4 da Ponte H.
      (Se um motor girar para o lado errado, inverta a conexão dos fios dele na
  Ponte H).

  3.  **CONTROLE DOS MOTORES (Ponte H para Arduino):**
      - **Motor Direito:**
          - L298N IN1  -> Arduino Digital 2 (MOTOR_DIREITO_IN1)
          - L298N IN2  -> Arduino Digital 3 (MOTOR_DIREITO_IN2)
          - L298N ENA  -> Arduino Digital 6 (MOTOR_DIREITO_ENA - Pino PWM!)
      - **Motor Esquerdo:**
          - L298N IN3  -> Arduino Digital 4 (MOTOR_ESQUERDO_IN3)
          - L298N IN4  -> Arduino Digital 5 (MOTOR_ESQUERDO_IN4)
          - L298N ENB  -> Arduino Digital 11 (MOTOR_ESQUERDO_ENB - Pino PWM!)

  4.  **SENSOR ULTRASSÔNICO HC-SR04:**
      - HC-SR04 VCC -> Arduino 5V
      - HC-SR04 GND -> Arduino GND
      - HC-SR04 Trig -> Arduino Digital 9 (PINO_TRIG)
      - HC-SR04 Echo -> Arduino Digital 10 (PINO_ECHO)

  5.  **SENSORES INFRAVERMELHOS (IR) de Linha:**
      - **Ambos os Sensores IR:**
          - IR VCC -> Arduino 5V
          - IR GND -> Arduino GND
      - **Sensor IR Frontal:**
          - IR OUT (D0) -> Arduino Digital 7 (IR_FRONT_SENSOR_PIN)
      - **Sensor IR Traseiro:**
          - IR OUT (D0) -> Arduino Digital 8 (IR_REAR_SENSOR_PIN)
      (Verifique a documentação do seu sensor IR: alguns podem precisar de um
  resistor pull-up/down ou ter uma saída invertida - HIGH para linha, LOW para
  fora da linha, ou vice-versa. O código assume HIGH para linha.)
      **IMPORTANTE:**
      - **Ajuste do Potenciômetro:** Muitos sensores IR possuem um pequeno potenciômetro (parafuso de ajuste) para calibrar a sensibilidade. Gire-o com uma chave de fenda pequena até que o sensor detecte a linha branca de forma confiável (geralmente, um LED no sensor acende/apaga).
      - **Tipo de Sensor:** Existem sensores IR apenas digitais (saída HIGH/LOW) e outros que também possuem saída analógica. Este código utiliza a saída digital (D0) do sensor.

  COMO USAR O CÓDIGO:
  1.  Conecte seu Arduino ao computador.
  2.  Abra este arquivo (.ino) no IDE do Arduino.
  3.  Verifique se as conexões dos pinos no código (seção CONFIGURAÇÕES)
  correspondem à sua montagem.
  4.  Faça o upload do código para o seu Arduino.
  5.  Desconecte o Arduino do computador e ligue a bateria do robô.
  6.  O robô aguardará o tempo inicial e começará a operar!

  DICAS:
  - Use a "Serial Monitor" do IDE do Arduino para ver as mensagens de depuração
  e entender o que o robô está fazendo.
  - Ajuste os parâmetros na seção CONFIGURAÇÕES para otimizar o comportamento do
  seu robô.
  - Experimente diferentes manobras e tempos para encontrar a melhor estratégia!
*/

/*
  --- Como este código está organizado? ---

  1.  **Configurações (Pinos e Parâmetros):** Onde definimos as conexões do Arduino e ajustamos o comportamento do robô.
  2.  **Protótipos de Função:** Declarações das funções para que o compilador as conheça.
  3.  **Variáveis Globais:** Variáveis que guardam informações importantes durante todo o funcionamento do robô.
  4.  **Função `setup()`:** O que o robô faz uma única vez quando liga (prepara os pinos e sensores).
  5.  **Loop Principal (`loop()`):** O que o robô faz repetidamente (a "mente" do robô, onde ele toma decisões).
  6.  **Funções de Movimento:** Como fazer o robô andar, girar e parar.
  7.  **Funções de Sensores:** Como ler a distância do ultrassônico.
  8.  **Funções de Comportamento:** As estratégias do robô (defesa, busca, evasão de borda).

  --- Como Ajustar os Parâmetros do Robô? ---

  Vá para a seção "CONFIGURAÇÕES" (logo abaixo).
  Você pode mudar os números depois do `const int` ou `const bool` para alterar
  o comportamento do robô sem precisar mexer na lógica principal.
  Por exemplo, para o robô esperar mais tempo antes de começar, mude o valor de `ESPERA_INICIAL_SEGUNDOS`.
  Experimente mudar:
  - `DISTANCIA_MAXIMA_CM`: Para o robô detectar oponentes mais perto ou mais longe.
  - `MOVIMENTO_DE_DEFESA_ATIVADO`: Para ligar ou desligar a defesa quando travado.
  - `TEMPO_RE_DEFESA_MS`, `TEMPO_GIRO_DEFESA_MS`: Para ajustar a manobra de defesa.
  - `searchDuration` dentro de `procurarOponente()`: Para mudar quanto tempo ele gira procurando.

  --- Fim da Documentação Principal ---
*/

// ---+---===[ 1. CONFIGURAÇÕES (Pinos e Parâmetros) ]===---+---
// Esta seção define os pinos do Arduino conectados aos componentes
// e os parâmetros que controlam o comportamento do robô. Você pode
// ajustar esses valores para otimizar o desempenho do seu robô!

// Pinos do Sensor Ultrassônico HC-SR04
// O sensor ultrassônico mede distâncias usando som.
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
const int MOTOR_DIREITO_IN1 =
    2; // Pino de controle de direção 1 do motor direito.
const int MOTOR_DIREITO_IN2 =
    3; // Pino de controle de direção 2 do motor direito.
const int MOTOR_DIREITO_ENA =
    6; // Pino de Habilitação (Enable) do motor direito. Conecte a um pino PWM
       // do Arduino para controlar a velocidade.

// Pinos da Ponte H L298N para o Motor Esquerdo
const int MOTOR_ESQUERDO_IN3 =
    4; // Pino de controle de direção 1 do motor esquerdo.
const int MOTOR_ESQUERDO_IN4 =
    5; // Pino de controle de direção 2 do motor esquerdo.
const int MOTOR_ESQUERDO_ENB =
    11; // Pino de Habilitação (Enable) do motor esquerdo. Conecte a um pino PWM
        // do Arduino para controlar a velocidade.

// Parâmetros de Comportamento do Robô
// Estes valores definem como o robô se move e reage.
const int ESPERA_INICIAL_SEGUNDOS =
    2; // Tempo em segundos que o robô espera antes de começar a se mover (para
       // o juiz iniciar a partida).
const int AVANCO_INICIAL_MS =
    500; // Tempo em milissegundos que o robô avança no início da partida.
const int DISTANCIA_MAXIMA_CM =
    50; // Distância máxima em cm para considerar um oponente detectado pelo
        // sensor ultrassônico.
const int MIN_DISTANCE_CM =
    5; // Distância mínima em cm. Leituras abaixo disso serão tratadas como esta
       // distância (zona morta do sensor).
const bool MOVIMENTO_DE_DEFESA_ATIVADO =
    true; // Define se o robô deve usar a manobra de defesa quando estiver
          // travado.
const int LIMITE_SEM_ALTERACAO_CM =
    5; // Se a distância para o oponente mudar menos que isso, o robô considera
       // que está travado e ativa a defesa.
const int TEMPO_RE_DEFESA_MS =
    400; // Tempo em milissegundos que o robô dá ré durante a manobra de defesa.
const int TEMPO_GIRO_DEFESA_MS =
    300; // Tempo em milissegundos que o robô gira durante a manobra de defesa.
const int TEMPO_PROCURA_GIRO_MS =
    250; // Tempo em milissegundos que o robô gira no próprio eixo ao procurar
         // um oponente (não mais usado diretamente, mas mantido para
         // referência).
const int TEMPO_PROCURA_AVANCO_MS =
    300; // Tempo em milissegundos que o robô avança um pouco se não encontrar
         // oponente após girar.

// ---+---===[ FIM DAS CONFIGURAÇÕES ]===---+---

// ---+---===[ 2. PROTÓTIPOS DE FUNÇÃO ]===---+---
// Protótipos informam ao compilador sobre a existência das funções
// antes que elas sejam realmente definidas mais abaixo no código.
// Isso é necessário para que o Arduino saiba que essas funções existem
// quando são chamadas em 'setup()' ou 'loop()'.
void controlMotor(int in1Pin, int in2Pin, int enablePin, int direction,
                  int speed);
void avancar(int speed = 200);
void re(int speed = 200);
void girarDireita(int speed = 200);
void girarNoEixo(int speed = 200);
void pararMotores();
long lerDistanciaCm();
void movimentoDeDefesa();
void procurarOponente();
bool checkRingBoundary();
// ---+---===[ FIM DOS PROTÓTIPOS ]===---+---

// ---+---===[ 3. VARIÁVEIS GLOBAIS ]===---+---
// Estas variáveis guardam informações importantes que são usadas em diferentes partes do código.
long distanciaAnterior = 0; // Armazena a última distância lida do oponente para verificar se o robô está travado.
bool isAttacking = false; // Variável booleana que indica se o robô está atualmente em modo de ataque (detectou um oponente).

// ---+---===[ 4. FUNÇÃO `setup()` ]===---+---
// Esta função roda APENAS UMA VEZ quando o Arduino é ligado ou resetado.
// É onde preparamos tudo para o robô começar a trabalhar.
void setup() {
  // Inicia a comunicação serial para enviar mensagens de depuração para o
  // computador. Abra o "Serial Monitor" no IDE do Arduino para ver essas
  // mensagens.
  Serial.begin(9600);

  // --- Configuração dos Pinos de Saída (OUTPUT) para os Motores ---
  // Estes pinos enviam sinais para a Ponte H para controlar os motores.
  pinMode(MOTOR_DIREITO_IN1, OUTPUT);
  pinMode(MOTOR_DIREITO_IN2, OUTPUT);
  pinMode(MOTOR_DIREITO_ENA,
          OUTPUT); // Pino PWM para controle de velocidade do motor direito.
  pinMode(MOTOR_ESQUERDO_IN3, OUTPUT);
  pinMode(MOTOR_ESQUERDO_IN4, OUTPUT);
  pinMode(MOTOR_ESQUERDO_ENB,
          OUTPUT); // Pino PWM para controle de velocidade do motor esquerdo.

  // --- Configuração dos Pinos de Entrada (INPUT) para os Sensores ---
  // Estes pinos recebem sinais dos sensores.
  pinMode(PINO_TRIG,
          OUTPUT); // O pino Trig do ultrassônico é uma SAÍDA (envia som).
  pinMode(PINO_ECHO,
          INPUT); // O pino Echo do ultrassônico é uma ENTRADA (recebe som).
  pinMode(IR_FRONT_SENSOR_PIN, INPUT); // Sensor IR frontal: ENTRADA.
  pinMode(IR_REAR_SENSOR_PIN, INPUT);  // Sensor IR traseiro: ENTRADA.

  // Garante que os motores estejam parados no início.
  pararMotores();

  Serial.println("Robô de Sumô Iniciando...");
  Serial.print("Aguardando ");
  Serial.print(ESPERA_INICIAL_SEGUNDOS);
  Serial.println(" segundos...");

  // Espera inicial configurável antes de qualquer movimento.
  // Isso dá tempo para o juiz iniciar a partida ou para você se afastar.
  delay(ESPERA_INICIAL_SEGUNDOS * 1000);

  Serial.println("Avanço inicial!");
  // O robô avança um pouco no início para sair da posição inicial.
  avancar();
  delay(AVANCO_INICIAL_MS);
  pararMotores();
  // Registra a distância inicial para o oponente (pode ser o ar, se não houver
  // ninguém na frente).
  distanciaAnterior = lerDistanciaCm();
}
// ---+---===[ 5. LOOP PRINCIPAL (`loop()`) ]===---+---
// Esta função roda INFINITAMENTE, uma vez após a outra, depois que o 'setup()' termina.
// É a "mente" do robô, onde ele toma decisões continuamente.
void loop() {
  // --- PRIORIDADE MÁXIMA: Verificação da Borda do Ringue ---
  // Esta é a primeira coisa que o robô verifica em cada ciclo.
  // Se uma borda for detectada, o robô executa uma manobra de recuperação
  // e o 'loop()' é reiniciado imediatamente para garantir que a borda
  // seja tratada com a maior prioridade.
  if (checkRingBoundary()) {
    return; // Se a borda foi detectada e tratada, reinicia o loop
            // imediatamente.
  }

  // --- Leitura do Sensor Ultrassônico ---
  // Mede a distância até o objeto mais próximo à frente do robô.
  long distanciaAtual = lerDistanciaCm();
  Serial.print("Distância: ");
  Serial.print(distanciaAtual);
  Serial.println(" cm");

  // --- Lógica Principal: Oponente Detectado vs. Nenhum Oponente ---
  // Se detecta um oponente dentro do alcance (incluindo os muito próximos)
  if (distanciaAtual <= DISTANCIA_MAXIMA_CM) {
    // --- Oponente Detectado: Modo de Ataque ---
    // Se o robô não estava atacando, mas agora detectou um oponente, ele entra
    // no modo de ataque.
    if (!isAttacking) {
      Serial.println("Oponente detectado, iniciando ataque.");
      isAttacking = true;
    }

    // --- Verificação de Defesa (Robô Travado?) ---
    // Se o mecanismo de defesa estiver ativado E a distância para o oponente
    // não mudou significativamente (indicando que o robô pode estar travado),
    // ele executa a manobra de defesa.
    if (MOVIMENTO_DE_DEFESA_ATIVADO &&
        abs(distanciaAtual - distanciaAnterior) < LIMITE_SEM_ALTERACAO_CM) {
      Serial.println(
          "Sem alteração significativa de distância. Ativando defesa!");
      movimentoDeDefesa();
    } else {
      // Se não está travado, continua avançando para empurrar o oponente.
      avancar();
    }
    // Atualiza a distância anterior para a próxima iteração, para verificar
    // travamento.
    distanciaAnterior = distanciaAtual;
  } else {
    // --- Nenhum Oponente Detectado: Modo de Busca ou Recuperação ---
    // Se o robô estava atacando, mas agora perdeu o oponente de vista,
    // ele executa uma manobra de recuperação (dar ré) e inicia a busca.
    if (isAttacking) {
      Serial.println("Oponente perdido! Dando ré e procurando.");
      re();       // Imediatamente dá ré.
      delay(300); // Dá ré por um curto período.
      pararMotores();
      isAttacking = false; // Não está mais atacando.
      procurarOponente();  // Inicia a busca por um novo oponente.
    } else {
      // Se não está atacando e nenhum oponente está à vista, ele continua
      // procurando.
      procurarOponente();
    }
  }
  // Pequeno atraso para não sobrecarregar o processador e permitir leituras
  // estáveis.
  delay(50);
}
// ---+---===[ 6. FUNÇÕES DE MOVIMENTO ]===---+---
// Estas funções controlam o movimento do robô, definindo a direção e velocidade
// dos motores.

// Define as direções de movimento para maior clareza no código.
#define MOTOR_FORWARD 1  // Mover para frente
#define MOTOR_BACKWARD 0 // Mover para trás
#define MOTOR_STOP 2     // Parar o motor

// Função auxiliar para controlar um único motor com velocidade (PWM).
// Ela recebe os pinos de controle da Ponte H para um motor, o pino de
// habilitação (PWM), a direção desejada e a velocidade (0-255).
void controlMotor(int in1Pin, int in2Pin, int enablePin, int direction,
                  int speed) {
  if (direction == MOTOR_FORWARD) {
    digitalWrite(in1Pin, HIGH); // Define a direção para frente.
    digitalWrite(in2Pin, LOW);
  } else if (direction == MOTOR_BACKWARD) {
    digitalWrite(in1Pin, LOW); // Define a direção para trás.
    digitalWrite(in2Pin, HIGH);
  } else { // MOTOR_STOP
    digitalWrite(in1Pin,
                 LOW); // Para o motor, desligando ambos os pinos de direção.
    digitalWrite(in2Pin, LOW);
  }
  analogWrite(enablePin, speed); // Aplica o valor PWM (velocidade) ao pino de
                                 // habilitação do motor.
}

// Faz o robô avançar.
// A velocidade padrão é 200, mas pode ser ajustada ao chamar a função.
void avancar(int speed) {
  controlMotor(MOTOR_DIREITO_IN1, MOTOR_DIREITO_IN2, MOTOR_DIREITO_ENA,
               MOTOR_FORWARD, speed);
  controlMotor(MOTOR_ESQUERDO_IN3, MOTOR_ESQUERDO_IN4, MOTOR_ESQUERDO_ENB,
               MOTOR_FORWARD, speed);
}

// Faz o robô dar ré.
// A velocidade padrão é 200.
void re(int speed) {
  controlMotor(MOTOR_DIREITO_IN1, MOTOR_DIREITO_IN2, MOTOR_DIREITO_ENA,
               MOTOR_BACKWARD, speed);
  controlMotor(MOTOR_ESQUERDO_IN3, MOTOR_ESQUERDO_IN4, MOTOR_ESQUERDO_ENB,
               MOTOR_BACKWARD, speed);
}

// Faz o robô girar para a direita (um motor para trás, outro para frente).
// A velocidade padrão é 200.
void girarDireita(int speed) {
  controlMotor(MOTOR_DIREITO_IN1, MOTOR_DIREITO_IN2, MOTOR_DIREITO_ENA,
               MOTOR_BACKWARD, speed); // Motor direito para trás.
  controlMotor(MOTOR_ESQUERDO_IN3, MOTOR_ESQUERDO_IN4, MOTOR_ESQUERDO_ENB,
               MOTOR_FORWARD, speed); // Motor esquerdo para frente.
}

// Faz o robô girar no próprio eixo para a direita.
// A velocidade padrão é 200.
void girarNoEixo(int speed) {
  girarDireita(speed); // Reutiliza a função girarDireita para girar no eixo.
}

// Para ambos os motores do robô.
void pararMotores() {
  controlMotor(MOTOR_DIREITO_IN1, MOTOR_DIREITO_IN2, MOTOR_DIREITO_ENA,
               MOTOR_STOP, 0); // Para o motor direito (velocidade 0).
  controlMotor(MOTOR_ESQUERDO_IN3, MOTOR_ESQUERDO_IN4, MOTOR_ESQUERDO_ENB,
               MOTOR_STOP, 0); // Para o motor esquerdo (velocidade 0).
}

// ---+---===[ 7. FUNÇÕES DE SENSORES ]===---+---
// Esta seção contém funções para ler os dados dos sensores.

// Função para ler a distância em centímetros usando o sensor ultrassônico
// HC-SR04. Retorna a distância em cm, ou um valor especial se a leitura for
// inválida ou muito próxima.
long lerDistanciaCm() {
  // Garante que o pino Trig esteja LOW por um curto período para limpar
  // leituras anteriores.
  digitalWrite(PINO_TRIG, LOW);
  delayMicroseconds(2);
  // Envia um pulso HIGH de 10 microssegundos no pino Trig.
  // Este pulso faz o sensor emitir um som ultrassônico.
  digitalWrite(PINO_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PINO_TRIG, LOW);

  // Mede a duração do pulso HIGH no pino Echo.
  // O pino Echo fica HIGH pelo tempo que o som leva para ir e voltar.
  // O 'pulseIn' tem um timeout para evitar que o programa trave se não houver
  // eco.
  long duracao =
      pulseIn(PINO_ECHO, HIGH, 30000); // Timeout de 30ms (aprox. 5m).

  // Calcula a distância em centímetros.
  // A velocidade do som no ar é de aproximadamente 343 metros por segundo, ou
  // 0.0343 cm/microssegundo. Como o som viaja e volta, dividimos a duração por
  // 2 e depois pelo tempo que leva para percorrer 1 cm (29 microssegundos).
  long distancia = duracao / 29 / 2;

  // --- Tratamento de Leituras Inválidas ou Extremas ---
  // Se a duração for 0, significa que não houve eco (objeto muito longe ou erro
  // na leitura). Ou se a distância calculada for muito grande (erro de leitura
  // do sensor, HC-SR04 geralmente vai até 400cm).
  if (duracao == 0 || distancia > 400) {
    return DISTANCIA_MAXIMA_CM + 1; // Retorna um valor que indica "fora de
                                    // alcance" para a lógica principal.
  }
  // Se a distância for muito pequena (dentro da zona morta do sensor, abaixo de
  // MIN_DISTANCE_CM), ainda é um objeto. Retornamos a distância mínima
  // configurada para indicar que está muito próximo.
  if (distancia < MIN_DISTANCE_CM) {
    return MIN_DISTANCE_CM; // Trata como se estivesse na distância mínima.
  }
  return distancia; // Retorna a distância calculada em centímetros.
}

// ---+---===[ 8. FUNÇÕES DE COMPORTAMENTO ]===---+---
// Estas funções definem a inteligência do robô, como ele reage a oponentes e ao ambiente.

void movimentoDeDefesa() {
  Serial.println("Executando manobra de defesa...");
  // Dá ré virando para a direita
  // Motor direito parado, motor esquerdo para trás
  controlMotor(MOTOR_DIREITO_IN1, MOTOR_DIREITO_IN2, MOTOR_DIREITO_ENA,
               MOTOR_STOP, 0); // Motor direito parado
  controlMotor(MOTOR_ESQUERDO_IN3, MOTOR_ESQUERDO_IN4, MOTOR_ESQUERDO_ENB,
               MOTOR_BACKWARD, 200); // Motor esquerdo para trás com velocidade
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
      return;    // Sai da função de procura
    }
    // Pequeno delay para não sobrecarregar o sensor e o loop
    delay(20); // Verifica a cada 20ms
  }

  // Se chegou aqui, não encontrou nada durante o giro completo
  Serial.println(
      "Nenhum oponente encontrado após giro. Virando e avançando um pouco.");
  pararMotores();
  delay(100);
  girarDireita();
  delay(200); // Gira um pouco para a direita
  avancar();
  delay(TEMPO_PROCURA_AVANCO_MS); // Avança um pouco
  pararMotores();
}

bool checkRingBoundary() {
  // Leitura bruta dos sensores IR.
  bool frontSensorRaw = digitalRead(IR_FRONT_SENSOR_PIN);
  bool rearSensorRaw = digitalRead(IR_REAR_SENSOR_PIN);

  // Aplica a lógica de inversão: se INVERTER_IR_X_SENSOR for true, inverte o sinal bruto.
  // Assume que 'true' (HIGH) significa borda detectada após a inversão, se necessário.
  bool frontBoundaryDetected = INVERTER_IR_FRONT_SENSOR ? !frontSensorRaw : frontSensorRaw;
  bool rearBoundaryDetected = INVERTER_IR_REAR_SENSOR ? !rearSensorRaw : rearSensorRaw;

  if (frontBoundaryDetected) {
    Serial.println("Limite frontal do ringue detectado! Manobra de evasão.");
    pararMotores(); // Para imediatamente.
    delay(100);
    re(); // Dá ré para sair da borda.
    delay(500); // Dá ré por meio segundo.
    pararMotores(); // Para novamente.
    delay(100);
    girarDireita(200); // Gira para a direita para se afastar da borda.
    delay(800); // Gira por um tempo.
    pararMotores(); // Para após o giro.
    return true; // Indica que a borda foi detectada e tratada.
  } 
  // --- Lógica para Borda Traseira Detectada ---
  else if (rearBoundaryDetected) {
    Serial.println("Limite traseiro do ringue detectado! Defesa contra saída.");
    pararMotores(); // Para imediatamente.
    delay(100);
    avancar(255); // Empurra para frente na velocidade máxima (255).
    delay(700); // Empurra por um tempo.
    pararMotores(); // Para.
    delay(100);
    girarDireita(200); // Gira para a direita para reorientar, caso ainda esteja perto da borda.
    delay(400);
    pararMotores(); // Para após o giro.
    return true; // Indica que a borda foi detectada e tratada.
  }
  return false; // Nenhuma borda detectada.
}
