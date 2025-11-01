# 💃 Robô Dançarino 🕺

## 📝 Descrição do Projeto
Este projeto contém o código para um robô dançarino, projetado para executar uma coreografia pré-programada. Ele demonstra o controle de motores para movimentação e servos para simular "braços", permitindo a criação de passos de dança personalizados. Prepare-se para o show! ✨

## ✨ Funcionalidades Principais
- 🏎️ **Controle de Motores:** Movimentação do robô (andar para frente, para trás, girar).
- 🤖 **Controle de Servos:** Movimento de "braços" com diferentes velocidades.
- 🎶 **Passos de Dança:** Funções modulares para criar e combinar movimentos.
- ✍️ **Coreografia Personalizável:** A função `comecarDanca()` permite montar a sequência de passos.

## 🛠️ Hardware Necessário
- 🧠 Placa Arduino (Uno, Nano, etc.)
- 🌉 Ponte H L298N (ou similar)
- 🏎️ 2 Motores DC com caixa de redução
- 🤖 2 Servos (para os braços)
- 🔋 Bateria (para alimentar o Arduino e os motores/servos)
- 🔌 Fios jumper, protoboard (opcional)

## 💻 Software Necessário
- 🚀 IDE do Arduino
- 📚 Biblioteca `Servo.h` (já incluída no IDE do Arduino)

## 🔌 Guia de Montagem e Conexões
Para que seu robô dançarino funcione, você precisa conectar os componentes ao Arduino corretamente. Siga este guia passo a passo:

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

## 🚀 Como Usar o Código
1.  📂 Abra o arquivo `dance.ino` no IDE do Arduino.
2.  ⚙️ Verifique e ajuste os parâmetros na seção "Constantes de Parâmetros de Dança e Movimento" do `dance.ino` para personalizar a coreografia.
3.  ⬆️ Faça o upload do código para o seu Arduino.
4.  🔌 Desconecte o Arduino do computador e ligue a bateria do robô.
5.  🎶 O robô aguardará o tempo inicial e começará a dançar! Que a música comece! 🎵

## ✍️ Personalização e Ajustes
- **Ajustar Parâmetros da Dança:** Altere os valores na seção "Constantes de Parâmetros de Dança e Movimento" no `dance.ino` (ex: `TEMPO_SINCRONIA_MUSICA`, `ACENO_REPETICOES`, `DURACAO_GIRO`). Solte a criatividade! 💡
- **Criar Novos Passos de Dança:** Siga as instruções detalhadas na seção "Como Criar Novos Passos de Dança?" no `dance.ino`. Invente seus próprios movimentos! ✨
- **Montar Sua Coreografia:** Edite a função `comecarDanca()` no `dance.ino` para organizar seus passos na ordem desejada. Crie sua própria dança! 🕺

## ❓ Solução de Problemas
- **Robô não se move/Servos não funcionam:** Verifique as conexões de alimentação e sinal dos motores e servos. Certifique-se de que a bateria está carregada. 🔋
- **Movimentos errados:** Verifique se os pinos definidos no código correspondem às suas conexões físicas. Se um motor girar para o lado errado, inverta seus fios na Ponte H. 🔄
- **Servos tremem:** Pode ser um problema de alimentação. Tente usar uma fonte de alimentação externa para os servos, se possível, ou verifique as conexões. ⚡
