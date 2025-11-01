# Robô de Sumô Autônomo

## Descrição do Projeto
Este projeto contém o código para um robô de sumô autônomo, projetado para competir em um ringue. O robô utiliza sensores ultrassônicos para detectar oponentes e sensores infravermelhos (IR) para identificar as bordas do ringue. Sua inteligência permite que ele ataque, defenda-se e evite cair da arena.

## Funcionalidades Principais
- **Detecção de Oponentes:** Usa sensor ultrassônico para localizar oponentes.
- **Ataque e Busca:** Avança em direção ao oponente detectado e realiza manobras de busca quando nenhum oponente está à vista.
- **Mecanismo de Defesa:** Executa uma manobra evasiva se ficar travado contra um oponente.
- **Detecção de Borda do Ringue:** Sensores IR frontal e traseiro para evitar que o robô saia da arena.
  - **Borda Frontal:** Recua e gira para se afastar.
  - **Borda Traseira:** Empurra agressivamente para frente para voltar ao ringue.
- **Controle de Motores:** Utiliza uma Ponte H com controle PWM para movimentos suaves e precisos.

## Hardware Necessário
- Placa Arduino (Uno, Nano, etc.)
- Ponte H L298N (ou similar)
- 2 Motores DC com caixa de redução
- 1 Sensor Ultrassônico HC-SR04
- 2 Sensores Infravermelhos (IR) de linha (um frontal, um traseiro)
- Bateria (para alimentar o Arduino e os motores)
- Fios jumper, protoboard (opcional)

## Software Necessário
- IDE do Arduino

## Guia de Montagem e Conexões
Para um guia detalhado de como montar o robô e conectar todos os componentes ao Arduino, por favor, consulte os comentários no arquivo `code.ino` dentro desta pasta. Ele contém um passo a passo completo para as ligações elétricas.

## Como Usar o Código
1.  Abra o arquivo `code.ino` no IDE do Arduino.
2.  Verifique e ajuste os parâmetros na seção `1. CONFIGURAÇÕES` do `code.ino` conforme a sua montagem e o comportamento desejado.
3.  Faça o upload do código para o seu Arduino.
4.  Desconecte o Arduino do computador e ligue a bateria do robô.
5.  O robô aguardará o tempo inicial e começará a operar!

## Personalização e Ajustes
Todos os parâmetros de comportamento do robô (tempos, distâncias, velocidades) podem ser ajustados na seção `1. CONFIGURAÇÕES` do arquivo `code.ino`. Experimente diferentes valores para otimizar a estratégia do seu robô!

## Solução de Problemas
- **Robô não se move:** Verifique as conexões dos motores e da Ponte H. Certifique-se de que a bateria está carregada.
- **Robô se move de forma inesperada:** Verifique as conexões dos sensores e dos motores. Use o Monitor Serial do Arduino para ver as mensagens de depuração e entender o que o robô está "pensando".
- **Sensor IR não funciona:** Verifique a polaridade (VCC/GND) e a conexão do pino de sinal. Alguns sensores IR podem ter uma saída invertida (LOW para linha), o que exigiria uma pequena alteração na lógica de `checkRingBoundary()`.
