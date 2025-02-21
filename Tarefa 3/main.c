#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

// Função de inicialização do botão
const uint botao = 10; // Pino do botão
void iniciando_botao() {
    gpio_init(botao); // Iniciando como entrada automaticamente
    gpio_pull_up(botao); // Configurando como pull_up
}

// Função de inicialização dos LEDs
#define LEDR 14
#define LEDY 13
#define LEDG 12
#define LEDP 11
void iniciando_leds() {
    gpio_init(LEDR);
    gpio_init(LEDY);
    gpio_init(LEDG);
    gpio_init(LEDP);

    gpio_set_slew_rate(LEDR, GPIO_SLEW_RATE_SLOW);
    gpio_set_slew_rate(LEDY, GPIO_SLEW_RATE_SLOW);
    gpio_set_slew_rate(LEDG, GPIO_SLEW_RATE_SLOW);
    gpio_set_slew_rate(LEDP, GPIO_SLEW_RATE_SLOW);

    gpio_set_dir(LEDR, true);
    gpio_set_dir(LEDY, true);
    gpio_set_dir(LEDG, true);
    gpio_set_dir(LEDP, true);
}

// Função de inicialização do PWM no buzzer
#define BUZZER_PIN 9
#define BUZZER_FREQUENCY 2000
void pwm_init_buzzer(uint pin) {
    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(pin);
    pwm_config config = pwm_get_default_config();
    float clkdiv = (float)clock_get_hz(clk_sys) / (BUZZER_FREQUENCY * 4096);
    pwm_config_set_clkdiv(&config, clkdiv);
    pwm_init(slice_num, &config, true);
    pwm_set_gpio_level(pin, 0);
}

// Função para emitir um beep com duração especificada
void beep(uint pin, uint duration_ms) {
    uint slice_num = pwm_gpio_to_slice_num(pin);
    pwm_set_gpio_level(pin, 2048); // Duty cycle de 50%
    sleep_ms(duration_ms);
    pwm_set_gpio_level(pin, 0);
}

// Função para selecionar o estado do semáforo comum
void semaforo_estado_comum(bool r, bool y, bool g) {
    gpio_put(LEDR, r);
    gpio_put(LEDY, y);
    gpio_put(LEDG, g);
}

// Função para selecionar o estado do semáforo de pedestre
void semaforo_botao_pressionado(bool p) {
    gpio_put(LEDP, p);
}

// Função que aciona a sequência dos semáforos para pedestres
void semaforo_pedestre(int estado) {
    if (estado == 0) {
        semaforo_estado_comum(false, true, false);
        sleep_ms(5000);
        semaforo_botao_pressionado(true);
        semaforo_estado_comum(true, false, false);
        beep(BUZZER_PIN, 15000);
    } else {
        semaforo_botao_pressionado(true);
        semaforo_estado_comum(true, false, false);
        beep(BUZZER_PIN, 15000);
    }
}

// Função para controlar o estado padrão do semáforo
void semaforo_estado_padrao() {
    semaforo_botao_pressionado(false);
    for (int i = 0; i < 8000; i++) {
        if (gpio_get(botao) == 0) {
            semaforo_pedestre(0);
            return;
        } else {
            semaforo_estado_comum(false, false, true);
            sleep_ms(1);
        }
    }
    for (int i = 0; i < 2000; i++) {
        if (gpio_get(botao) == 0) {
            semaforo_pedestre(0);
            return;
        } else {
            semaforo_estado_comum(false, true, false);
            sleep_ms(1);
        }
    }
    for (int i = 0; i < 10000; i++) {
        if (gpio_get(botao) == 0) {
            semaforo_pedestre(1);
            return;
        } else {
            semaforo_estado_comum(true, false, false);
            sleep_ms(1);
        }
    }
}

// Função principal do programa
int main() {
    stdio_init_all();
    iniciando_leds();
    iniciando_botao();
    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);
    pwm_init_buzzer(BUZZER_PIN);
    while (true) {
        semaforo_estado_padrao();
    }
}
