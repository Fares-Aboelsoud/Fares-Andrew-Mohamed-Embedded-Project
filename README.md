# KH5048CEM – Human Machine Interface
**Temperature Monitor with Adjustable Limits on the ATmega328P**

| Field | Details |
| :--- | :--- |
| **Module** | KH5048CEM – Embedded System Design & Development |
| **Group Members** | Andrew Khalil \| Mohamed Hussein \| Fares Aboelsoud |
| **Microcontroller** | AVR ATmega328P (Arduino Uno R3 + 16×2 LCD Keypad Shield) |
| **Language** | Native AVR C (direct register-level programming, no Arduino libraries) |

---

## Project Overview

A car-dashboard style Human Machine Interface (HMI) that reads ambient temperature from an LM35 sensor via the ATmega328P's ADC, displays the live temperature and configurable High/Low limits (`LH` / `LL`) on a 16×2 LCD, and triggers an audible/visual alarm (buzzer + LED) whenever the temperature leaves the valid range (`NOK`).

* **Default Limits:** `LL = 20 °C`, `LH = 38 °C`
* **Controls:** `UP` / `DOWN` adjust `LH`; `RIGHT` / `LEFT` adjust `LL` (bounded within `0 ≤ LL < LH ≤ 99`)
* **Alert Condition:** Active when `Temp < LL` or `Temp > LH`

---

## Repository Files

### Source Code
| File | Description |
| :--- | :--- |
| **[`temp_monitor.ino`](temp_monitor.ino)** | Main application loop handling temperature evaluation, button limit updates, alert logic, and 16×2 LCD rendering. |
| **[`adc.h`](adc.h)** | ADC driver header file (function prototypes). |
| **[`adc.c`](adc.c)** | ADC peripheral initialization (AVcc 5 V reference, 125 kHz clock) and 10-bit channel reading. |
| **[`lcd.h`](lcd.h)** | 16×2 LCD driver header file. |
| **[`lcd.c`](lcd.c)** | 4-bit HD44780 LCD driver using direct `PORTB` / `PORTD` register control. |
| **[`io.h`](io.h)** | I/O module header defining shield button enums and hardware helper prototypes. |
| **[`io.c`](io.c)** | Keypad button decoding (`A0`), averaged LM35 temperature conversion (`A1`), and LED/buzzer alert control. |

### Documentation, BOM & Testing
| File | Description |
| :--- | :--- |
| **[`project_report.md`](Project_Report.md)** | Full consolidation report covering architecture, implementation, safety rules, test cases, and cost analysis. |
| **[`bom.csv`](bill_of_materials.csv)** | Prototype Bill of Materials with local Egyptian market pricing (≈ 655 EGP). |
| **[`optimized_bom.csv`](optimized_bom.csv)** | Optimized standalone product BOM for 1,000-unit mass production (≈ 226 EGP, ~66% cost reduction). |
| **[`test_cases.robot`](temperature_monitor_test_cases.robot)** | Robot Framework test suite covering all 10 functional and boundary test cases (`T1`–`T10`). |

---

## Hardware Pinout

| Component | Arduino / Shield Pin | ATmega328P Pin | Function |
| :--- | :--- | :--- | :--- |
| **16×2 LCD (RS, EN, BL)** | Pins 8, 9, 10 | `PB0`, `PB1`, `PB2` | LCD control & backlight |
| **16×2 LCD (D4–D7)** | Pins 4–7 | `PD4`–`PD7` | 4-bit LCD data bus |
| **Keypad Buttons** | `A0` | `PC0` (ADC0) | Resistor-ladder button input |
| **LM35 Temp Sensor** | `A1` | `PC1` (ADC1) | Analog temperature input (10 mV/°C) |
| **Active Buzzer** | Pin 3 | `PD3` | Audible `NOK` alert |
| **Status LED (+ / -)** | Pin 12 / Pin 13 | `PB4` / `PB5` | Visual `NOK` alert (`PB5` driven `LOW` as GND) |

---

## Video
[`Demo_Video`](Demo_Video.mp4)
