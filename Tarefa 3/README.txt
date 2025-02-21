Tarefa - Linguagens e Ambiente de Programação
1. Contextualização 

          Um aluno do curso de Tecnologia em Análise e Desenvolvimento de Sistemas do IFRN – campus Central tem um familiar com deficiência visual e ele sempre atravessa cruzamentos nas vias de Natal para chegar ao trabalho ou estudar.  Dessa forma, o aluno de TADS está desenvolvendo um sinal sonoro de baixo custo para ajudar o seu familiar na travessia de faixas de pedestres da cidade.
          
2. Objetivos

Desenvolver um sistema de controle de sinais de trânsito para um cruzamento, incluindo: 
Sinal sonoro indicando permissão para pedestres atravessarem. 
Botão de acionamento para deficientes visuais que desencadeia uma sequência de fechamento do sinal de trânsito para veículos, permitindo a travessia segura em todas no cruzamento da via. 

3. Descrição do Projeto
Componentes Virtuais no Wokwi: 
Raspberry Pi Pico W para controle lógico. 
LEDs:
Vermelho, amarelo e verde para representar o sinal de trânsito dos carros. 
Um LED verde para pedestres. 
Buzzer: para o som emitido quando o pedestre pode atravessar. 
Push button: para simular a botoeira (acionador do sinal de pedestre). 
Resistores: para os LEDs e botão. Protoboard e fiação. 
Linguagem: 
Todo o projeto deve ser desenvolvido a partir da linguagem C.
O código deve estar estruturado (indentado) e as principais funções deverão estar comentadas. 
Se possível, utilize funções e variáveis para simplificar o controle. 
Funcionamento do Sistema 

4. Estado Inicial: 
Os LEDs para carros alternarão automaticamente (verde -> amarelo -> vermelho) seguindo uma lógica de tempo fixa (ex.: 8 segundos no verde, 2 no amarelo, 10 no vermelho). 
O LED verde para pedestres estará desligado e o buzzer ficará inativo. 
Quando o botão é pressionado: 
O led amarelo deve ficar acionado por 5 segundos e depois o vermelho deve ser acionado por 15 segundos, permitindo o tempo de fechamento seguro para os veículos.  
A luz verde do pedestre deve ficar acesa pelo mesmo tempo em que a luz vermelha do semáforo estiver ativa (15 segundos). 
Além disso, durante esse tempo, o buzzer deverá emitir um som intermitente, indicando que o sinal está aberto para o pedestre. 
Pós-travessia: 
O sinal retorna ao funcionamento normal, alternando entre as luzes do semáforo do veículo. 
O led verde do pedestre deve ser apagado e o buzzer deve ficar sem acionamento.  

5. Esquemático proposto para o Wokwi 

No Wokwi, conecte os componentes da seguinte forma: 

Use um led vermelho, um led amarelo e um led verde, representando o semáforo dos carros. Dica: Conecte cada um dos leds a um pino configurado como saída (output) do Rapsberry Pi Pico. 
Use um led verde para representar o semáforo dos pedestres. Dica: Conecte o led a um pino configurado como saída (output) do Rapsberry Pi Pico. 
Use um Push Button para simular o botão que deve ser acionado pelo pedestre. Dica: Conecte a um GPIO configurado como entrada com pull-up. 
Use o Buzzer conectado a um GPIO para gerar o som. 
Use resistor (res) para limitar a corrente dos LEDs. Esses resistores são chamados de resistores de proteção e são colocados em série com o LED. 
