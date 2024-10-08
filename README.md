# IoT Fluid Level Monitoring System

## Projeto de Trabalho de Conclusão da Disciplina de Plataformas de Prototipação para IoT (E2PPT) - IFSP CTD

### Visão Geral

Este projeto visa monitorar o nível de fluído em contêineres utilizando sensores ultrassônicos. O sistema é composto por três conjuntos principais:

1. **Publicação de Dados via MQTT**:  
   Os sensores ultrassônicos instalados nos contêineres medem o nível de fluído, e os dados coletados são publicados em um tópico MQTT específico.

2. **Controle da Bomba d'Água**:  
   Um segundo conjunto de dispositivos está inscrito no tópico MQTT para receber as medições de nível. Este sistema analisa os dados e comanda uma bomba d'água, garantindo o escoamento do contêiner correspondente quando necessário. Além disso, é possível acionar a bomba arbitrariamente, enviando um comando para um segundo tópico MQTT.

3. **Visualização e Controle Local**:  
   O terceiro conjunto é composto por um ESP32 e um display LCD 15x2, que exibe em tempo real as leituras do nível de fluído publicadas no tópico. Um botão está integrado ao sistema para permitir o comando manual da bomba d'água, enviando um comando a outro tópico MQTT.

### Componentes Utilizados
- Sensores ultrassônicos
- ESP32
- Display LCD 15x2
- Bomba d'água
- Protocolo de comunicação MQTT

### Funcionalidades
- Monitoramento contínuo dos níveis de fluído nos contêineres.
- Controle automático e manual do escoamento via bomba d'água.
- Publicação e assinatura de tópicos MQTT para comunicação entre dispositivos.
- Interface de visualização local com ESP32 e LCD.

### Como Executar
1. Configure o servidor MQTT e conecte todos os dispositivos aos tópicos corretos.
2. Programe o ESP32 para ler os dados dos sensores e publicar no tópico MQTT.
3. Programe o segundo conjunto de dispositivos para monitorar os níveis e acionar a bomba d'água.
4. O ESP32 com LCD pode ser utilizado para visualizar os níveis e enviar comandos de escoamento.

### Contribuição
Contribuições são bem-vindas! Para contribuir, faça um fork do repositório e envie um pull request com as suas alterações.

### Licença
Este projeto está licenciado sob a Licença MIT - veja o arquivo [LICENSE](LICENSE) para mais detalhes.

### Contato
Desenvolvido por [Seu Nome](https://github.com/seu-usuario) - Entre em contato para mais informações!

