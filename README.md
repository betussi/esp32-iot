IoT IFSP CTD
Projeto do trabalho de conclusão da disciplina de Plataformas de Prototipação para IoT (E2PPT) IFSP CTD.

Visão Geral
Medidores de nível de fluído em contêineres usando sensores ultrassônicos. Os dados originados nos sensores são publicados em um tópico via MQTT. Um segundo conjunto, inscrito no tópico, analisa os níveis e comanda uma bomba d'água garantindo o escoamento do contêiner correspondente. Esse conjunto também pode ser acionado arbitrariamente através de um comando lido de um segundo tópico.

Um terceiro conjunto contendo um ESP32 e um LCD 15x2 faz a leitura do tópico e exibe as leituras no display. Também contém um botão que comanda, arbitrariamente, o escoamento do contêiner, enviando um comando em um segundo tópico.
