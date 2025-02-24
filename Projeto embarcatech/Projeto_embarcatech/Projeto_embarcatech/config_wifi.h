#ifndef CONFIG_H
#define CONFIG_H
// O config_wifi.h, armazena as informações da configuração da API e da conexão wifi.

// Configuração do Wi-Fi
#define WIFI_SSID "brisa-3509628"   // Digite o Wifi aqui "brisa-3509628" 
#define WIFI_PASSWORD "fs7b94ha" // Digite a senha aqui fs7b94ha"

// Configuração da API OpenWeatherMap
#define API_KEY "3f18968dbd0051e3f1cfd83d38fb1879"  // Insira sua chave gerada aqui Open Weather
#define CITY "Natal"        // Substitua pela cidade desejada, Utilizando URL Encoding por causa do ã "
#define COUNTRY_CODE "BR"        // Código do país (exemplo: BR para Brasil)

#define API_KEY_2 "X52RONJ01MYDOZR2"  // API do thingspeaker
#define THINGSPEAK_HOST "api.thingspeak.com"
#define THINGSPEAK_PORT 80

// URL para obter o clima em graus Celsius
#define CLIMA_URL "http://api.openweathermap.org/data/2.5/weather?q=" CITY "," COUNTRY_CODE "&appid=" API_KEY "&units=metric" 

#endif
