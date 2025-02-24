Desenvolvimento de uma Solução em Sistemas Embarcados
Olá, estudantes! Bem-vindos à nossa tarefa prática sobre o desenvolvimento de soluções baseadas em Sistemas Embarcados, utilizando PWM e I2C. Nesta atividade, vocês terão a oportunidade de aplicar os conceitos estudados para projetar uma solução inovadora que atenda a um problema, utilizando tecnologias em Sistemas Embarcados.

As seções da tarefa estão organizadas conforme instruções a seguir:

1. Planejamento do Projeto
O projeto pode ser desenvolvido utilizando os códigos disponíveis no repositório da placa BitDogLab (https://github.com/BitDogLab/BitDogLab-C)
Deve ser desenvolvido na linguagem C do Pico SDK
2. Especificação do Projeto
Desenvolva um programa em C utilizando o Pico SDK para um sistema embarcado que implemente um menu interativo no display SSD1306 (OLED) e utilize o joystick para navegação.
O eixo Y do joystick deve controlar a navegação para cima e para baixo no menu.
O botão do joystick deve ser utilizado para selecionar uma opção e também para retornar ao menu.
3. Estrutura do menu
O menu deve conter as seguintes opções:
Rodar o programa do joystick LED do Repositório do BitDogLab (https://github.com/BitDogLab/BitDogLab-C/tree/main/Joystick_led)
Tocar o buzzer do Repositório do BitDogLab (https://github.com/BitDogLab/BitDogLab-C/tree/main/buzzer_pwm1)
Ligar o LED RGB do Repositório do BitDogLab  (https://github.com/BitDogLab/BitDogLab-C/tree/main/PWM_LED_0)
4. Comportamento esperado
Ao selecionar uma opção do menu, com o botão do joystick, deve ser executado o programa correspondente.
O botão do joystick também deve ter a função de retornar ao menu principal com o seu pressionamento.
Sugestões e recomendações
Iniciar o desenvolvimento pela lógica do menu
Utilizar alguma interrupção para o botão do joystick.
Implementar uma função de cada vez para um item do menu.

