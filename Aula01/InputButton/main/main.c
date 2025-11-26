#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define BUTTON_GPIO 4

void app_main(void){
    // Na aula lembra de tirar o resistor pra mostrar sem pull-up
    // Depois altera o codigo para PULLUP_ENABLE para mostrar q a ESP32 permite isso
    
    gpio_config_t io_conf = {
        .pin_bit_mask = 1ULL << BUTTON_GPIO,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,    // Para ativar: GPIO_PULLUP_ENABLE, desativar: GPIO_PULLUP_DISABLE
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    gpio_config(&io_conf);
    int contador = 0; 

    while (1) {
        int level = gpio_get_level(BUTTON_GPIO);
        if(!level){
            ++contador;
            printf("Botão pressionado [%d] vezes\n", contador);
        }
    }
}