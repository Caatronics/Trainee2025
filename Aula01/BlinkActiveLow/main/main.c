#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LED_1 GPIO_NUM_32
#define LED_2 GPIO_NUM_25
#define LED_3 GPIO_NUM_13

void app_main() {
  ESP_ERROR_CHECK(gpio_set_direction(LED_1, GPIO_MODE_OUTPUT));
  ESP_ERROR_CHECK(gpio_set_direction(LED_2, GPIO_MODE_OUTPUT));
  ESP_ERROR_CHECK(gpio_set_direction(LED_3, GPIO_MODE_OUTPUT));

  bool led_on = false;

  while (true) {
    ESP_ERROR_CHECK(gpio_set_level(LED_1, led_on));
    ESP_ERROR_CHECK(gpio_set_level(LED_2, !led_on));
    ESP_ERROR_CHECK(gpio_set_level(LED_3, !led_on));
    vTaskDelay(2000/portTICK_PERIOD_MS);

    ESP_ERROR_CHECK(gpio_set_level(LED_1, !led_on));
    ESP_ERROR_CHECK(gpio_set_level(LED_2, led_on));
    ESP_ERROR_CHECK(gpio_set_level(LED_3, !led_on));
    vTaskDelay(2000/portTICK_PERIOD_MS);

    ESP_ERROR_CHECK(gpio_set_level(LED_1, !led_on));
    ESP_ERROR_CHECK(gpio_set_level(LED_2, !led_on));
    ESP_ERROR_CHECK(gpio_set_level(LED_3, led_on));
    vTaskDelay(2000/portTICK_PERIOD_MS);
  }
}
