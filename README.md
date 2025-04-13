# STM32 Timer Utility Library

This project provides a utility for managing timer-based delays, PWM, and interrupt functionalities using the STM32F10x microcontroller. It abstracts the configuration of TIM1 to TIM4 timers for both time delays and interrupt-based operations in microseconds and milliseconds.

## Table of Contents
- [File Structure](#file-structure)
- [Enum Declaration](#enum-declaration)
- [Timer Utilities](#timer-utilities)
  - [Delay Functions](#delay-functions)
  - [Timer Start Functions](#timer-start-functions)
  - [Time Retrieval](#time-retrieval)
- [Interrupt Utilities](#interrupt-utilities)
- [Timer Stop Functions](#timer-stop-functions)
- [GPIO and Timer Mapping](#gpio-and-timer-mapping)
- [Usage Example](#usage-example)

---

## File Structure

- `timer.h`: Header file for timer functions.
- `gpio_driver.h`: Assumed header for GPIO utilities.
- `main.c`: Contains the logic to initialize and use the timer functionalities.

---

## Enum Declaration

```c
enum timers { T, T1, T2, T3, T4 };
```

This is used to reference timers easily in function calls.

---

## Timer Utilities

### Delay Functions

- `void TDelay_Micros(char timer, int microS);`  
  Delays execution for `microS` microseconds using the specified timer.

- `void TDelay_Millis(char timer, int milliS);`  
  Delays execution for `milliS` milliseconds using the specified timer.

- `void TDelay_Micro(int microS);`  
- `void TDelay_Milli(int milliS);`  
  Same as above, but uses Timer 4 by default.

---

### Timer Start Functions

- `TIM_TypeDef * timer_start_micros(char timer, int microS);`  
- `TIM_TypeDef * timer_start_millis(char timer, int millis);`  
  Starts a timer with appropriate prescaler for the specified delay.

---

### Time Retrieval

- `int get_time_micros(char timer);`  
- `int get_time_millis(char timer);`  
  Returns the current elapsed time of the timer in microseconds or milliseconds.

---

## Interrupt Utilities

- `void timer_irq_micro_start(char timer, int microS);`  
- `void timer_irq_millis_start(char timer, int milliS);`  
  Enables interrupt on a timer after setting it for a given duration.

- `void timer_irq_Rflag(char timer);`  
  Clears the update interrupt flag (`SR` register).

- `void timer_irq_stop(char timer);`  
  Disables the timer interrupt.

---

## Timer Stop Functions

- `void timer_stop(char timer);`  
  Disables and resets the timer and its peripheral clock.

---

## GPIO and Timer Mapping

These functions map the GPIO port/pin to their respective timers and channels for PWM or Compare operations.

- `char get_channel(char port, char pin);`  
  Returns the channel number (1-4) associated with a specific pin.

- `char get_timer_pin(char port, char pin);`  
  Returns the timer number used for that GPIO pin.

- `TIM_TypeDef * get_timer_address(char port, char pin);`  
  Returns the `TIM_TypeDef` address (timer register base) for that pin.  
  ⚠️ Note: This function's implementation was incomplete in the snippet.

---

## PWM and Compare Mode Functions

- `void timer_compare_micro(char port, char pin, int arr, int compare);`  
- `void timer_compare_milli(char port, char pin, int arr, int compare);`  
- `void timer_pwm_micro(char port, char pin, int arr, int compare);`  
- `void timer_pwm_milli(char port, char pin, int arr, int compare);`  

These functions will:
1. Configure the timer for Compare/PWM mode.
2. Set ARR (Auto Reload Register) and Compare value.
3. Start signal generation on a specific pin and timer.

---

## Usage Example

```c
int main(void) {
    // Delay example
    TDelay_Millis(T2, 1000);  // Delay 1 second using TIM2

    // Start Timer Interrupt
    timer_irq_micro_start(T3, 500); // 500 µs timer with interrupt

    // Get elapsed time
    int micros = get_time_micros(T2);

    // Stop timer
    timer_stop(T2);
}
```

---

## Notes

- Make sure to define the corresponding IRQ Handlers like `TIM2_IRQHandler()` in your main file.
- Ensure system clock is configured correctly for the prescaler settings used (72 MHz expected).
- The GPIO-Timer mapping logic may need refinement based on your specific board layout.

---

## License

MIT License — Feel free to use and modify as per your project needs.
