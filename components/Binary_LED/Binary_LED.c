#include <stdio.h>
#include "Binary_LED.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "esp_timer.h"

static esp_timer_handle_t blink_timer;
static binary_led_t *led_ptr;
static int on_time_ms = 500;
static int off_time_ms = 500;
static bool led_state = false;

void blink_timer_callback(void *arg) {
    led_state = !led_state;
    bin_set_led(led_ptr, led_state);
    
    // Restart timer with new duration depending on LED state
    int next_interval = led_state ? on_time_ms : off_time_ms;
    esp_timer_start_once(blink_timer, next_interval * 1000);
}

void bin_led_init(binary_led_t *led, int pin)
{
    led->pin = pin;

    gpio_config_t config = {
        .pin_bit_mask = (1ULL << pin),
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE};
    
    gpio_config(&config);
    gpio_set_level(pin, 0);
    led->state = 0;
}

void bin_led_update(binary_led_t *led){
    gpio_set_level(led->pin, led->state);
}

void bin_set_led(binary_led_t *led, int value){
    if (led->state != value)
    {
        led->state = value;
        bin_led_update(led);
    }
}

void bin_led_blink_start(binary_led_t *led, int MS_on, int MS_off) {
    if (blink_timer != NULL) {
        esp_timer_stop(blink_timer);
        esp_timer_delete(blink_timer);
    }
    
    led_ptr = led;
    on_time_ms = MS_on;
    off_time_ms = MS_off;
    
    esp_timer_create_args_t timer_args = {
        .callback = &blink_timer_callback,
        .arg = NULL,
        .name = "blink_timer"
    };
    
    esp_timer_create(&timer_args, &blink_timer);
    esp_timer_start_once(blink_timer, on_time_ms * 1000);
}

void bin_led_blink_stop() {
    if (blink_timer != NULL) {
        esp_timer_stop(blink_timer);
        esp_timer_delete(blink_timer);
        blink_timer = NULL;
    }
    bin_set_led(led_ptr, 0);
}