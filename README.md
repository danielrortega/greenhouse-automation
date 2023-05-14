# greenhouse-automation
Sistema de automação de estufa com área para enraizamento de mudas, crescimento das mudas e endurecimento das plantas em fase pré-plantio

O projeto consiste inicialmente de um Arduino Mega 2560, Relay shield, sensor de temperatura e umidade (DHT22) e display de cristal 
líquido para mostrar e configurar tempos e intervalos de pulverização. Também está incluso um sensor ultrasônico que verifica o volume de
água no reservatório e controla a abertura e fechamento de uma solenóide responsável por enchê-lo.

Na próxima etada será incluído shield ethernet ou ESP8266 para visualização de dashboard em tempo real e outros acessórios para 
automatizar irrigação da área aberta (moisture sensor e pluviômetro).

Com uso do Ethernet Shield grava informações numa base de dados do InfluxDB para visualização num dashboard do Grafana.
