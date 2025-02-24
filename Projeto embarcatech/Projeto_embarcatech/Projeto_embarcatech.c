#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "ssd1306.h"    // Responsavel por controlar o display oled
#include "hardware/pwm.h" 
#include "matriz_led.h" // Responsavel por controlar a Matriz de LEDS
#include "wifi_clima.h" // Responsavel pelo recebimento e proessamentod de dados da OpenWeather
#include "pico/time.h"
#include "WifiHttp.h"  // Responsavel pelo envio de dados para o Thing Speak
#define I2C_PORT i2c1
#define I2C_SDA 15 
#define I2C_SCL 14
#define BOTAO_A 5   // Botão A para ligar/desligar matriz LED (abajur)
#define BOTAO_B 6   // Botão B para exibir informações do clima


// Declaraçõa de variáveis globais
ssd1306_t disp;             // Instância do display OLED
volatile bool atualizar_clima_flag = false; 
bool matriz_ligada = false;  // Estado inicial da matriz LED
bool wifi_conectado = false; // Variavel para armazenar o status do wifi
struct repeating_timer timer; // Temporizador para atualização periódica de dados
bool estado_anterior_abajur = false; // Armazena o estado anterior do abajur


// Configuração inicial do sistema
void setup(){
    stdio_init_all();
    // Inicializa a comunicação I2C para o display OLED
    i2c_init(I2C_PORT, 400*1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);  
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    // Configuração do display OLED
    disp.external_vcc=false;
    ssd1306_init(&disp, 128, 64, 0x3C, I2C_PORT);
    ssd1306_clear(&disp);


    // Inicializa botões de entrada
    gpio_init(BOTAO_A);
    gpio_set_dir(BOTAO_A, GPIO_IN);
    gpio_pull_up(BOTAO_A);
    gpio_init(BOTAO_B);
    gpio_set_dir(BOTAO_B, GPIO_IN);
    gpio_pull_up(BOTAO_B);
}


// Função para escrever no display OLED
void print_texto(int x, int y, int tam, char * msg){
    ssd1306_draw_string(&disp, x, y, tam, msg);
    ssd1306_show(&disp);
}

// Função para das intruções do uso do abajur no display OLED
void print_instrucoes(){ 
  
    print_texto(1, 5, 1, "BTN_A: Abajur ON/OFF");
    print_texto(1, 30, 1,"BTN_B: Mostrar clima");
}

// Função para printar dados do clima
void print_clima_atualizado(){  
    ssd1306_clear(&disp);
    print_texto(10, 5, 1, "Cidade:");
    print_texto(60, 5, 1, cidade);
    print_texto(10, 20, 1, "Temp:");
    print_texto(60, 20, 1, temp_str);
    print_texto(10, 35, 1, "Umidade:");
    print_texto(60, 35, 1, umi_str);
}

// Função para alternar a Matriz de LEDs
void alternar_matriz_led(){  
    // Caso a matriz estivesse ligada antes de exibir o clima, liga novamente
    if (estado_anterior_abajur) {
        ssd1306_clear(&disp);
        printf("Abajur Ligado\n");
        npAbajur();  
        print_texto(25, 28, 1, "Abajur Ligado");
        estado_anterior_abajur = false;  
    } else {
        // Alterna o estado da matriz normalmente
        matriz_ligada = !matriz_ligada;
        ssd1306_clear(&disp);
        if (matriz_ligada) {
            ssd1306_clear(&disp);
            printf("Abajur Ligado\n");
            npAbajur();
            print_texto(25, 28, 1, "Abajur Ligado");
        } else {
            ssd1306_clear(&disp);
            printf("Abajur Desligado\n");
            npClear();
            npWrite();
            print_texto(20, 28, 1, "Abajur Desligado");
        }
    }
    sleep_ms(1000);
    ssd1306_clear(&disp);
}

// Calcula o ponto de orvalho para estimar chuva
float calcular_ponto_orvalho(float temperatura, int umidade) {  
    return temperatura - ((100 - umidade) / 5.0);
}

// Estima a chance de chuva com base na umidade e temperatura
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

// Exibe a previsão do tempo no display OLED
void print_previsao_chuva() {
    ssd1306_clear(&disp);
    print_texto(15, 5, 1, "Previsao do Tempo:");
    const char* previsao = estimar_chuva(temperatura, umidade);   
    char buffer[50]; 
    snprintf(buffer, sizeof(buffer), "%s", previsao); 
    print_texto(1, 28, 1, buffer); 


// Atualiza a Matriz de LEDs conforme a previsão do tempo
 if (matriz_ligada || !matriz_ligada) {  
    printf("Atualizando Abajur\n");

    if (strstr(previsao, "Baixa chance") != NULL) {
       
        npSol();  // Liga  Matriz com desenho do sol 
      
    } else {
        npChuva();  // Liga Matriz com desenho de chuva 
    }
} 
}


int main(){
    setup();
    npInit(LED_PIN);

    // Mensagem de boas vindas
    print_texto(25,28,1, "Ola, bem-vindo!"); 
    sleep_ms(2000);

    wifi_conectado = conectar_wifi();
    sleep_ms(2000);
    print_instrucoes();  
    sleep_ms(2000);
    // Adiciona temporizador para atualizar os dados a cada 30 segundos
    add_repeating_timer_ms(30000, repeating_timer_callback, NULL, &timer); 
  
    while (true) {

        print_instrucoes();

        // Detecta pressionamento do botão A (Liga/Desliga Matriz de LEDs)
        if (gpio_get(BOTAO_A) == 0) {
            sleep_ms(100);
            if (gpio_get(BOTAO_A) == 0) {
                alternar_matriz_led();
                while (gpio_get(BOTAO_A) == 0);
            }
        }

      // Exibir clima com Botão B apenas se Wi-Fi estiver conectado
      if (gpio_get(BOTAO_B) == 0) {
        sleep_ms(100);
        if (gpio_get(BOTAO_B) == 0) {
            if (wifi_conectado) {
                if(matriz_ligada){
                    estado_anterior_abajur = true;  
                    npClear(); 
                }
                ssd1306_clear(&disp);
                print_texto(5, 28, 1, "Confirmando dados...");
                sleep_ms(300);
                buscar_clima();
                print_clima_atualizado();
                sleep_ms(3000);
                print_previsao_chuva();
                sleep_ms(5000);
                ssd1306_clear(&disp);
                npClear();
            } else {
                ssd1306_clear(&disp);
                print_texto(10, 20, 1, "Wi-Fi Desconectado!");
                print_texto(10,30,1, "Nao e possivel");
                print_texto(10,40,1, "mostrar os dados");   
                sleep_ms(2000);
                ssd1306_clear(&disp);
            }
            while (gpio_get(BOTAO_B) == 0);
        }
    }
    sleep_ms(50);
}
}