#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LED_PIN GPIO_NUM_19

void app_main() {
  ESP_ERROR_CHECK(gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT));
  bool estado = true;

  while (true) {
    ESP_ERROR_CHECK(gpio_set_level(LED_PIN, estado));
    if (estado)
      printf("\nAcendi!");
    else
      printf("\nApaguei!");
    estado = !estado;
    vTaskDelay(2000/portTICK_PERIOD_MS);
  }
}
