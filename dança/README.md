# Robô Dançarino

## Descrição do Projeto
Este projeto contém o código para um robô dançarino, projetado para executar uma coreografia pré-programada. Ele demonstra o controle de motores para movimentação e servos para simular "braços", permitindo a criação de passos de dança personalizados.

## Funcionalidades Principais
- **Controle de Motores:** Movimentação do robô (andar para frente, para trás, girar).
- **Controle de Servos:** Movimento de "braços" com diferentes velocidades.
- **Passos de Dança:** Funções modulares para criar e combinar movimentos.
- **Coreografia Personalizável:** A função `comecarDanca()` permite montar a sequência de passos.

## Hardware Necessário
- Placa Arduino (Uno, Nano, etc.)
- Ponte H L298N (ou similar) para controle dos motores
- 2 Motores DC com caixa de redução
- 2 Servos (para os braços)
- Bateria (para alimentar o Arduino e os motores/servos)
- Fios jumper, protoboard (opcional)

## Software Necessário
- IDE do Arduino
- Biblioteca `Servo.h` (já incluída no IDE do Arduino)

## Guia de Montagem e Conexões
Para um guia detalhado de como montar o robô e conectar todos os componentes ao Arduino, por favor, consulte os comentários no arquivo `dance.ino` dentro desta pasta. Ele contém um passo a passo completo para as ligações elétricas.

## Como Usar o Código
1.  Abra o arquivo `dance.ino` no IDE do Arduino.
2.  Verifique e ajuste os parâmetros na seção "Constantes de Parâmetros de Dança e Movimento" do `dance.ino` para personalizar a coreografia.
3.  Faça o upload do código para o seu Arduino.
4.  Desconecte o Arduino do computador e ligue a bateria do robô.
5.  O robô aguardará o tempo inicial e começará a dançar!

## Personalização e Ajustes
- **Ajustar Parâmetros da Dança:** Altere os valores na seção "Constantes de Parâmetros de Dança e Movimento" no `dance.ino` (ex: `TEMPO_SINCRONIA_MUSICA`, `ACENO_REPETICOES`, `DURACAO_GIRO`).
- **Criar Novos Passos de Dança:** Siga as instruções detalhadas na seção "Como Criar Novos Passos de Dança?" no `dance.ino`.
- **Montar Sua Coreografia:** Edite a função `comecarDanca()` no `dance.ino` para organizar seus passos na ordem desejada.

## Solução de Problemas
- **Robô não se move/Servos não funcionam:** Verifique as conexões de alimentação e sinal dos motores e servos. Certifique-se de que a bateria está carregada.
- **Movimentos errados:** Verifique se os pinos definidos no código correspondem às suas conexões físicas. Se um motor girar para o lado errado, inverta seus fios na Ponte H.
- **Servos tremem:** Pode ser um problema de alimentação. Tente usar uma fonte de alimentação externa para os servos, se possível, ou verifique as conexões.
