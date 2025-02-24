#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2818b.pio.h"

#define LED_COUNT 25
#define LED_PIN 7

struct pixel_t {
  uint8_t G, R, B;
};
typedef struct pixel_t pixel_t;
typedef pixel_t npLED_t;

npLED_t leds[LED_COUNT];

PIO np_pio;
uint sm;


uint8_t brightness = 255;  // Variável de controle de brilho (0 a 255)

void npInit(uint pin) { 
  uint offset = pio_add_program(pio0, &ws2818b_program);
  np_pio = pio0;
  sm = pio_claim_unused_sm(np_pio, false);
  if (sm < 0) {
    np_pio = pio1;
    sm = pio_claim_unused_sm(np_pio, true);
  }
  ws2818b_program_init(np_pio, sm, offset, pin, 800000.f);

  for (uint i = 0; i < LED_COUNT; ++i) {
    leds[i].R = 0;
    leds[i].G = 0;
    leds[i].B = 0;
  }
}

void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b) {
  leds[index].R = r;
  leds[index].G = g;
  leds[index].B = b;
}


void npWrite() { // Função pra escrever na matriz
  for (uint i = 0; i < LED_COUNT; ++i) {
    pio_sm_put_blocking(np_pio, sm, (leds[i].G * brightness) );
    pio_sm_put_blocking(np_pio, sm, (leds[i].R * brightness) );
    pio_sm_put_blocking(np_pio, sm, (leds[i].B * brightness) );
  }
  sleep_us(100);
}

void npClear() {  // Função pra limpar a matriz
  for (uint i = 0; i < LED_COUNT; ++i)
    npSetLED(i, 0, 0, 0);
    npWrite();
}


void npAbajur() { // Função para ligar abajur
 
  for (int i = 0; i < LED_COUNT; i++) {
    npSetLED(i, 255, 255, 255);
  }
  npWrite();
}


void npSol() {  // Função para desenhar um sol
  
  int sol_indices[] = {0,4, 6, 7, 8, 11,13,16, 17,18,20,24};
  for (int i = 0; i < sizeof(sol_indices) / sizeof(sol_indices[0]); i++) {
    npSetLED(sol_indices[i], 255, 255, 0); // Amarelo
  }
  npWrite();
}


void npChuva() {  // Função para desenhar um símbolo de chuva

  int chuva_indices[] = {1, 3, 5, 7, 9, 11, 13, 17}; // Novo padrão de gotas
  for (int i = 0; i < sizeof(chuva_indices) / sizeof(chuva_indices[0]); i++) {
    npSetLED(chuva_indices[i], 0, 0, 255); // Azul
  }
  npWrite();
}


