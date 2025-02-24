#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "ssd1306.h"
#include "hardware/adc.h"
#include "hardware/pwm.h" 
#include "joystick.h"
#include "buzzer_pwm1.h"
#include "PWM_LED_0.h"
#define I2C_PORT i2c1
#define I2C_SDA 15 
#define I2C_SCL 14
#define LED_B 12 // Pino do LED RGB BLUE
#define LED_R 13 // Pino do LED RGB RED 
#define LED_G 11 // Pino LED RGB GREEN
#define SW 22  //Pino do Botão do Joystick
#define VRY 26 //Porta ADC de variação do Y do Joystick
#define VRX 27 //Porta ADC de variação do X do joystick
#define BUZZER_PIN 21 // Pino do Buzzer
ssd1306_t disp;//variável display do Oled

void setup(){
   
    stdio_init_all();                //inicialização do Oled
    i2c_init(I2C_PORT, 400*1000);    // Inicializa o barramento I2C
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);  
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    disp.external_vcc=false;
    ssd1306_init(&disp, 128, 64, 0x3C, I2C_PORT);  // Inicializa o display OLED
    ssd1306_clear(&disp);                          // Limpa o display OLED

    //inicializa leds
    gpio_init(LED_B);
    gpio_init(LED_R);
    gpio_init(LED_G);
    gpio_set_dir(LED_B, GPIO_OUT);
    gpio_set_dir(LED_R, GPIO_OUT);
    gpio_set_dir(LED_G, GPIO_OUT);

    //inicialização do botão do joystick
    gpio_init(SW);
    gpio_set_dir(SW, GPIO_IN);
    gpio_pull_up(SW);

    //Inicializar ADC do joystick
    adc_init();
    adc_gpio_init(VRY);
    adc_gpio_init(VRX);
    setup_joystick();                                // Chama a função de configuração do joystick
    setup_pwm_led(LED_B, &slice_led_b, led_b_level); // Configura o PWM para o LED azul
    setup_pwm_led(LED_R, &slice_led_r, led_r_level); // Configura o PWM para o LED vermelho


    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);       // Configura o buzzer com PWM
    uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 4.0f); 
    pwm_init(slice_num, &config, true);
    pwm_set_gpio_level(BUZZER_PIN, 0);

}
void print_texto(int x, int y, int tam, char * msg){   // Função para exibir texto no display
    ssd1306_draw_string(&disp, x, y, tam, msg);
    ssd1306_show(&disp);
}
void print_retangulo(int x1, int y1, int x2, int y2){   //  Desenhar retangulo no display
    ssd1306_draw_empty_square(&disp, x1, y1, x2, y2);
    ssd1306_show(&disp);
}
void set_leds(int R, int G, int B){     // Função para ligar e desligar o LED individualmente 
    gpio_put(LED_R, R);   
    gpio_put(LED_G, G);   
    gpio_put(LED_B, B);
}
void print_menu(int pos){       // Exibição do menus
        ssd1306_clear(&disp);
        print_texto(52, 2, 1, "Menu");
        print_retangulo(2,pos+2,120,12);
        print_texto(6, 18, 1.5,"Joystick LED");
        print_texto(6, 30, 1.5, "Buzzer");
        print_texto(6, 42, 1.5, "RGB");
}

int main()
{
    setup();
    uint countdown = 0; //verificar seleções para baixo do joystick
    uint countup = 2; //verificar seleções para cima do joystick
    uint pos_y=12; //inicialização de variável para ler posição do Y do Joystick
    uint posy_ant=12;//posição anterior
    uint menu=1;
    
    print_menu(pos_y);//impressão inicial do menu

    while (true) {
        //trecho de código aproveitado de https://github.com/BitDogLab/BitDogLab-C/blob/main/joystick/joystick.c
        adc_select_input(0);
        uint adc_y_raw = adc_read();//leitura do joystick
        const uint bar_width = 40;
        const uint adc_max = (1 << 12) - 1;
        uint bar_y_pos = adc_y_raw * bar_width / adc_max; //bar_y_pos determinará se o Joystick foi pressionado para cima ou para baixo

        if(bar_y_pos < 17 && countdown <2){
            pos_y+=12;
            countdown+=1;
            countup-=1;
            menu++;//incrementa menu
        }else
            if(bar_y_pos > 21 && countup <2){
                pos_y-=12;
                countup+=1;
                countdown-=1;
                menu--;//decrementa menu
        }
        //texto do Menu
        if(pos_y!=posy_ant){//verifica se houve mudança de posição no menu.
            print_menu(pos_y);
        }
         //verifica se botão foi pressionado. 
         //Se sim, entra no switch case para verificar posição do seletor e chama acionamento dos leds.
        if(gpio_get(SW) == 0){
            sleep_ms(25);
            while(gpio_get(SW) == 0);
            switch (menu){
            case 1:
                joysticket_led();    //Programa do joystick led
            break;
            case 2:
                play_buzzer(); // Toca o buzzer
            break;
            case 3:
                led_rgb();  // Controla o LED RGB
            break;
          
            
           
            }
       }
        sleep_ms(50);//delay de atualização
        posy_ant=pos_y;//atualização posição anterior.
    }
}