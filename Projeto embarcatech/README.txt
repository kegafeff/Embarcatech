Link do ThingSpeak: https://thingspeak.mathworks.com/channels/2841618
Link do Video: https://drive.google.com/file/d/1aC6DAo8nwO4p8zBkXN8WcWAQOzCjReBs/view?usp=sharing

1. Objetivo
Desenvolver uma solução embarcada, utilizando a placa BitDogLab. O projeto deve integrar os conhecimentos adquiridos durante o curso, como programação em C, protocolos de comunicação, projeto de sistemas embarcados e IoT. 

2. Introdução
Em aplicações IoT, geralmente estamos interessados em um conjunto de diferentes variáveis, obtidos por meio de diferentes sensores, que se utilizam de microcontroladores interligados em rede para enviar dados que serão processados e analisados. Além disso, dependendo da aplicação, esses controladores podem enviar sinais de controle para dispositivos IoT com função de atuadores, que também estão na rede, para alterar o processo e, com isso, a variável que está sendo controlada.  

A Figura 1 mostra um fluxograma básico de uma solução IoT. Os dispositivos IoT, que podem medir e/ou atuar, estão na camada de percepção e formam uma interface do sistema embarcado com o mundo. Esses dados são enviados para um servidor (ou uma infraestrutura na cloud) usando diferentes tecnologias (wiFi, Bluetooth, NFC, etc) e protocolos de rede (HTTP, MQTT, AMQP, etc). Na nuvem, é possível processar esses dados, usar modelos de IA para classificação ou previsão de dados, armazenamento etc. Por fim, esses dados podem ser utilizados em diferentes aplicações, como dashboards, games, databoards, etc. 

É importante perceber que existem inúmeras aplicações IoT com diferentes tipos de dispositivos e estruturas, de forma que não existe um padrão em todas as soluções. Por exemplo, um dispositivo IoT pode ser um sensor de temperatura ligado a um microcontrolador capaz de se conectar em rede com outros dispositivos ou com um servidor. Por outro lado, um ar-condicionado já possui toda uma estrutura com sensor, controlador e atuador que, por si só, já desempenham a função de um sistema de controle. Apesar disso, aparelhos modernos de refrigeração já possuem a função de se conectar à internet, permitindo que ele seja controlado por um smartphone ou algum dispositivo assistente. Perceba que, nesse segundo caso, o ar-condicionado é um dispositivo (“coisa”) dentro de uma solução IoT.   

A ideia do projeto final é projetar e desenvolver uma solução IoT (protótipo) usando a BitDogLab, de forma que os conhecimentos adquiridos durante o curso sejam aplicados e o(a) aluno(a) possa direcionar esse conhecimento, se preparando para cenários que envolvem a solução de problemas reais, seja na residência tecnológica, seja no trabalho em empresas.  

3. Requisitos Gerais
Tema
Escolha uma área de aplicação de sua preferência, como saúde, automação residencial, agricultura, indústria, entretenimento, educação, segurança, etc. 
Funcionalidades
A solução deve interagir com os componentes da placa, como botões, joystick, matriz de LEDs, display LCD, LED RGB, buzzer, microfone ou sensor de temperatura. A utilização de pelo menos 3 periféricos é obrigatória. Mas lembre-se, isso deve fazer sentido dentro do contexto da sua aplicação! 
Deve utilizar algum protocolo de dados como (I2C, UART ou SPI), ADC ou PWM na solução. 
Na marcação de tempo da aplicação é necessário usar temporizador. 
Deve ser implementada uma funcionalidade de comunicação externa, utilizando o módulo Wi-Fi do Raspberry Pi Pico W (usando algum tipo de protocolo estudado). 
Os dados devem ser enviados para algum serviço de nuvem, como o ThingSpeak.  
A interface do sistema deve ser intuitiva, fornecendo informações relevantes por meio de displays, LEDs e/ou sons. Além disso, caso necessário, dashboards ou plataformas web devem ser utilizadas e projetadas de acordo com o contexto da aplicação.  
Pode ser utilizado sensores externos. Mas, o que deve ser feito é um protótipo! Caso não tenha acesso a sensores específicos, você pode simular a aquisição de dados via código. 
Desenvolvimento
Crie um sistema que resolva ou melhore uma necessidade ou problema na área escolhida. 
Utilize boas práticas de programação, incluindo modularização, documentação e tratamento de erros. 
Implemente, sempre que aplicável, métodos para configuração e monitoramento em tempo real. 
4. Sugestões e Áreas de Aplicações
Saúde: Monitoramento de sinais vitais, lembrete de medicamentos.
Automação Residencial: Controle de iluminação, monitoramento de temperatura ou segurança. 
Indústria: Monitoramento de máquinas, controle de processos. 
Agricultura: Sistema de irrigação automática baseado em sensores. 
Entretenimento: Jogos interativos, sistemas de luz e som sincronizados. 
Educação: Ferramentas de aprendizado interativo. 
5. Entrega Final
Ao final, o aluno deverá entregar 3 artefatos mostrando o desenvolvimento do projeto, conforme descritos a seguir:

Relatório em formato de Artigo Científico conforme modelo em anexo , com as seguintes seções:
Introdução: Deve contextualizar o projeto, apresentando o problema ou necessidade que será abordado, além de sua relevância na área de aplicação escolhida. É importante que essa seção explique por que o tema foi selecionado e quais são os objetivos do trabalho, destacando como a solução proposta contribui para a resolução do problema. 
Metodologia:  O foco está em descrever detalhadamente os recursos utilizados no projeto, como a placa BitDogLab, componentes específicos (botões, display LCD, matriz de LEDs, etc.) e tecnologias de comunicação (Wi-Fi, protocolos MQTT ou HTTP). Também é necessário explicar a abordagem adotada no desenvolvimento, os passos principais e as estratégias de teste ou validação. Caso haja coleta de dados, deve-se esclarecer os métodos empregados. Se for o caso, utilize fluxogramas para explicar os métodos utilizando. 
Desenvolvimento: Apresentar de forma estruturada todas as etapas de implementação. Deve-se descrever como cada funcionalidade foi desenvolvida, detalhando a interação entre os componentes, com o suporte de diagramas, fluxogramas e explicações de trechos relevantes do código. Nesta parte, é importante mostrar resultados obtidos durante os testes e validações, demonstrando o funcionamento correto do sistema. 
Conclusão: O autor deve fazer uma avaliação geral do projeto, destacando os resultados alcançados e se os objetivos propostos foram atendidos. Também é necessário refletir sobre os desafios enfrentados, as limitações encontradas e possíveis melhorias para versões futuras. Essa seção deve finalizar reforçando a contribuição prática e teórica do projeto para a área escolhida. 
Código-fonte comentado e estruturado.  
Facilite a correção! Comente o código, indicando os pontos chaves da sua aplicação: acionamento de botões ou periféricos de saída, conexão com a rede, etc. 
Vídeo de Apresentação da Solução.  
Grave um vídeo de até 2 minutos explicando a solução , sugere-se que siga o seguinte roteiro:
Comece se apresentando! Nome completo e a matrícula do EmbarcaTech. 
Fale rapidamente sobre a sua solução. 
Apresente a solução em funcionamento na sua placa, indicando os periféricos que estão sendo utilizados. 
Não se apresentar no vídeo implicará na perda dos pontos relativos ao vídeo. 
O apresentador do vídeo deve ser o autor da tarefa, não podendo utilizar ferramentas de automação.  
Neste vídeo não é necessário apresentar código fonte, nem o relatório desenvolvido. 
A utilização de slides é opcional, a prioridade deve ser a demonstração da solução em funcionamento. 
Aparecer no vídeo não é obrigatório, mas é desejável. Aqueles que não quiserem aparecer, mesmo assim, devem se apresentar, falando o nome completo e a matrícula do EmbarcaTech. 
O vídeo pode ser colocado no YouTube ou adicionado a um drive e o link deve ser enviado na tarefa do moodle. Atenção: assegure que o link está publico para que seja possível realizar a correção. 


