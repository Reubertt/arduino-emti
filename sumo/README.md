# 🤖 Robô de Sumô Autônomo 🥋

## 📝 Descrição do Projeto
Este projeto contém o código para um robô de sumô autônomo, projetado para competir em um ringue. O robô utiliza sensores ultrassônicos para detectar oponentes e sensores infravermelhos (IR) para identificar as bordas do ringue. Sua inteligência permite que ele ataque, defenda-se e evite cair da arena. Prepare-se para a batalha! 💥

## ✨ Funcionalidades Principais
- 🎯 **Detecção de Oponentes:** Usa sensor ultrassônico para localizar oponentes.
- 🚀 **Ataque e Busca:** Avança em direção ao oponente detectado e realiza manobras de busca quando nenhum oponente está à vista.
- 🛡️ **Mecanismo de Defesa:** Executa uma manobra evasiva se ficar travado contra um oponente.
- 🚧 **Detecção de Borda do Ringue:** Sensores IR frontal e traseiro para evitar que o robô saia da arena.
  - ➡️ **Borda Frontal:** Recua e gira para se afastar.
  - ⬅️ **Borda Traseira:** Empurra agressivamente para frente para voltar ao ringue.
- ⚙️ **Controle de Motores:** Utiliza uma Ponte H com controle PWM para movimentos suaves e precisos.

## 🛠️ Hardware Necessário
- 🧠 Placa Arduino (Uno, Nano, etc.)
- 🌉 Ponte H L298N (ou similar)
- 🏎️ 2 Motores DC com caixa de redução
- 🔊 1 Sensor Ultrassônico HC-SR04
- 🚦 2 Sensores Infravermelhos (IR) de linha (um frontal, um traseiro)
- 🔋 Bateria (para alimentar o Arduino e os motores)
- 🔌 Fios jumper, protoboard (opcional)

## 💻 Software Necessário
- 🚀 IDE do Arduino

## 🔌 Guia de Montagem e Conexões
Para que seu robô de sumô funcione, você precisa conectar os componentes ao Arduino corretamente. Siga este guia passo a passo:

1.  **ALIMENTAÇÃO DA PONTE H (L298N):**
    - Conecte o pino VIN (ou 12V) da Ponte H ao polo positivo da sua bateria.
    - Conecte o pino GND da Ponte H ao polo negativo da sua bateria E ao GND do Arduino.
    - (Opcional) Se sua bateria for > 9V, conecte o pino 5V da Ponte H ao 5V do Arduino para alimentar o Arduino (verifique a documentação da sua Ponte H).

2.  **MOTORES DC:**
    - Conecte os dois fios de um motor aos pinos OUT1 e OUT2 da Ponte H.
    - Conecte os dois fios do outro motor aos pinos OUT3 e OUT4 da Ponte H.
    (Se um motor girar para o lado errado, inverta a conexão dos fios dele na Ponte H).

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
    (Verifique a documentação do seu sensor IR: alguns podem precisar de um resistor pull-up/down ou ter uma saída invertida - HIGH para linha, LOW para fora da linha, ou vice-versa. O código assume HIGH para linha.)

## 🚀 Como Usar o Código
1.  📂 Abra o arquivo `code.ino` no IDE do Arduino.
2.  ⚙️ Verifique e ajuste os parâmetros na seção `1. CONFIGURAÇÕES` do `code.ino` conforme a sua montagem e o comportamento desejado.
3.  ⬆️ Faça o upload do código para o seu Arduino.
4.  🔌 Desconecte o Arduino do computador e ligue a bateria do robô.
5.  🏁 O robô aguardará o tempo inicial e começará a operar! Que a batalha comece! 💥

## 🔧 Personalização e Ajustes
Todos os parâmetros de comportamento do robô (tempos, distâncias, velocidades) podem ser ajustados na seção `1. CONFIGURAÇÕES` do arquivo `code.ino`. Experimente diferentes valores para otimizar a estratégia do seu robô! Seja criativo! 💡

## ❓ Solução de Problemas
- **Robô não se move:** Verifique as conexões dos motores e da Ponte H. Certifique-se de que a bateria está carregada. 🔋
- **Robô se move de forma inesperada:** Verifique as conexões dos sensores e dos motores. Use o Monitor Serial do Arduino para ver as mensagens de depuração e entender o que o robô está "pensando". 🤔
- **Sensor IR não funciona:** Verifique a polaridade (VCC/GND) e a conexão do pino de sinal. Alguns sensores IR podem ter uma saída invertida (LOW para linha), o que exigiria uma pequena alteração na lógica de `checkRingBoundary()`.
