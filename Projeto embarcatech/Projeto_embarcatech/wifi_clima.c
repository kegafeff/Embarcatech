#include "wifi_clima.h"
#include "config_wifi.h"

// Definição das variáveis globais
char cidade[50] = "";
float temperatura;
int umidade;
char temp_str[10]= "";
char umi_str[10] = "";
static httpc_state_t *clima_conexao = NULL;


// Definição das variáveis globais
void print_texto_2(int x, int y, int tam, char *msg) {   
    ssd1306_draw_string(&disp, x, y, tam, msg);
    ssd1306_show(&disp);
}

// Callback para processar a resposta HTTP e extrair os dados do clima
static err_t http_callback(void *arg, struct altcp_pcb *conn, struct pbuf *p, err_t err) { 
    if (err == ERR_OK && p != NULL) {
        char response[p->tot_len + 1];
        pbuf_copy_partial(p, response, p->tot_len, 0);
        response[p->tot_len] = '\0';

        printf("Resposta HTTP recebida!\n");
       
        dados_clima(response); // Processa os dados JSON da resposta

        pbuf_free(p);
    } else {
        printf("Erro no callback HTTP: %d\n", err);
        sleep_ms(1000);
    }
    return ERR_OK;
}

// Verifica e reconecta o Wi-Fi se necessário
void verificar_wifi() {  
    int tentativas = 0;

    cyw43_arch_enable_sta_mode();
    cyw43_wifi_pm(&cyw43_state, CYW43_NO_POWERSAVE_MODE);

    if (cyw43_wifi_link_status(&cyw43_state, CYW43_ITF_STA) == CYW43_LINK_UP) {
        printf("Wi-Fi já está conectado, sem necessidade de reconexão.\n");
        return;  // Se já estiver conectado, não faz nada
    }
    // Exibe mensagem no display informando a tentativa de reconexão
    printf("Wi-Fi caiu! Tentando reconectar...\n");
    print_texto_2(10,28,1, "Tentando reconectar");
    print_texto_2(35,40,1, "ao Wi-Fi...");
    sleep_ms(3000);
    ssd1306_clear(&disp);
    
    while (tentativas < 3) {
        if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 15000) == 0) {
            printf("Wi-Fi reconectado com sucesso!\n");
            return;
        }

        printf("Falha ao reconectar! Tentando novamente...\n");
        tentativas++;
        sleep_ms(5000);
    }
    // Exibe mensagem de falha no display caso não consiga reconectar
    printf("Falha ao reconectar após várias tentativas. Mantendo Wi-Fi desconectado.\n");
    ssd1306_clear(&disp);
    print_texto_2(1,10,1, "Nao foi possivel");
    print_texto_2(1,20,1, "reconectar, o abajur");
    print_texto_2(1,30,1, "so funcionara");
    print_texto_2(1,40,1, "no modo ON/OFF ");
    sleep_ms(5000);
    ssd1306_clear(&disp);
  
   
}


void buscar_clima() { // Envia requisição HTTP para obter os dados do clima
  

    printf("Buscando dados do clima...\n");
    sleep_ms(1000);

    // Gera a URL para a requisição HTTP com os parâmetros de cidade e API key
    char url[256];
    snprintf(url, sizeof(url), "/data/2.5/weather?q=%s,%s&appid=%s&units=metric", CITY, COUNTRY_CODE, API_KEY);

    printf("URL Gerada: %s\n", url);
    sleep_ms(1000);
    // Configura e envia a requisição HTTP
    httpc_connection_t settings = {0};
    err_t err = httpc_get_file_dns("api.openweathermap.org", 80, url, &settings, http_callback, NULL, &clima_conexao);

    if (err == ERR_OK) {
        printf("Requisição enviada!\n");
        
    // Aguarda a resposta processando eventos de rede
      printf("Aguardando resposta...\n");
       for (int i = 0; i < 100; i++) {  
           cyw43_arch_poll();  
           sleep_ms(100);
        }
  }
     else {
        printf("Erro ao iniciar requisição HTTP: %d\n", err);
        sleep_ms(1000);
    }
}


void dados_clima(const char *json_string) {  //  Processa o JSON recebido e armazena os dados nas variáveis globais
    cJSON *json = cJSON_Parse(json_string);
    if (!json) {
        printf("Erro ao analisar JSON\n");
        sleep_ms(1000);
        return;
    }

    //  Obtém o nome da cidade
    cJSON *name = cJSON_GetObjectItem(json, "name");
    if (cJSON_IsString(name)) {
        strncpy(cidade, name->valuestring, sizeof(cidade));
        cidade[sizeof(cidade) - 1] = '\0';  // Garante que a string termine corretamente
    }

    //  Obtém a temperatura e umidade
    cJSON *main = cJSON_GetObjectItem(json, "main");
    if (main) {
        cJSON *temp = cJSON_GetObjectItem(main, "temp");
        cJSON *hum = cJSON_GetObjectItem(main, "humidity");

        if (cJSON_IsNumber(temp)) {
            temperatura = (float) temp->valuedouble;  // Converte corretamente para float
        } else {
            temperatura = -999.9;  //  Valor de erro para depuração
        }

        if (cJSON_IsNumber(hum)) {
            umidade = hum->valueint;  // Armazena corretamente a umidade como inteiro
        } else {
            umidade = -1;  // Valor de erro para depuração
        }
    }

    // Converte FLOAT e INT para STRING

    snprintf(temp_str, sizeof(temp_str), "%.2f°C", temperatura);
    snprintf(umi_str, sizeof(umi_str), "%d%%", umidade);



    cJSON_Delete(json);  // Libera a memória JSON
}

int conectar_wifi() { //  Função para conectar ao Wi-Fi
    stdio_init_all();
    
    
    ssd1306_clear(&disp);
    print_texto_2(1, 28, 1, "Conectando ao Wi-Fi...");
    sleep_ms(1000);
    if (cyw43_arch_init()) {
        printf("Erro ao iniciar Wi-Fi\n");
        ssd1306_clear(&disp);
        print_texto_2(1, 28, 1, "Erro ao iniciar Wi-Fi");
        sleep_ms(1000);
        return false;
    }

    cyw43_arch_enable_sta_mode();
    cyw43_wifi_pm(&cyw43_state, CYW43_NO_POWERSAVE_MODE);
 // Tenta conectar ao Wi-Fi e verifica se a conexão foi bem-sucedida
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 15000)) {
        ssd1306_clear(&disp);
        printf("Falha ao conectar ao Wi-Fi\n");
        print_texto_2(10,28,1, "Falha ao conectar");
        print_texto_2(35,40,1, "ao Wi-Fi");
        sleep_ms(3000);
        ssd1306_clear(&disp);
        verificar_wifi();  
        return false;
    }

    printf("Wi-Fi conectado!\n");
    ssd1306_clear(&disp);
    print_texto_2(20,28,1, "Wi-Fi conectado");
    ssd1306_clear(&disp);
   
    sleep_ms(200);

    return true;
}
