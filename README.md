# Description

This repository contains the **SEN15901** wind vane and rain gauge driver.

# Dependencies

The driver relies on:

* An external `types.h` header file defining the **standard C types** of the targeted MCU.
* The **embedded utility functions** defined in the [embedded-utils](https://github.com/Ludovic-Lesur/embedded-utils) repository.

Here is the versions compatibility table:

| **sen15901-driver** | **embedded-utils** |
|:---:|:---:|
| [sw1.1](https://github.com/Ludovic-Lesur/sen15901-driver/releases/tag/sw1.1) | >= [sw3.0](https://github.com/Ludovic-Lesur/embedded-utils/releases/tag/sw3.0) |
| [sw1.0](https://github.com/Ludovic-Lesur/sen15901-driver/releases/tag/sw1.0) | >= [sw3.0](https://github.com/Ludovic-Lesur/embedded-utils/releases/tag/sw3.0) |

# Compilation flags

| **Flag name** | **Value** | **Description** |
|:---:|:---:|:---:|
| `SEN15901_DRIVER_DISABLE_FLAGS_FILE` | `defined` / `undefined` | Disable the `sen15901_driver_flags.h` header file inclusion when compilation flags are given in the project settings or by command line. |
| `SEN15901_DRIVER_DISABLE` | `defined` / `undefined` | Disable the SEN15901 driver. |
| `SEN15901_DRIVER_GPIO_ERROR_BASE_LAST` | `<value>` | Last error base of the low level GPIO driver. |
| `SEN15901_DRIVER_TIMER_ERROR_BASE_LAST` | `<value>` | Last error base of the low level timer driver. |
| `SEN15901_DRIVER_ADC_ERROR_BASE_LAST` | `<value>` | Last error base of the low level ADC driver. |
| `SEN15901_DRIVER_WIND_DIRECTION_PULL_UP_RESISTOR_OHMS` | `<value>` | Value of the pull-up resistor placed on the wind direction input (in Ohms). |
| `SEN15901_DRIVER_WIND_SPEED_SAMPLING_TIME_SECONDS` | `<value>` | Time interval in seconds where the wind speed is evaluated. |
| `SEN15901_DRIVER_WIND_DIRECTION_SAMPLING_PERIOD_SECONDS` | `<value>` | Wind direction reading period in seconds. |
