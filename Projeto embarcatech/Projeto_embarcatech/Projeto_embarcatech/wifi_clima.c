#include "wifi_clima.h"
#include "config_wifi.h"

// Definição das variáveis globais

char cidade[50] = "";
float temperatura;
int umidade;
char temp_str[10]= "";
char umi_str[10] = "";
static httpc_state_t *clima_conexao = NULL;


void print_texto_2(int x, int y, int tam, char *msg) {   
    ssd1306_draw_string(&disp, x, y, tam, msg);
    ssd1306_show(&disp);
}


static err_t http_callback(void *arg, struct altcp_pcb *conn, struct pbuf *p, err_t err) { //  Callback para processar a resposta HTTP
    if (err == ERR_OK && p != NULL) {
        char response[p->tot_len + 1];
        pbuf_copy_partial(p, response, p->tot_len, 0);
        response[p->tot_len] = '\0';

        printf("✅ Resposta HTTP recebida!\n");
       
        dados_clima(response);

        pbuf_free(p);
    } else {
        printf("❌ Erro no callback HTTP: %d\n", err);
        sleep_ms(1000);
    }
    return ERR_OK;
}


void verificar_wifi() {  // Verifica e reconecta o Wi-Fi se necessário
    int tentativas = 0;

    if (cyw43_wifi_link_status(&cyw43_state, CYW43_ITF_STA) == CYW43_LINK_UP) {
        printf("✅ Wi-Fi já está conectado, sem necessidade de reconexão.\n");
        return;  // Se já estiver conectado, não faz nada
    }

    printf("❌ Wi-Fi caiu! Tentando reconectar...\n");
    
    while (tentativas < 3) {
        if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 15000) == 0) {
            printf("✅ Wi-Fi reconectado com sucesso!\n");
            return;
        }

        printf("⚠️ Falha ao reconectar! Tentando novamente...\n");
        tentativas++;
        sleep_ms(5000);
    }

    printf("❌ Falha ao reconectar após várias tentativas. Mantendo Wi-Fi desconectado.\n");
}


void buscar_clima() { // Envia requisição para obter os dados do clima
    verificar_wifi();  // Verifica se o Wi-Fi está ativo antes da requisição

    printf("📡 Buscando dados do clima...\n");
    sleep_ms(1000);

    char url[256];
    snprintf(url, sizeof(url), "/data/2.5/weather?q=%s,%s&appid=%s&units=metric", CITY, COUNTRY_CODE, API_KEY);

    printf("🔗 URL Gerada: %s\n", url);
    sleep_ms(1000);

    httpc_connection_t settings = {0};
    err_t err = httpc_get_file_dns("api.openweathermap.org", 80, url, &settings, http_callback, NULL, &clima_conexao);

    if (err == ERR_OK) {
        printf("✅ Requisição enviada!\n");
        
        
        // 🔄 Aguarda a resposta processando eventos de rede
      printf("⌛ Aguardando resposta...\n");
       for (int i = 0; i < 100; i++) {  
           cyw43_arch_poll();  
           sleep_ms(100);
        }
  }
     else {
        printf("❌ Erro ao iniciar requisição HTTP: %d\n", err);
        sleep_ms(1000);
    }
}


void dados_clima(const char *json_string) {  //  Processa o JSON recebido e armazena os dados nas variáveis globais
    cJSON *json = cJSON_Parse(json_string);
    if (!json) {
        printf("❌ Erro ao analisar JSON\n");
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

    // 🔄 Converte FLOAT e INT para STRING

    snprintf(temp_str, sizeof(temp_str), "%.2f°C", temperatura);
    snprintf(umi_str, sizeof(umi_str), "%d%%", umidade);



    cJSON_Delete(json);  // ✅ Libera a memória JSON
}

int conectar_wifi() { //  Função para conectar ao Wi-Fi
    stdio_init_all();
    
    
    ssd1306_clear(&disp);
    print_texto_2(1, 28, 1, "Conectando ao Wi-Fi...");
    sleep_ms(1000);
    if (cyw43_arch_init()) {
        printf("❌ Erro ao iniciar Wi-Fi\n");
        ssd1306_clear(&disp);
        print_texto_2(1, 28, 1, "Erro ao iniciar Wi-Fi");
        sleep_ms(1000);
        return 1;
    }

    cyw43_arch_enable_sta_mode();
    cyw43_wifi_pm(&cyw43_state, CYW43_NO_POWERSAVE_MODE);

    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 15000)) {
        printf("❌ Falha ao conectar ao Wi-Fi\n");
        ssd1306_clear(&disp);
        print_texto_2(10,28,1, "Falha ao conectar");
        print_texto_2(35,40,1, "ao Wi-Fi");
        sleep_ms(3000);
        ssd1306_clear(&disp);
        return 1;
    }

    printf("✅ Wi-Fi conectado!\n");
    ssd1306_clear(&disp);
    print_texto_2(20,28,1, "Wi-Fi conectado");
    sleep_ms(3000);
    ssd1306_clear(&disp);
    print_texto_2(15,20,1.6, "Recebendo Dados...");
    print_texto_2(30,30,1.6, "Aguarde");
    

    ssd1306_clear(&disp);
   
    sleep_ms(200);

    
    return 0;
}
