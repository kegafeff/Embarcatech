#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "ssd1306.h"
#include "hardware/pwm.h" 
#include "matriz_led.h"
#include "wifi_clima.h"
#include "pico/time.h"
#include "WifiHttp.h"
#define I2C_PORT i2c1
#define I2C_SDA 15 
#define I2C_SCL 14
#define LED_B 12
#define LED_R 13
#define LED_G 11
#define BOTAO_A 5   // Botão A para ligar/desligar matriz LED
#define BOTAO_B 6   // Botão B para exibir temperatura

ssd1306_t disp;
volatile bool atualizar_clima_flag = false; 
bool matriz_ligada = false;  // Estado inicial da matriz LED
repeating_timer_t timer; // Timer Global
char temp_atualizada[10]; // Variavel para passar os dados atualizados
char umi_atualizada[10]; // Variavel para passar os dados atualizados
struct repeating_timer timer;




void setup(){
    stdio_init_all();
    i2c_init(I2C_PORT, 400*1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);  
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    disp.external_vcc=false;
    ssd1306_init(&disp, 128, 64, 0x3C, I2C_PORT);
    ssd1306_clear(&disp);

    // Inicializa LEDs
    gpio_init(LED_B);
    gpio_init(LED_R);
    gpio_init(LED_G);
    gpio_set_dir(LED_B, GPIO_OUT);
    gpio_set_dir(LED_R, GPIO_OUT);
    gpio_set_dir(LED_G, GPIO_OUT);

    // Inicializa botões
    gpio_init(BOTAO_A);
    gpio_set_dir(BOTAO_A, GPIO_IN);
    gpio_pull_up(BOTAO_A);

    gpio_init(BOTAO_B);
    gpio_set_dir(BOTAO_B, GPIO_IN);
    gpio_pull_up(BOTAO_B);

}



void print_texto(int x, int y, int tam, char * msg){
    ssd1306_draw_string(&disp, x, y, tam, msg);
    ssd1306_show(&disp);
}


void set_leds(int R, int G, int B){
    gpio_put(LED_R, R);   
    gpio_put(LED_G, G);   
    gpio_put(LED_B, B);
}

void print_instrucoes(){ // Função para das intruções do uso do abajur
    ssd1306_clear(&disp);
    print_texto(1, 5, 1, "BTN_A: Abajur ON/OFF");
    print_texto(1, 30, 1,"BTN_B: Mostrar clima");
}

void print_clima_atualizado(){  // Função para printar dados do clima
    ssd1306_clear(&disp);
    print_texto(10, 5, 1, "Cidade:");
    print_texto(60, 5, 1, cidade);
    print_texto(10, 20, 1, "Temp:");
    print_texto(60, 20, 1, temp_atualizada);
    print_texto(10, 35, 1, "Umidade:");
    print_texto(60, 35, 1, umi_atualizada);
}



void alternar_matriz_led(){  // Função para alternar a Matriz de LEDs
    matriz_ligada = !matriz_ligada;  // Alterna entre ligado/desligado
    ssd1306_clear(&disp);
    if (matriz_ligada) {
        printf("Abajur Ligado\n");
        npAbajur();  // Liga matriz
        print_texto(25, 28, 1, "Abajur Ligado");
    } else {
        printf("Abajur Desligado\n");
        npClear();  // Desliga matriz
        npWrite();
        print_texto(20, 28, 1, "Abajur Desligado");
    }
}


float calcular_ponto_orvalho(float temperatura, int umidade) {  
    return temperatura - ((100 - umidade) / 5.0);
}


const char* estimar_chuva(float temperatura, int umidade) {  // Função para estimar a chance de chuva
    float ponto_orvalho = calcular_ponto_orvalho(temperatura, umidade);
    if (umidade > 85 && (temperatura - ponto_orvalho) < 2) {
        return "Alta chance de chuva!";
    } else if (umidade > 70 && (temperatura - ponto_orvalho) < 4) {
        return "Talvez Chova";
    } else {
        return "Baixa chance de chuva";
    }
}

void print_previsao_chuva() {
    ssd1306_clear(&disp);
    print_texto(15, 5, 1, "Previsao do Tempo:");
    const char* previsao = estimar_chuva(temperatura, umidade);   // Obtém a previsão
    char buffer[50]; // Criando um buffer temporário
    snprintf(buffer, sizeof(buffer), "%s", previsao); // Copiando string para buffer
    print_texto(1, 28, 1, buffer); // Exibe no display

 if (matriz_ligada || !matriz_ligada) {  
    printf("Atualizando Abajur\n");

    if (strstr(previsao, "Baixa chance") != NULL) {
       
        npSol();  // Liga  Matriz com desenho do sol 
      
    } else {
        npChuva();  // Liga Matriz com padrão de chuva 
    }
} 
}

// Função que será chamada a cada X segundos
bool atualizar_clima_em_segundo_plano(repeating_timer_t *rt) {
    atualizar_clima_flag = true;  // Ativa a flag para atualização no loop principal
    return true;
}
int main(){
    setup();
    npInit(LED_PIN);

    print_texto(25,28,1, "Ola, bem-vindo!"); // Mensagem inicial
    sleep_ms(2000);

   
    conectar_wifi();  // Conectar ao Wi-Fi 
    buscar_clima(); //  Buscar dados do clima na API Open Weather
    snprintf(temp_atualizada, sizeof(temp_atualizada), "%s", temp_str);
    snprintf(umi_atualizada, sizeof(umi_atualizada), "%s", umi_str);
    print_instrucoes();  // Exibir instruções do funcionamento do abajur
    sleep_ms(2000);

    add_repeating_timer_ms(120000, atualizar_clima_em_segundo_plano, NULL, &timer);  // Adiciona temporizador para atualizar os dados a cada 30 segundos
    
    add_repeating_timer_ms(30000, repeating_timer_callback, NULL, &timer);
  
    while (true) {
        // Se a flag de atualização for ativada, buscar novos dados
        if (atualizar_clima_flag) {
            printf("⏳ Atualizando temperatura e umidade...\n");
            buscar_clima();
            snprintf(temp_atualizada, sizeof(temp_atualizada), "%s", temp_str);
            snprintf(umi_atualizada, sizeof(umi_atualizada), "%s", umi_str);
            printf("✅ Dados armazenados! Temp: %s °C | Umidade:  %%\n", temp_atualizada, umi_atualizada);
            atualizar_clima_flag = false;  // Resetar flag
        }

        if (gpio_get(BOTAO_A) == 0) {
            sleep_ms(100);
            if (gpio_get(BOTAO_A) == 0) {
                alternar_matriz_led();
                while (gpio_get(BOTAO_A) == 0);
            }
        }

        if (gpio_get(BOTAO_B) == 0) {
            sleep_ms(100);
            if (gpio_get(BOTAO_B) == 0) {
                sleep_ms(2000);
                print_clima_atualizado();
                npClear();
                sleep_ms(3000);
                print_previsao_chuva();
                while (gpio_get(BOTAO_B) == 0);
            }
        }

        sleep_ms(50);
    }
}