// NOTA:
// Aqui estou utilizando esp_timer, que é um timer de precisão muito alta
// que permite criar timers com intervalos muito curtos.
// Ele funciona na camada de software, ou seja, não é um timer de hardware.
// Ele agenda quando uma "tarefa" deve ser executada.

#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>

static const char *TAG = "TIMER"; // Tag para identificar os logs deste arquivo

// Definição dos GPIOs conectados aos LEDs
#define LED_1 GPIO_NUM_32 // LED Vermelho (por exemplo)
#define LED_2 GPIO_NUM_25 // LED Amarelo
#define LED_3 GPIO_NUM_13 // LED Verde

// Função de callback do timer periódica
void timer_callback(void *arg) {
  static int counter =
      0; // Variável estática: mantém seu valor entre as chamadas da função

  // Lógica para reiniciar o contador e manter o ciclo 0 -> 1 -> 2 -> 0...
  if (counter == 3) {
    counter = 0;
  }

  // Máquina de estados para controlar os LEDs baseada no valor de counter
  switch (counter) {
  case 0:                                      // Estado 0
    ESP_ERROR_CHECK(gpio_set_level(LED_1, 1)); // Liga LED 1
    ESP_ERROR_CHECK(gpio_set_level(LED_2, 0)); // Desliga LED 2
    ESP_ERROR_CHECK(gpio_set_level(LED_3, 0)); // Desliga LED 3
    break;
  case 1:                                      // Estado 1
    ESP_ERROR_CHECK(gpio_set_level(LED_1, 0)); // Desliga LED 1
    ESP_ERROR_CHECK(gpio_set_level(LED_2, 1)); // Liga LED 2
    ESP_ERROR_CHECK(gpio_set_level(LED_3, 0)); // Desliga LED 3
    break;
  case 2:                                      // Estado 2
    ESP_ERROR_CHECK(gpio_set_level(LED_1, 0)); // Desliga LED 1
    ESP_ERROR_CHECK(gpio_set_level(LED_2, 0)); // Desliga LED 2
    ESP_ERROR_CHECK(gpio_set_level(LED_3, 1)); // Liga LED 3
    break;
  }

  // Imprime no log serial e DEPOIS incrementa o contador (counter++)
  ESP_LOGI(TAG, "Callback executado! count=%d", counter++);
}

// Função principal (entry point)
void app_main() {
  // Configura a direção dos pinos dos LEDs como SAÍDA (Output)
  ESP_ERROR_CHECK(gpio_set_direction(LED_1, GPIO_MODE_OUTPUT));
  ESP_ERROR_CHECK(gpio_set_direction(LED_2, GPIO_MODE_OUTPUT));
  ESP_ERROR_CHECK(gpio_set_direction(LED_3, GPIO_MODE_OUTPUT));

  // Argumentos para criação do timer
  const esp_timer_create_args_t timer_args = {
      .callback = &timer_callback, // Função que será chamada periodicamente
      .arg = NULL,       // Argumento para passar à função (não usado aqui)
      .name = "Semaforo" // Nome descritivo do timer
  };

  esp_timer_handle_t timer; // Handler para armazenar a referência do timer
  // Cria o timer, passando os argumentos e o endereço do handler
  ESP_ERROR_CHECK(esp_timer_create(&timer_args, &timer));

  // Inicia o timer em modo PERIÓDICO
  // O tempo é especificado em microssegundos
  // 2.000.000 us = 2 segundos
  esp_timer_start_periodic(timer, 2000000);
}
