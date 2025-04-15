# REE_HS4001_RH850_Exercise

This project demonstrates how to interface an I2C-based temperature and humidity sensor with the Renesas [RH850/F1KMS-1/4](https://www.renesas.com/en/products/microcontrollers-microprocessors/rh850-automotive-mcus/y-ask-rh850f1km-s4-v3-rh850f1km-s4-and-rh850f1km-s2-starter-kit?srsltid=AfmBOopyt9SV0fHnDLxiTXRxzb-_vKV4NsoCn9Lvpf3vnacnWSLGJH2V) microcontroller using the Smart Configurator. The sensor used can be either the [THERMO 8 Click](https://www.mikroe.com/thermo-8-click) from MikroE or the [Renesas QCIOT-HS4001POCZ](https://www.renesas.com/en/products/sensor-products/environmental-sensors/humidity-temperature-sensors/qciot-hs4001pocz-relative-humidity-sensor-pmod-board?srsltid=AfmBOopQm-yDBKPeVT2nIua_tZPkWGW48-JHyJWXf6HlOwa8ZNXakaJM). The application reads sensor data periodically using a timer, toggles an LED on each read, and transmits the temperature via UART. Additional features include real-time clock integration, low power operation via STOP mode, and optional DMA-based data transfer.


| ![](https://www.renesas.com/sites/default/files/styles/two_columns/public/qciot-hs4001pocz-pmod-board.jpg?itok=tpyNxCd1) | ![]([https://www.mouser.de/images/renesas/lrg/968-ASKRH850F1KMS4V3_SPL.jpg](https://media.rs-online.com/image/upload/bo_1.5px_solid_white,b_auto,c_pad,dpr_2,f_auto,h_399,q_auto,w_710/c_pad,h_399,w_710/Y2505108-01?pgw=1)) |
| :------------------------------------------------------------------------------------------------------------------------: | :----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------: |
|                                                     QCIOT-HS4001POCZ                                                     |                                                                                                   Renesas RH850-F1KM Board                                                                                                   |

## Version History & Task Breakdown

✔️ ❌ ⏳

### v1.0.0 – Initial Hardware Setup

- Connected RH850/F1KMS-1/4 with an I2C sensor ✔️
- Verified I2C pin connections using RH850 and sensor schematics. ✔️
- Generated base project using Renesas Smart Configurator. ✔️

### v1.1.0 – Basic Sensor Communication

- Configured I2C (RIIC module) for communication with the sensor. ❌
- Implemented basic readout of temperature and humidity values. ❌

> ![Info]
>
> Currently I don't have a HS3001 on my Desk. That's why I just implemented Dummy Values.

### v1.2.0 – Periodic Sensor Readout

- Added a timer to periodically trigger temperature/humidity readings. ✔️
- Toggled an onboard LED on each read operation. ✔️
- Printed temperature values over UART to a serial terminal. ✔️

### v1.3.0 – Timestamp Integration

- Configured RTCA (Real-Time Clock) peripheral. ✔️
- Extended UART output to include timestamp information with sensor data. ✔️

_Optional: You can use DMA to transfer both the data without CPU intervention (v1.4.0)_ ❌

### v2.0.0 – Low Power Mode Handling

- Implemented STOP Mode for low power consumption. ❌
- On button (S1) press, sensor readings are halted and MCU enters STOP mode. ❌
- On subsequent button press, MCU wakes and resumes temperature monitoring. ❌

_Optional: Wake-up using RTCA after a defined interval (e.g., 1 minute) as an alternative to button-based wake-up (v2.1.0)_ ❌

---

**More Information can be found in the corresponding Obsidian Document**
