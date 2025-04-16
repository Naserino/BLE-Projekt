typedef struct 
{
    int pin;
    int state;
}binary_led_t;

void bin_led_init(binary_led_t *led, int pin);
void bin_led_update(binary_led_t *led);
void bin_set_led(binary_led_t *led, int state);
void bin_led_blink_start(binary_led_t *led, int MS_on, int MS_off);
void bin_led_blink_stop();