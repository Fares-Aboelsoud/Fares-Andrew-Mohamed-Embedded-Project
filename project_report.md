# KH5048CEM Embedded System Design & Development
## Element 2 – Human Machine Interface
**Temperature monitor with adjustable limits on the ATmega328P**

| Field | Details |
| :--- | :--- |
| **Group members** | Andrew Khalil \| Mohamed Hussein \| Fares Aboelsoud |
| **GitHub repository** | <https://github.com/Fares-Aboelsoud/Fares-Andrew-Mohamed-Embedded-Project> |
| **Supporting files** | [`bom.csv`](bom.csv) \| [`optimized_bom.csv`](optimized_bom.csv) \| [`test_cases.robot`](test_cases.robot) |

---

## 1. Introduction

Our task was to build a small car-dashboard style HMI: show a temperature, let the user set an acceptable range, and warn them when the value leaves it. We used an LM35 sensor read through the ATmega328P's ADC, a 16×2 LCD, push buttons for the High Limit (`LH`) and Low Limit (`LL`), and an LED plus buzzer as the alarm. Everything is written in native C using the registers directly, with no Arduino functions or libraries, even though the hardware is an Uno board.

## 2. Design Architecture

An LCD keypad shield was used in conjunction with the Arduino Uno. This device has a built-in display and five buttons, which minimizes external connections. The LM35 analog temperature sensor is connected to `A1`. Button input is multiplexed through a resistor chain to the `A0` pin. The LCD interface is accessed through pins `PB0`–`PB2` and `PD4`–`PD7`. The buzzer output is accessed through `PD3` and the LED is controlled by `PB4`, with `PB5` kept in logic `LOW` to provide a ground.

Three modules have been implemented to provide a layered software architecture so that the main program runs without direct register accesses:

* **`adc.c` / `adc.h`:** Initializes the ADC ($5\text{ V}$ voltage reference, $125\text{ kHz}$ clock) and reads channels.
* **`io.c` / `io.h` and `lcd.c` / `lcd.h`:** Translates ADC values into button status or temperatures, activates the alarm, and controls the 16×2 LCD.
* **`main` (`temp_monitor.ino`):** Manages low and high temperature tracking, temperature evaluation, and display rendering.

After start-up the program loops forever: read the temperature, read the buttons, switch the alarm on or off, update the screen, then wait $100\text{ ms}$. The brief mentions one button per limit, but going back down would then need a long-press trick. Since the shield had four arrows, we used `UP`/`DOWN` for `LH` and `RIGHT`/`LEFT` for `LL`, so each limit can move both ways with simple taps.

## 3. Implementation

The LM35 gives $10\text{ mV/}^\circ\text{C}$, so with a $5\text{ V}$ reference the conversion is:

$$T = \frac{\text{ADC} \times 500}{1024}$$

We average four readings to stop the display flickering and use a 32-bit intermediate to avoid overflow. Buttons are decoded by comparing the `A0` reading against thresholds halfway between each button's voltage.

Each limit change only happens if the result stays valid: $0 \le \text{LL} < \text{LH} \le 99$, so the limits can never cross. Defaults are `LL = 20` and `LH = 38`. The alarm is on when $T < \text{LL}$ or $T > \text{LH}$, so the limits themselves count as `OK`. A $180\text{ ms}$ wait after each press works as debouncing and also gives auto-repeat when a button is held. On the LCD we overwrite text in place instead of clearing, which removed flicker.

## 4. Coding Rules and Safety

Functions follow a `Module_Action` naming style (`Adc_ReadChannel`, `Io_SetAlert`), internal helpers are `static`, and every header has an include guard. We used fixed-width types throughout and masked the ADC channel bits. LCD writes only touch `PD4`–`PD7` so they never disturb the buzzer on `PD3`, and all alarm outputs start `LOW` so the board never powers up beeping.

## 5. Code Submission

All source code is hosted at <https://github.com/Fares-Aboelsoud/Fares-Andrew-Mohamed-Embedded-Project>, built with `avr-gcc` and flashed with `avrdude`.

## 6. Software Test Cases

To test the alarm exactly at the limits, we swapped the LM35 for a potentiometer and set exact voltages ($200\text{ mV} = 20\ ^\circ\text{C}$, $391\text{ mV} = 39\ ^\circ\text{C}$). Defaults `LL = 20` and `LH = 38` were used. Automated verification cases are also provided in [`test_cases.robot`](test_cases.robot).

| ID | Test | Result | Status |
| :--- | :--- | :--- | :--- |
| **T1** | Power on | `LL:20  LH:38`, live temperature, `OK` | Pass |
| **T2** | Tap each arrow once | Only the matching limit changes by 1 | Pass |
| **T3** | Room temperature vs thermometer | Within $\pm 1\ ^\circ\text{C}$ | Pass |
| **T4** | `LH = 98`, press `UP` 3 times | Stops at 99 | Pass |
| **T5** | `LL = 1`, press `LEFT` 3 times | Stops at 0 | Pass |
| **T6** | `LL = 36`, `LH = 38`, press `RIGHT` 3 times | `LL` stops at 37 | Pass |
| **T7** | Input $195\text{ mV}$ ($19\ ^\circ\text{C}$) | `NOK`, LED and buzzer on | Pass |
| **T8** | Input $200\text{ mV}$ / $386\text{ mV}$ ($20\ ^\circ\text{C}$ / $38\ ^\circ\text{C}$) | `OK` on both limits | Pass |
| **T9** | Input $391\text{ mV}$ ($39\ ^\circ\text{C}$) | `NOK`, LED and buzzer on | Pass |
| **T10** | Warm the LM35 above `LH` by hand | Alarm on, clears when cooled | Pass |

## 7. Bill of Materials

Full spreadsheet available in [`bom.csv`](bom.csv).

| Component | Qty | Price (EGP) |
| :--- | :---: | :---: |
| Arduino Uno R3 (compatible) | 1 | 350 |
| LCD keypad shield (16×2 + 5 buttons) | 1 | 150 |
| LM35DZ temperature sensor | 1 | 75 |
| 5 V buzzer, LED and $220\ \Omega$ resistor | 1 each | 10 |
| Jumpers and USB cable | 1 set | 70 (est.) |
| **Total** | | **≈ 655** |

The Uno and the shield are almost three quarters of the cost, mostly for parts only needed during development.

## 8. Optimised BOM for a Stand-alone Product

For a 1,000-unit car product we would use a bare ATmega328PB on our own PCB running from its internal oscillator, a bare LCD1602, two tactile switches as in the brief, an NTC thermistor instead of the LM35, a transistor to drive the buzzer, and a $12\text{ V}$ input stage for the car supply. Full spreadsheet available in [`optimized_bom.csv`](optimized_bom.csv).

| Component | Price (EGP) |
| :--- | :---: |
| ATmega328PB-AU microcontroller | 57.53 |
| LCD1602 display | 117.76 |
| NTC sensor, 2 switches, buzzer, driver, LED (est.) | 14.00 |
| 12 V regulator and protection (est.) | 13.50 |
| Passives and PCB (est.) | 23.12 |
| **Total** | **≈ 226 EGP** |

That is about 66% cheaper than the prototype.

## 9. Limitations

There is no hysteresis, so the buzzer can flicker when the temperature sits on a limit. A disconnected sensor isn't detected, and the limits reset at power-off. Next time we would add hysteresis, a sensor-fault check and EEPROM storage for the limits, and replace the blocking delays with a timer interrupt.

## 10. Conclusion

We built a working temperature HMI in native C with our own ADC and LCD drivers and a clear layered structure. Boundary testing confirmed the alarm and limit logic, and a stand-alone version could cost about a third of the prototype.