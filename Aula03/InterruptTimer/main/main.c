// NOTA:
// Aqui estou utilizando driver/timer.h, que é a biblioteca do ESP32 para 
// controlar o timer de hardware.
// Então aqui eu vou gerar interrupções REAIS DE HARDWARE

#include "driver/gpio.h"       // Biblioteca para controle de GPIOs
#include "driver/timer.h"      // Biblioteca para controle do Timer de Hardware
#include "esp_log.h"           // Biblioteca para logs do ESP32
#include "freertos/FreeRTOS.h" // Biblioteca base do FreeRTOS
#include "freertos/task.h"     // Biblioteca para gerenciamento de tasks
#include <stdbool.h>           // Biblioteca para tipos booleanos
#include <stdio.h>             // Biblioteca padrão de entrada e saída

#define LED1_PIN 32 // Define pino do LED 1 como 32
#define LED2_PIN 25 // Define pino do LED 2 como 25
#define LED3_PIN 13 // Define pino do LED 3 como 13

static const char *TAG = "HW_TIMER"; // Tag para logs

#define TIMER_GROUP TIMER_GROUP_0 // Usa o Grupo de Timers 0
#define TIMER_IDX TIMER_0         // Usa o Timer 0 do Grupo 0

int counter; // Variável global para contagem

// Callback da interrupção do timer (executa na ISR)
bool IRAM_ATTR timer_isr_callback(void *args) {
  counter++;      // Incrementa o contador
  return pdFALSE; // Retorna falso para não forçar troca de contexto
  // Basicamente ele volta ao contexto anterior (a Task anterior)
}

void app_main(void) {
  // Configuração dos pinos dos LEDs
  gpio_config_t io_conf = {
      .pin_bit_mask = (1ULL << LED1_PIN) |   // Máscara para LED 1
                      (1ULL << LED2_PIN) |   // Máscara para LED 2
                      (1ULL << LED3_PIN),    // Máscara para LED 3
      .mode = GPIO_MODE_OUTPUT,              // Define como saída
      .pull_up_en = GPIO_PULLUP_DISABLE,     // Desabilita pull-up
      .pull_down_en = GPIO_PULLDOWN_DISABLE, // Desabilita pull-down
      .intr_type = GPIO_INTR_DISABLE};       // Desabilita interrupção no GPIO
  gpio_config(&io_conf);                     // Aplica a configuração

  // Apaga todos os LEDs inicialmente
  gpio_set_level(LED1_PIN, 0);
  gpio_set_level(LED2_PIN, 0);
  gpio_set_level(LED3_PIN, 0);

  // Configuração do Timer de Hardware
  timer_config_t config = {
      .divider = 80, // Divisor de clock: 80MHz / 80 = 1MHz (1us por tick)
      .counter_dir = TIMER_COUNT_UP, // Contagem crescente
      .counter_en = TIMER_PAUSE,     // Inicia pausado
      .alarm_en = TIMER_ALARM_EN,    // Habilita alarme
      .auto_reload = true, // Reinicia contagem automaticamente após alarme
  };
  timer_init(TIMER_GROUP, TIMER_IDX,
             &config); // Inicializa o timer com a config

  timer_set_counter_value(TIMER_GROUP, TIMER_IDX,
                          0); // Zera o contador do timer
  timer_set_alarm_value(TIMER_GROUP, TIMER_IDX,
                        1000000); // Define alarme para 1s (1.000.000us)
  timer_enable_intr(TIMER_GROUP, TIMER_IDX); // Habilita interrupção do timer
  timer_isr_callback_add(TIMER_GROUP, TIMER_IDX, timer_isr_callback, NULL,
                         0); // Registra a função de callback da ISR

  timer_start(TIMER_GROUP, TIMER_IDX); // Inicia o timer
  ESP_LOGI(TAG, "Timer iniciado. Controle dos LEDs movido para o loop.");

  while (1) { // Loop infinito
    // Alterna os LEDs baseado no resto da divisão do contador por 3
    switch (counter % 3) {
    case 0:                        // Resto 0
      gpio_set_level(LED1_PIN, 1); // Liga LED 1
      gpio_set_level(LED2_PIN, 0); // Desliga LED 2
      gpio_set_level(LED3_PIN, 0); // Desliga LED 3
      break;
    case 1:                        // Resto 1
      gpio_set_level(LED1_PIN, 0); // Desliga LED 1
      gpio_set_level(LED2_PIN, 1); // Liga LED 2
      gpio_set_level(LED3_PIN, 0); // Desliga LED 3
      break;
    case 2:                        // Resto 2
      gpio_set_level(LED1_PIN, 0); // Desliga LED 1
      gpio_set_level(LED2_PIN, 0); // Desliga LED 2
      gpio_set_level(LED3_PIN, 1); // Liga LED 3
      break;
    }

    ESP_LOGI(TAG, "Led ativo=%d", counter % 3); // Loga qual LED está ativo

    vTaskDelay(pdMS_TO_TICKS(10)); // Aguarda 10ms para aliviar a CPU
  }
}
