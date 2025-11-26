#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define BUTTON_GPIO 4
#define DEBOUNCE_TIME_MS 200 
#define LED_PIN 21

void app_main(void){
    gpio_config_t io_conf = {
        .pin_bit_mask = 1ULL << BUTTON_GPIO,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,     
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_conf);

    ESP_ERROR_CHECK(gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT));

    int contador = 0;
    int estado_anterior = 1; // Assumindo pull-up (1 = solto)
    bool estado_led = false; // Estado atual do LED (Comeca desligado, configuracao Active High)

    TickType_t ultimo_tempo_clique = 0; // Variável para guardar o momento do último clique válido

    while (1) {
        int estado_atual = gpio_get_level(BUTTON_GPIO);
        
        TickType_t tempo_atual = xTaskGetTickCount(); // Pega o tempo atual em Ticks

        // Lógica:
        // 1. Detectou borda de descida (botão apertado)?
        // 2. E JÁ PASSOU o tempo de debounce desde o último clique?
        if (estado_anterior == 1 && estado_atual == 0 && (tempo_atual - ultimo_tempo_clique) > pdMS_TO_TICKS(DEBOUNCE_TIME_MS)){
            
            contador++;
            printf("Clique Válido! [%d] (Tempo: %lu)\n", contador, (unsigned long)tempo_atual);

            estado_led = !estado_led; // Atualiza o estado do LED
            
            ultimo_tempo_clique = tempo_atual; // Atualiza o relógio do último clique válido
        }

        estado_anterior = estado_atual;
        ESP_ERROR_CHECK(gpio_set_level(LED_PIN, estado_led)); // Garante o novo estado do LED quando necessario
        
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}