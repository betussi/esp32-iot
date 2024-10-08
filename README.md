# IoT - Sistema de Controle de Temperatura e Umidade Residencial

#### Autores: Leonardo Donato e Rodrigo Betussi

## Projeto de Trabalho de Conclusão da Disciplina de Plataformas de Prototipação para IoT (E2PPT) - IFSP CTD

### Visão Geral

Este projeto visa monitorar em tempo real a temperatura e umidade através do sensor DHT11, onde os dados são enviados para um banco de dados (DynamoDB) onde é possível através de um gráfico ter um histórico completo das coletas para futuras análises. 
O sistema é composto por três conjuntos principais:

1. **Publicação de Dados via MQTT**:  
   Através do ESP32 os dados são coletados e enviados via protocolo MQTT.

2. **Controle da Bomba d'Água**:  
   Um segundo conjunto de dispositivos está inscrito no tópico MQTT para receber as medições de nível. Este sistema analisa os dados e comanda uma bomba d'água, garantindo o escoamento do contêiner correspondente quando necessário. Além disso, é possível acionar a bomba arbitrariamente, enviando um comando para um segundo tópico MQTT.

3. **Visualização dos dados**:  
   O terceiro conjunto é composto por um ESP32 e um display LCD 15x2, que exibe em tempo real as leituras do nível de fluído publicadas no tópico. Um botão está integrado ao sistema para permitir o comando manual da bomba d'água, enviando um comando a outro tópico MQTT.

### Componentes Utilizados
- Sensor DHT11
- ESP32
- Protocolo de comunicação MQTT
- Node-Red (Back-end e Front-End)
- AWS DynamoDB

### Funcionalidades

### Como Executar
