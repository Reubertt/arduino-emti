/*
  Robô Dançarino

  Este código faz um robô dançar! Ele foi feito para ser fácil de entender e modificar.
  Aqui você vai aprender como controlar motores, servos (os "braços" do robô) e criar sua própria coreografia.

  --- COMO MONTAR O ROBÔ (Conexões Elétricas) ---

  Para que seu robô dançarino funcione, você precisa conectar os componentes ao Arduino corretamente.
  Siga este guia passo a passo:

  1.  **ALIMENTAÇÃO DO ARDUINO E COMPONENTES:**
      - Conecte o pino GND do Arduino ao polo negativo da sua bateria.
      - Conecte o pino VIN (ou o jack de alimentação) do Arduino ao polo positivo da sua bateria (se a bateria for de 7-12V).
      - Conecte o pino 5V do Arduino ao VCC (positivo) de todos os seus servos e da sua Ponte H (se ela precisar de 5V para lógica).
      - Conecte o pino GND do Arduino ao GND (negativo) de todos os seus servos e da sua Ponte H.

  2.  **MOTORES DC (Rodas) e PONTE H (Ex: L298N):**
      - **Ponte H:**
          - Conecte os pinos de alimentação da Ponte H (VCC/12V e GND) à sua bateria (VCC/12V ao positivo, GND ao negativo).
          - Se sua Ponte H tiver um pino 5V de saída, você pode usá-lo para alimentar o Arduino (conecte ao 5V do Arduino).
          - **IMPORTANTE:** Certifique-se de que o GND da Ponte H esteja conectado ao GND do Arduino!
      - **Motores:**
          - Conecte os dois fios de um motor (Motor Direito) aos pinos OUT1 e OUT2 da Ponte H.
          - Conecte os dois fios do outro motor (Motor Esquerdo) aos pinos OUT3 e OUT4 da Ponte H.
          - Se um motor girar para o lado errado, inverta a conexão dos fios dele na Ponte H.
      - **Controle da Ponte H (para o Arduino):**
          - L298N IN1  -> Arduino Digital 8 (MOTOR_DIR_1)
          - L298N IN2  -> Arduino Digital 9 (MOTOR_DIR_2)
          - L298N IN3  -> Arduino Digital 10 (MOTOR_ESQ_1)
          - L298N IN4  -> Arduino Digital 11 (MOTOR_ESQ_2)
          - (Nota: Este código não usa pinos ENA/ENB para PWM, assumindo que eles estão ligados diretamente ou que a velocidade é controlada pela duração do movimento.)

  3.  **SERVOS (Braços):**
      - **Servo do Braço Direito:**
          - Fio de Sinal (geralmente amarelo ou laranja) -> Arduino Digital 3 (SERVO_BRACO_D)
          - Fio VCC (geralmente vermelho) -> Arduino 5V
          - Fio GND (geralmente marrom ou preto) -> Arduino GND
      - **Servo do Braço Esquerdo:**
          - Fio de Sinal (geralmente amarelo ou laranja) -> Arduino Digital 2 (SERVO_BRACO_E)
          - Fio VCC (geralmente vermelho) -> Arduino 5V
          - Fio GND (geralmente marrom ou preto) -> Arduino GND

  ---

  --- Como este código está organizado? ---

  1.  **Definições de Pinos:** Onde dizemos ao Arduino quais pinos estão conectados aos motores e servos.
  2.  **Objetos Servo:** Criamos "controles" para os braços do robô.
  3.  **Constantes de Velocidade dos Servos:** Definimos o quão rápido os braços se movem.
  4.  **Constantes de Parâmetros de Dança e Movimento:** Aqui você pode ajustar facilmente os tempos e repetições da dança!
  5.  **Funções de Movimento do Robô:** Como fazer o robô andar para frente, para trás, girar e parar.
  6.  **Funções de Movimento dos Braços (Servos):** Como levantar e abaixar os braços, tanto juntos quanto individualmente, em diferentes velocidades.
  7.  **Funções de Passos de Dança:** São os "movimentos" que o robô faz, como acenar ou girar. Você pode criar os seus!
  8.  **Função Principal da Dança (`comecarDanca()`):** É onde montamos a sequência de todos os passos para criar a dança completa.
  9.  **Configuração Inicial (`setup()`):** O que o robô faz uma única vez quando liga (prepara os motores e servos).
  10. **Loop Principal (`loop()`):** O que o robô faz repetidamente depois de ligar (chama a dança e o passo final).

  --- Como Ajustar os Parâmetros da Dança? ---

  Vá para a seção "Constantes de Parâmetros de Dança e Movimento" lá em cima.
  Você pode mudar os números depois do `#define` para alterar a velocidade, duração dos movimentos e quantas vezes um passo se repete.
  Por exemplo, para o robô esperar mais tempo antes de começar a dança, mude o valor de `TEMPO_SINCRONIA_MUSICA`.

  --- Como Criar Novos Passos de Dança? ---

  1.  **Pense no movimento:** O que você quer que o robô faça? Andar, girar, mexer os braços?
  2.  **Use as funções existentes:** Olhe as seções "Funções de Movimento do Robô" e "Funções de Movimento dos Braços (Servos)". Elas são suas ferramentas!
      *   `moverFrente(velocidade)`
      *   `moverTras(velocidade)`
      *   `girarEsquerda(duracao)`
      *   `girarDireita(duracao)`
      *   `levantarBracosRapido()`
      *   `abaixarBracosLento()`
      *   `levantarBracoDireito(velocidade)`
      *   `abaixarBracoEsquerdo(velocidade)`
      *   `delay(tempo_em_ms)` para fazer o robô esperar.
  3.  **Crie uma nova função:** Copie um dos `void passoX_nomeDoPasso() { ... }` da seção "Funções de Passos de Dança".
  4.  **Dê um nome legal:** Mude `passoX_nomeDoPasso` para algo que descreva seu novo movimento, tipo `passo_rodopio()`.
  5.  **Escreva o código:** Dentro das chaves `{ }`, use as funções de movimento e `delay()` para criar seu passo.
      Exemplo:
      ```cpp
      void passo_rodopio() {
        Serial.println("Passo: Rodopiando!");
        girar360(); // Faz o robô girar 360 graus
        levantarBracosMedio(); // Levanta os braços
        delay(500);
        abaixarBracosMedio(); // Abaixa os braços
      }
      ```
  6.  **Teste seu passo:** Você pode chamar seu novo passo diretamente no `loop()` para ver se funciona como esperado.

  --- Como Juntar Tudo Numa Dança? ---

  A função `void comecarDanca() { ... }` é a sua coreografia principal.
  Para adicionar seu novo passo:
  1.  Vá para a função `comecarDanca()`.
  2.  No lugar onde você quer que seu novo passo aconteça, escreva o nome da sua nova função (ex: `passo_rodopio();`).
  3.  Você pode chamar os passos na ordem que quiser e repetir quantos quiser!

  Divirta-se programando seu robô dançarino!
*/

#include <Servo.h>

// --- Definições de Pinos ---
// Aqui dizemos ao Arduino quais pinos estão conectados aos componentes do robô.
// Se você mudar as conexões, mude os números aqui!
#define MOTOR_DIR_1 8  // Pino para controlar o motor direito (sentido 1)
#define MOTOR_DIR_2 9  // Pino para controlar o motor direito (sentido 2)
#define MOTOR_ESQ_1 10 // Pino para controlar o motor esquerdo (sentido 1)
#define MOTOR_ESQ_2 11 // Pino para controlar o motor esquerdo (sentido 2)

// Pinos para os servos dos braços
#define SERVO_BRACO_D 3 // Pino para o servo do braço direito
#define SERVO_BRACO_E 2 // Pino para o servo do braço esquerdo

// --- Objetos Servo ---
// Criamos "controles" para os nossos servos. Pense neles como o controle remoto dos braços.
Servo bracoD; // Objeto para controlar o servo do braço direito
Servo bracoE; // Objeto para controlar o servo do braço esquerdo

// --- Constantes de Velocidade dos Servos (em milissegundos de delay entre os passos) ---
// Estes números controlam o quão rápido os braços se movem.
// Um número MENOR significa movimento MAIS RÁPIDO.
#define VELOCIDADE_LENTA 15  // Movimento lento dos braços
#define VELOCIDADE_MEDIA 8   // Movimento médio dos braços
#define VELOCIDADE_RAPIDA 2  // Movimento rápido dos braços

// --- Constantes de Parâmetros de Dança e Movimento (para fácil ajuste!) ---
// Esta é a seção mais importante para personalizar a dança!
// Mude os números aqui para alterar o comportamento do robô sem mexer nas funções.

#define TEMPO_SINCRONIA_MUSICA 5000 // Tempo de espera inicial para sincronizar com a música (em milissegundos)
                                    // 1000 ms = 1 segundo

// Parâmetros para o passo de acenar repetidamente
#define ACENO_REPETICOES_PRIMEIRO 3 // Quantas vezes o robô acena no primeiro bloco da dança
#define ACENO_REPETICOES_SEGUNDO 2  // Quantas vezes o robô acena no segundo bloco da dança
#define PAUSA_ENTRE_ACENOS 200      // Pequena pausa entre cada aceno (em ms)

// Parâmetros de duração para giros e movimentos do robô
#define DURACAO_GIRO_ESQUERDA_CURTO 700 // Duração para um giro curto à esquerda (em ms)
#define DURACAO_GIRO_DIREITA_CURTO 700  // Duração para um giro curto à direita (em ms)
#define DURACAO_GIRO_180 1000           // Duração aproximada para um giro de 180 graus (em ms)
#define DURACAO_GIRO_360 2000           // Duração aproximada para um giro de 360 graus (em ms)

#define DURACAO_ANDA_FRENTE 1000 // Duração para o robô andar para frente (em ms)
#define DURACAO_ANDA_TRAS_FINAL 3000 // Duração para o robô andar para trás no passo final (em ms)

#define PAUSA_POS_MOVIMENTO 200 // Pequena pausa após alguns movimentos (em ms)

// --- Funções de Movimento do Robô ---
// Estas funções controlam as rodas do robô.

// Faz o robô andar para frente
void moverFrente(int velocidadeMotores) {
  // Lógica para fazer as rodas girarem para frente
  digitalWrite(MOTOR_DIR_1, HIGH); // Motor direito para frente
  digitalWrite(MOTOR_DIR_2, LOW);
  digitalWrite(MOTOR_ESQ_1, HIGH); // Motor esquerdo para frente
  digitalWrite(MOTOR_ESQ_2, LOW);
  // Nota: A "velocidadeMotores" aqui não é usada diretamente para PWM (controle de velocidade real),
  // mas em robôs mais avançados, você usaria analogWrite(PINO_PWM, velocidadeMotores) para isso.
  // Por enquanto, a "velocidade" é mais sobre a duração do movimento (controlada pelo delay).
}

// Faz o robô andar para trás
void moverTras(int velocidadeMotores) {
  // Lógica para fazer as rodas girarem para trás
  digitalWrite(MOTOR_DIR_1, LOW);  // Motor direito para trás
  digitalWrite(MOTOR_DIR_2, HIGH);
  digitalWrite(MOTOR_ESQ_1, LOW);  // Motor esquerdo para trás
  digitalWrite(MOTOR_ESQ_2, HIGH);
}

// Faz o robô parar completamente
void pararMotores() {
  digitalWrite(MOTOR_DIR_1, LOW);
  digitalWrite(MOTOR_DIR_2, LOW);
  digitalWrite(MOTOR_ESQ_1, LOW);
  digitalWrite(MOTOR_ESQ_2, LOW);
}

// Faz o robô girar para a esquerda por um tempo determinado
void girarEsquerda(int duracao) {
  digitalWrite(MOTOR_DIR_1, HIGH); // Roda direita para frente
  digitalWrite(MOTOR_DIR_2, LOW);
  digitalWrite(MOTOR_ESQ_1, LOW);  // Roda esquerda para trás (para girar no lugar)
  digitalWrite(MOTOR_ESQ_2, HIGH);
  delay(duracao); // Espera o tempo de giro
  pararMotores(); // Para o robô depois de girar
}

// Faz o robô girar para a direita por um tempo determinado
void girarDireita(int duracao) {
  digitalWrite(MOTOR_DIR_1, LOW);  // Roda direita para trás
  digitalWrite(MOTOR_DIR_2, HIGH);
  digitalWrite(MOTOR_ESQ_1, HIGH); // Roda esquerda para frente (para girar no lugar)
  digitalWrite(MOTOR_ESQ_2, LOW);
  delay(duracao); // Espera o tempo de giro
  pararMotores(); // Para o robô depois de girar
}

// Faz o robô girar 180 graus (usando a função girarEsquerda com uma duração pré-definida)
void girar180() {
  Serial.println("Girando 180 graus!");
  girarEsquerda(DURACAO_GIRO_180); // Usa a constante para a duração
}

// Faz o robô girar 360 graus (usando a função girarEsquerda com uma duração pré-definida)
void girar360() {
  Serial.println("Girando 360 graus!");
  girarEsquerda(DURACAO_GIRO_360); // Usa a constante para a duração
}

// --- Funções de Movimento dos Braços (Servos) ---
// Estas funções controlam os braços do robô (os servos).

// Função mais importante para os braços: move um braço para uma posição específica com uma certa velocidade.
// 'braco' é qual braço (bracoD ou bracoE),
// 'posicaoFinal' é para onde o braço deve ir (0 a 180 graus),
// 'velocidade' é o quão rápido ele vai (use VELOCIDADE_LENTA, VELOCIDADE_MEDIA, VELOCIDADE_RAPIDA).
void moverBraco(Servo &braco, int posicaoFinal, int velocidade) {
  int posicaoAtual = braco.read(); // Descobre onde o braço está agora

  if (posicaoAtual < posicaoFinal) { // Se o braço precisa subir
    for (int pos = posicaoAtual; pos <= posicaoFinal; pos++) { // Vai de onde está até a posição final
      braco.write(pos); // Move o braço um pouquinho
      delay(velocidade); // Espera um pouco (controla a velocidade)
    }
  } else { // Se o braço precisa descer
    for (int pos = posicaoAtual; pos >= posicaoFinal; pos--) { // Vai de onde está até a posição final
      braco.write(pos); // Move o braço um pouquinho
      delay(velocidade); // Espera um pouco (controla a velocidade)
    }
  }
}

// --- Funções para Mover AMBOS os Braços ---
// Usam a função 'moverBraco' para controlar os dois braços ao mesmo tempo.

void levantarBracosLento() {
  Serial.println("Levantando braços lentamente!");
  moverBraco(bracoD, 180, VELOCIDADE_LENTA);
  moverBraco(bracoE, 180, VELOCIDADE_LENTA);
}

void levantarBracosMedio() {
  Serial.println("Levantando braços em velocidade média!");
  moverBraco(bracoD, 180, VELOCIDADE_MEDIA);
  moverBraco(bracoE, 180, VELOCIDADE_MEDIA);
}

void levantarBracosRapido() {
  Serial.println("Levantando braços rapidamente!");
  moverBraco(bracoD, 180, VELOCIDADE_RAPIDA);
  moverBraco(bracoE, 180, VELOCIDADE_RAPIDA);
}

void abaixarBracosLento() {
  Serial.println("Abaixando braços lentamente!");
  moverBraco(bracoD, 0, VELOCIDADE_LENTA);
  moverBraco(bracoE, 0, VELOCIDADE_LENTA);
}

void abaixarBracosMedio() {
  Serial.println("Abaixando braços em velocidade média!");
  moverBraco(bracoD, 0, VELOCIDADE_MEDIA);
  moverBraco(bracoE, 0, VELOCIDADE_MEDIA);
}

void abaixarBracosRapido() {
  Serial.println("Abaixando braços rapidamente!");
  moverBraco(bracoD, 0, VELOCIDADE_RAPIDA);
  moverBraco(bracoE, 0, VELOCIDADE_RAPIDA);
}

// --- Funções para Mover APENAS UM Braço (Novo!) ---
// Estas funções são para quando você quer mexer só um braço de cada vez.

void levantarBracoDireito(int velocidade) {
  Serial.println("Levantando braço direito!");
  moverBraco(bracoD, 180, velocidade);
}

void abaixarBracoDireito(int velocidade) {
  Serial.println("Abaixando braço direito!");
  moverBraco(bracoD, 0, velocidade);
}

void levantarBracoEsquerdo(int velocidade) {
  Serial.println("Levantando braço esquerdo!");
  moverBraco(bracoE, 180, velocidade);
}

void abaixarBracoEsquerdo(int velocidade) {
  Serial.println("Abaixando braço esquerdo!");
  moverBraco(bracoE, 0, velocidade);
}

// --- Funções de Passos de Dança ---
// Aqui criamos os "passos" que o robô vai fazer. Cada função é um movimento!
// Você pode criar seus próprios passos aqui, usando as funções de movimento do robô e dos braços.

void passo1_acena() {
  Serial.println("Passo: Acenando!");
  levantarBracosRapido(); // Levanta os dois braços rápido
  delay(500);             // Espera um pouco
  abaixarBracosRapido();  // Abaixa os dois braços rápido
  delay(500);             // Espera um pouco
}

// Este passo faz o robô acenar várias vezes, usando um loop 'for'.
// 'vezes' é quantas vezes ele vai acenar.
void passo_acenaRepetidamente(int vezes) {
  Serial.print("Passo: Acenando ");
  Serial.print(vezes);
  Serial.println(" vezes!");
  for (int i = 0; i < vezes; i++) { // O 'for' repete o que está dentro dele 'vezes' vezes
    passo1_acena(); // Chama o passo de acenar uma vez
    delay(PAUSA_ENTRE_ACENOS); // Pequena pausa entre cada aceno (ajustável lá em cima!)
  }
}

// Este passo é um exemplo de como mexer um braço de cada vez.
void passo_mexerUmBraco() {
  Serial.println("Passo: Mexendo um braço de cada vez!");
  levantarBracoDireito(VELOCIDADE_MEDIA); // Levanta o braço direito na velocidade média
  delay(300);
  abaixarBracoDireito(VELOCIDADE_MEDIA);  // Abaixa o braço direito na velocidade média
  delay(300);
  levantarBracoEsquerdo(VELOCIDADE_RAPIDA); // Levanta o braço esquerdo rápido
  delay(300);
  abaixarBracoEsquerdo(VELOCIDADE_RAPIDA);  // Abaixa o braço esquerdo rápido
  delay(300);
}

void passo2_giraEsq() {
  Serial.println("Passo: Girando para a esquerda!");
  girarEsquerda(DURACAO_GIRO_ESQUERDA_CURTO); // Gira um pouco para a esquerda (duração ajustável!)
  levantarBracosMedio(); // Levanta os braços
  delay(300);
  abaixarBracosMedio();  // Abaixa os braços
}

void passo3_andaFrente() {
  Serial.println("Passo: Andando para frente!");
  moverFrente(255); // Faz o robô andar para frente (255 seria a "velocidade" máxima se usasse PWM)
  delay(DURACAO_ANDA_FRENTE); // Anda por um tempo (ajustável!)
  pararMotores();             // Para o robô
  delay(PAUSA_POS_MOVIMENTO); // Pequena pausa
  levantarBracosLento();      // Levanta os braços lentamente
  delay(PAUSA_POS_MOVIMENTO); // Pequena pausa
  abaixarBracosLento();       // Abaixa os braços lentamente
}

void passo4_giraDir180() {
  Serial.println("Passo: Girando 180 graus para a direita!");
  girarDireita(DURACAO_GIRO_180); // Gira 180 graus para a direita (duração ajustável!)
  delay(500);
  levantarBracosRapido(); // Levanta os braços rápido
  delay(200);
  abaixarBracosRapido();  // Abaixa os braços rápido
}

// --- Função Principal da Dança (`comecarDanca()`) ---
// Esta é a sua coreografia! Aqui você junta todos os passos que criou na ordem que quiser.
// Pense nisso como a lista de movimentos que o robô vai fazer do começo ao fim da música.
void comecarDanca() {
  Serial.println("A dança vai começar!");
  passo_acenaRepetidamente(ACENO_REPETICOES_PRIMEIRO); // O robô acena algumas vezes (quantas? veja lá em cima!)
  passo2_giraEsq();                                    // Gira para a esquerda
  passo_mexerUmBraco();                                // Mexe um braço de cada vez (novo passo!)
  passo3_andaFrente();                                 // Anda para frente
  passo4_giraDir180();                                 // Gira 180 graus
  passo_acenaRepetidamente(ACENO_REPETICOES_SEGUNDO);  // Acena mais algumas vezes
  passo3_andaFrente();                                 // Anda para frente de novo
  Serial.println("Fim da sequência de dança!");
}

// --- Configuração Inicial (`setup()`) ---
// Esta função roda APENAS UMA VEZ quando o Arduino é ligado ou resetado.
// É onde preparamos tudo para o robô começar a trabalhar.
void setup() {
  Serial.begin(9600); // Inicia a comunicação com o computador (para ver as mensagens no Monitor Serial)
  Serial.println("Iniciando o robô dançarino!");

  // Anexa os servos aos pinos que definimos lá em cima.
  // Isso "liga" os servos ao controle do Arduino.
  bracoD.attach(SERVO_BRACO_D);
  bracoE.attach(SERVO_BRACO_E);

  // Define os pinos dos motores como SAÍDA.
  // Isso significa que o Arduino vai ENVIAR energia para esses pinos para controlar os motores.
  pinMode(MOTOR_DIR_1, OUTPUT);
  pinMode(MOTOR_DIR_2, OUTPUT);
  pinMode(MOTOR_ESQ_1, OUTPUT);
  pinMode(MOTOR_ESQ_2, OUTPUT);

  // Garante que os braços comecem abaixados (posição 0 graus).
  bracoD.write(0);
  bracoE.write(0);
  pararMotores(); // Garante que o robô esteja parado no início
}

// --- Loop Principal (`loop()`) ---
// Esta função roda INFINITAMENTE, uma vez após a outra, depois que o 'setup()' termina.
// É aqui que chamamos a nossa dança e o passo final.
void loop() {
  Serial.println("Esperando a música começar...");
  delay(TEMPO_SINCRONIA_MUSICA); // Espera o tempo definido para sincronizar com a música (ajustável lá em cima!)

  comecarDanca(); // Chama a nossa função de dança principal!

  // --- Último Passo: Final da Dança ---
  // Este é um movimento especial que acontece só uma vez, depois da dança principal.
  Serial.println("Executando o último passo da dança!");
  levantarBracosRapido();             // Levanta os braços rapidamente
  moverTras(255);                     // Anda para trás (255 é a "velocidade" máxima)
  delay(DURACAO_ANDA_TRAS_FINAL);     // Anda para trás por um tempo (ajustável!)
  pararMotores();                     // Para os motores
  abaixarBracosLento();               // Abaixa os braços lentamente

  Serial.println("Dança finalizada! O robô vai descansar aqui.");
  while (true); // Faz o robô parar de dançar e esperar aqui para sempre.
                // Ele não vai mais executar nada depois disso, até ser resetado.
}
