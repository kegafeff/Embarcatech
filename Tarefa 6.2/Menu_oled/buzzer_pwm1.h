/**
 * Exemplo de acionamento do buzzer utilizando sinal PWM no GPIO 21 da Raspberry Pico / BitDogLab
 * 06/12/2024
 */

 #include <stdio.h>
 #include "pico/stdlib.h"
 #include "hardware/pwm.h"
 #include "hardware/clocks.h"
 #include "joystick.h"
 
 // Configuração do pino do buzzer
 #define BUZZER_PIN 21
 
 // Notas musicais para a música tema de Star Wars
 const uint star_wars_notes[] = {
     330, 330, 330, 262, 392, 523, 330, 262,
     392, 523, 330, 659, 659, 659, 698, 523,
     415, 349, 330, 262, 392, 523, 330, 262,
     392, 523, 330, 659, 659, 659, 698, 523,
     415, 349, 330, 523, 494, 440, 392, 330,
     659, 784, 659, 523, 494, 440, 392, 330,
     659, 659, 330, 784, 880, 698, 784, 659,
     523, 494, 440, 392, 659, 784, 659, 523,
     494, 440, 392, 330, 659, 523, 659, 262,
     330, 294, 247, 262, 220, 262, 330, 262,
     330, 294, 247, 262, 330, 392, 523, 440,
     349, 330, 659, 784, 659, 523, 494, 440,
     392, 659, 784, 659, 523, 494, 440, 392
 };
 
 // Duração das notas em milissegundos
 const uint note_duration[] = {
     500, 500, 500, 350, 150, 300, 500, 350,
     150, 300, 500, 500, 500, 500, 350, 150,
     300, 500, 500, 350, 150, 300, 500, 350,
     150, 300, 650, 500, 150, 300, 500, 350,
     150, 300, 500, 150, 300, 500, 350, 150,
     300, 650, 500, 350, 150, 300, 500, 350,
     150, 300, 500, 500, 500, 500, 350, 150,
     300, 500, 500, 350, 150, 300, 500, 350,
     150, 300, 500, 350, 150, 300, 500, 500,
     350, 150, 300, 500, 500, 350, 150, 300,
 };
 
 // Inicializa o PWM no pino do buzzer
 void pwm_init_buzzer(uint pin) {
     gpio_set_function(pin, GPIO_FUNC_PWM);
     uint slice_num = pwm_gpio_to_slice_num(pin);
     pwm_config config = pwm_get_default_config();
     pwm_config_set_clkdiv(&config, 4.0f); // Ajusta divisor de clock
     pwm_init(slice_num, &config, true);
     pwm_set_gpio_level(pin, 0); // Desliga o PWM inicialmente
 }
 
 // Toca uma nota com a frequência e duração especificadas
 void play_tone(uint pin, uint frequency, uint duration_ms) {
    uint slice_num = pwm_gpio_to_slice_num(pin);
    uint32_t clock_freq = clock_get_hz(clk_sys);
    uint32_t top = clock_freq / frequency - 1;

    pwm_set_wrap(slice_num, top);
    pwm_set_gpio_level(pin, top / 2); // 50% de duty cycle

    uint elapsed_time = 0;
    while (elapsed_time < duration_ms) {
        if (gpio_get(SW) == 0) { // Se o botão for pressionado, interrompe imediatamente
            pwm_set_gpio_level(pin, 0);
            return;
        }
        sleep_ms(10); // Pequenos intervalos para verificar o botão
        elapsed_time += 10;
    }

    pwm_set_gpio_level(pin, 0); // Desliga o som após a duração
}

 
 
 // Função principal para tocar a música
 void play_star_wars(uint pin) {
    for (int i = 0; i < sizeof(star_wars_notes) / sizeof(star_wars_notes[0]); i++) {
        
        if(gpio_get(SW) == 0){
            pwm_set_gpio_level(pin, 0);
            return;
        }

        if (star_wars_notes[i] == 0) {
            sleep_ms(note_duration[i]);
        } else {
            play_tone(pin, star_wars_notes[i], note_duration[i]);
        }
    }
 }
 
 int play_buzzer() {
    pwm_init_buzzer(BUZZER_PIN);

    while(gpio_get(SW) == 1){
        play_star_wars(BUZZER_PIN);
    }

    sleep_ms(200); 
    return 0;
}