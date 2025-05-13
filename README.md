>[!IMPORTANT]
>This branch is still work in Progress.
>
>**Known Issue:** When reading sensor values the first read works great. But If you attempt the 2nd data fetch, the SDA and SCL line both stay low.

# REE_HS4001_RH850_Exercise

This project demonstrates how to interface an I2C-based temperature and humidity sensor with the Renesas [RH850/F1KMS-1/4](https://www.renesas.com/en/products/microcontrollers-microprocessors/rh850-automotive-mcus/y-ask-rh850f1km-s4-v3-rh850f1km-s4-and-rh850f1km-s2-starter-kit?srsltid=AfmBOopyt9SV0fHnDLxiTXRxzb-_vKV4NsoCn9Lvpf3vnacnWSLGJH2V) microcontroller using the Smart Configurator. The sensor used can be either the [THERMO 8 Click](https://www.mikroe.com/thermo-8-click) from MikroE or the [Renesas QCIOT-HS4001POCZ](https://www.renesas.com/en/products/sensor-products/environmental-sensors/humidity-temperature-sensors/qciot-hs4001pocz-relative-humidity-sensor-pmod-board?srsltid=AfmBOopQm-yDBKPeVT2nIua_tZPkWGW48-JHyJWXf6HlOwa8ZNXakaJM). The application reads sensor data periodically using a timer, toggles an LED on each read, and transmits the temperature via UART. Additional features include real-time clock integration, low power operation via STOP mode, and optional DMA-based data transfer.


| ![](https://www.renesas.com/sites/default/files/styles/two_columns/public/qciot-hs4001pocz-pmod-board.jpg?itok=tpyNxCd1) | ![](https://media.rs-online.com/image/upload/bo_1.5px_solid_white,b_auto,c_pad,dpr_2,f_auto,h_399,q_auto,w_710/c_pad,h_399,w_710/Y2727643-01?pgw=1) |
| :------------------------------------------------------------------------------------------------------------------------: | :----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------: |
|                                                     QCIOT-HS4001POCZ                                                     |                                                                                                   Renesas RH850-F1KM Board                                                                                                   |

## Version History & Task Breakdown

✔️ ❌ ⏳

### v1.0.0 – Initial Hardware Setup

- Connected RH850/F1KMS-1/4 with an I2C sensor ✔️
- Verified I2C pin connections using RH850 and sensor schematics. ✔️
- Generated base project using Renesas Smart Configurator. ✔️

### v1.1.0 – Basic Sensor Communication

- Configured I2C (RIIC module) for communication with the sensor. ✔️
- Implemented basic readout of temperature and humidity values. ✔️

> [!IMPORTANT]  
> It was suggested not to use the HS4001 Sensor. Since it is to difficult to write a driver for it. That's the reason why I have ordered a [HS3001](US082-HS3001EVZ) Sensor Module.

### v1.2.0 – Periodic Sensor Readout

- Added a timer to periodically trigger temperature/humidity readings. ✔️
- Toggled an onboard LED on each read operation. ✔️
- Printed temperature values over UART to a serial terminal. ✔️

### v1.3.0 – Timestamp Integration

- Configured RTCA (Real-Time Clock) peripheral. ✔️
- Extended UART output to include timestamp information with sensor data. ✔️

_Optional: You can use DMA to transfer both the data without CPU intervention (v1.4.0)_ ⏳

### v2.0.0 – Low Power Mode Handling

- Implemented STOP Mode for low power consumption. ✔️
- On button (S1) press, sensor readings are halted and MCU enters STOP mode. ✔️
- On subsequent button press, MCU wakes and resumes temperature monitoring. ✔️

_Optional: Wake-up using RTCA after a defined interval (e.g., 1 minute) as an alternative to button-based wake-up (v2.1.0)_ ✔️


## Project Documentation

### 🧩Installation of the required tools and first Setup

- [RH850 Smart Configurator](https://www.renesas.com/en/software-tool/rh850-smart-configurator?srsltid=AfmBOor4SMtzOW-0Qb2vgJXUqwqGRnnhWiG-TNuVDqWrxINEY4_mrwob)  
- [CS+ for CC](https://www.renesas.com/en/software-tool/cs?srsltid=AfmBOop0g-pdqgDfdsOyezCdQ4QEG7y1Gxbd2lbEj7-URI7VAc9_erg4)

**Project Setup:**
1. Create a new project in **CS+ for CC**.
2. Select device part number: [R7F701649](https://www.renesas.com/en/products/microcontrollers-microprocessors/rh850-automotive-mcus/rh850f1km-s4-high-end-automotive-microcontrollers-ideal-body-applications?srsltid=AfmBOop97VEppKgARmjWsGN98QKK9btjE7cEQr401Q0O47m-ZGjNSlWP).
3. Connect the target board using the **E2 Debugger**.
4. In the debugger **Properties**:
   - Enable **"Power target from emulator"**
   - Set **Clock frequency** to **16 MHz**
5. Build the project. The target board's LED should light up, confirming successful setup.

### 🌡Driver for the HS4001 Temperature & Humidity Sensor

#### Required Wiring

| Signal | Pin     | Wire Color |
|--------|---------|------------|
| VCC5   | 3V3     | Green      |
| GND    | GND     | Blue       |
| SCL    | P0.11   | Yellow     |
| SDA    | P0.12   | Orange     |

> [!CAUTION]  
> **Remove the jumper for LED1.**  
> P0_11 is connected to the LED according to the datasheet. If used for I2C output, current will flow continuously through `R_pullUpBus + R_LED + LED`, causing the bus to remain busy.  
> A faintly glowing LED indicates this issue due to unintended current draw.

#### Interrupt Configuration

> [!IMPORTANT]  
> Default interrupt settings in CS+ are incorrect.  
> Follow the configuration steps outlined in 📓[Interrupt setting 1.pdf](<file:///C:\Users\a5153359\Documents\12 - Training Exercise RH850\Interrupt  setting 1.pdf>).  
> Document shared by **Alexandros Zikas** via MS Teams on 10.04.2025.

#### Documentation & Datasheets

- 📚[User Manual of the Evaluation Kit](https://www.renesas.com/en/document/mah/rh850f1x-starter-kit-v3-user-manual-hardware?r=1261056)  
- 📚[Datasheet HS3001](https://www.renesas.com/en/document/dst/hs3xxx-datasheet?r=417401)

#### Sensor Selection

Switch to the [HS3001](https://www.renesas.com/en/products/general-parts/hs3001-high-performance-relative-humidity-and-temperature-sensor?srsltid=AfmBOorPxZifqnYRBswt10uAaMbjAJPkpxP3Ljtc3fhT7xxDIAdM08sg) sensor for ease of driver development.

#### Driver Workflow

1. **Send Measurement Request**  
   Send the sensor address (`0x44`) with a read bit. The sensor responds with an ACK and begins the measurement cycle.

2. **Wait**  
   Delay the read by approx. 1 second using a `for` loop to allow the sensor to complete measurement.

3. **Fetch Measurement Data**  
   Re-send the sensor address with a read bit. The sensor transmits 4 bytes of data. After reception, the master must send a NACK and then issue a stop condition.

> [!CAUTION]  
> When using **DMA**, the NACK bit is not transmitted.  
> This prevents the sensor from finalizing the communication, leading to issues with subsequent measurements.  
> The data line stays low and the sensor doesn’t send new data.

#### Sample Code

```c
R_Config_RIIC0_Master_Send(SENSOR_SLAVE_ADDRESS, "", 0); // Measurement request
R_Config_RIIC0_Master_Receive(0x44, i2c_receive_buffer, I2C_RECEIVE_SIZE); // Data fetch

void calc_humidity(TemperatureMeasurement *measurement)
{
    measurement->relative_humidity = (measurement->relative_humidity_raw / 16383.0f) * 100.0f;
}

void calc_temperature(TemperatureMeasurement *measurement)
{
    measurement->temperature_celsius = ((measurement->temperature_raw / 16383.0f) * 165.0f) - 40.0f;
}

```
### 👨‍💻 Logging using UART

Logging over UART follows the same approach as with other MCUs.

```c
void uart_send_log_humidity_and_temperature(TemperatureMeasurement measurement)
{
    volatile MD_STATUS err = MD_OK;
    uint16_t length = 0;

    // Format and store the message in the transmission buffer
    length = sprintf(g_tx_buf1,
        "+-------------------------------+\r\n"
        "| %s | %5.2f C | %5.2f %%  |\r\n"
        "+-------------------------------+\r\n",
        measurement.timestamp, measurement.temperature_celsius, measurement.relative_humidity);

    // Send only the used buffer length
    err = R_Config_UART0_Send(g_tx_buf1, length);

    if (err != MD_OK)
    {
        g_event = EVT_ERROR_DETECTED;
        return;
    }

    // Wait for send to complete
    while (uart_isr_status_flag_send_complete != 1);

    uart_isr_status_flag_send_complete = 0;
}
```

>[!IMPORTANT]
>Ensure the rx_buffer is large enough.
>If it's too small, the MCU will hang in a loop without throwing an error.

### 🕒 Using the RTC

Many Real Time Clocks (RTC) use **BCD (Binary Coded Decimal)** format to store time values. Understanding and converting this format is essential for proper display and handling.

#### Time Format Comparison

| Time Component | Decimal | BCD         | Binary      |
|----------------|---------|-------------|-------------|
| Hour           | 12      | `0001 0010` | `0000 1100` |
| Minute         | 45      | `0100 0101` | `0010 1101` |

- **Binary (bin):** Standard numeric format
- **BCD (Binary Coded Decimal):** Each decimal digit is encoded into a 4-bit nibble

📌 **Note:** Many RTCs output values in BCD format. Conversion to binary is required to display or process the values correctly.

#### Conversion Functions

```c
// BCD to Decimal
uint8_t bcdToDec(uint8_t val) {
    return ((val >> 4) * 10) + (val & 0x0F);
}

// Decimal to BCD
uint8_t decToBcd(uint8_t val) {
    return ((val / 10) << 4) | (val % 10);
}
```

### 🔋 Low Power Mode

The RH850 supports multiple low power modes (LPMs) to reduce energy consumption. Here's how to use them effectively:

- **HALT Mode** can be triggered directly with the `__halt()` instruction.
- For other LPMs, use the **Standby Controller (STC)**.
- Initialize the STC like any other peripheral.
- Call the appropriate API to enter the selected low power mode.
- Configure the **wake-up source** in the settings to ensure the MCU can resume operation.

> [!NOTE]  
> In sleep mode, most peripherals stop functioning.  
> Only the RTC continues counting during this time.


### 🧠 Direct Memory Access

DMA integration was one of the more complex and interesting parts of the project. Here's a summary of the key findings and implementation steps.

#### Key Learnings

- **Memory Protection:**  
  Some areas of the CPU are protected. Use the **PEG register** to grant peripheral access to these regions.

- **Address Offsets:**  
  Global RAM cells have different addresses for CPU and peripheral access. Retention RAM appears to be unaffected by this.

- **Retention RAM:**  
  Data is written to **Retention RAM**, which retains its values even without power.

> [!NOTE]  
> Unlike regular RAM, Retention RAM maintains its state after power-off. Useful for storing persistent data without non-volatile memory.

#### DMA Constraints with I2C

- Standard I2C drivers use **blocking code**, which halts the CPU during data transfer.
- DMA avoids this by using interrupts.
- The DMA is triggered via the **DRR register interrupt**.
- To enable DMA usage:
  - Disable the regular I2C ISR that handles data shifts from DRR.
  - Manually handle data transfer in the **DMA interrupt routine**.

#### Initialization Example

```c
void R_Config_DMAC00_Create_UserInit(void)
{
    // Init DMA
    PDMA0.DCEN0 = 0x0;
    PDMA0.DDA0 = DMA_SRC_ADDRESS;              // Destination (CPU memory)
    PDMA0.DSA0 = (uint32_t) &RIIC0.DRR;        // Source (I2C data register)
    PDMA0.DCEN0 = 0x1;

    // Enable PEG for local RAM access
    PEG.SP.UINT32 |= 0x00000001;               // Enable SPID access

    PEG.G0MK.UINT32 = (END_OF_RAM - START_OF_RAM) & 0xFFFFF000;

    PEG.G0BA.UINT32 = START_OF_RAM + 0x00000017; // Enable, read/write, SPID0 allowed
}
```

> [!Important]
> The I2C peripheral makes a "dummy read" when the measurement start is being called. Make sure to be aware of that. In this project, I just start the transfer and let it write 5 bytes then. When reading the result I just start with the 2nd address. This is not memory efficient but easier

- Since the I2C interrupt is disabled, the incoming data must be handled manually -> managed in the DMA transfer finished ISR
- The interrupt must be called 1 byte before the final

```c
void r_Config_DMAC00_dmac00_interrupt(void)
{
    R_Config_DMAC00_Stop();

    // Toggle GPIO to check timing via logic analyzer
    PORT.P0 = (PORT.P0 == _PORT_Pn6_OUTPUT_LOW) ? _PORT_Pn6_OUTPUT_HIGH : _PORT_Pn6_OUTPUT_LOW;

    // Wait while reception is still in progress
    while ((RIIC0.SR2.UINT32 & (1U << 5)) == 1);

    PORT.P0 = (PORT.P0 == _PORT_Pn6_OUTPUT_LOW) ? _PORT_Pn6_OUTPUT_HIGH : _PORT_Pn6_OUTPUT_LOW;

    // Set NACK
    RIIC0.MR3.UINT32 |= (1U << 4); // Disable ACK write protection
    RIIC0.MR3.UINT32 |= (1U << 3); // Enable ACKBT (send NACK)

    // Wait for data ready
    while ((RIIC0.SR2.UINT32 & (1U << 5)) == 0);

    PORT.P0 = (PORT.P0 == _PORT_Pn6_OUTPUT_LOW) ? _PORT_Pn6_OUTPUT_HIGH : _PORT_Pn6_OUTPUT_LOW;

    // Send stop condition
    RIIC0.SR2.UINT32 = ~(1U << 3);
    RIIC0.CR2.UINT32 |= _RIIC_STOP_CONDITION_REQUEST;

    // Write final byte manually
    *(volatile uint32_t *) 0xFEF01014 = RIIC0.DRR.UINT32;

    // Wait for stop to complete
    while ((RIIC0.SR2.UINT32 & (1U << 3)) == 0);

    // Reset status flags
    RIIC0.SR2.UINT32 = ~(1U << 4);
    RIIC0.SR2.UINT32 = ~(1U << 3);

    g_event = EVT_DMA_TRANSFER_FINISHED;
}
```

> [!IMPORTANT]
> This should all work like a charm from my point of view. Well it does for the first reception. But when I am trying to get a second one, the Data line stays low after I sent the data fetch request. I first thought that the problem occures because I don't sent a NACK after receiving 4 bytes of data but that is still the case after fixing that.
> - Still in contact with the AE Team in order to solve this issue

### 🚧 Program Architecture

The system uses an advanced state machine to control all operational behavior.  
A second timer was added to trigger LED2 rapidly, serving as a visual indicator for the Error State.

![image](https://github.com/user-attachments/assets/b6b3023e-4e30-4441-89ea-da3f67898e67)

---

**More Information can be found in the corresponding Obsidian Document**
