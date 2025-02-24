#ifndef WIFI_CLIMA_H
#define WIFI_CLIMA_H
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/apps/http_client.h"
#include "lwip/dns.h"
#include "lwip/netif.h"
#include "lwip/sockets.h"
#include "lwip/tcp.h"
#include "lwip/udp.h"
#include <string.h>
#include "config_wifi.h"
#include "ssd1306.h"
#include "cJSON.h"
extern ssd1306_t disp;
extern char cidade[50];\
extern float temperatura;
extern int umidade;
extern char temp_str[10];
extern char umi_str[10];

// Declaração das funções para uso externo
void buscar_clima(); // Buscar dados do clima
int conectar_wifi(); // Conectar ao Wifi
void print_texto_2(int x, int y, int tam, char * msg);  // Função para exibir texto no display
void dados_clima(const char *json_string); // Tratar dados do clima

#endif
