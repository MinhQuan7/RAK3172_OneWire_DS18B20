/*************************************************************
  Download latest library here:
    Github:                     https://github.com/MinhQuan7
    ERa website:                https://e-ra.io
    ERa blog:                   https://iotasia.org
    ERa forum:                  https://forum.eoh.io
    Follow us:                  https://www.fb.com/EoHPlatform
 *************************************************************/

#include <Arduino.h> 
#include <Rak3172_Canopus.h>

// Định nghĩa các chân và các hàm hỗ trợ
#define ONEWIRE_PIN_DATA GPIO_PIN_11
#define ONEWIRE_PORT_DATA GPIOA

//________________________________OneWire - DS18B20 __________________________

 // Định nghĩa các chân và các hàm hỗ trợ
#define ONEWIRE_PIN_DATA GPIO_PIN_11
#define ONEWIRE_PORT_DATA GPIOA

void set_pin_as_output(GPIO_TypeDef *port, uint16_t pin) {
  // Cấu hình chân GPIO thành chế độ xuất (output).
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(port, &GPIO_InitStruct);
}

void set_pin_as_input(GPIO_TypeDef *port, uint16_t pin) {
    // Cấu hình chân GPIO thành chế độ nhập (input).
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(port, &GPIO_InitStruct);
}

// Đặt lại DS18B20
void onewire_reset(void) {
    // Đặt lại cảm biến DS18B20.
    set_pin_as_output(ONEWIRE_PORT_DATA, ONEWIRE_PIN_DATA);  // Cấu hình chân dữ liệu thành chế độ xuất.
    HAL_GPIO_WritePin(ONEWIRE_PORT_DATA, ONEWIRE_PIN_DATA, GPIO_PIN_RESET);  // Kéo chân dữ liệu xuống mức thấp.
    delayMicroseconds(480);  // Chờ 480 microseconds.
    HAL_GPIO_WritePin(ONEWIRE_PORT_DATA, ONEWIRE_PIN_DATA, GPIO_PIN_SET);  // Kéo chân dữ liệu lên mức cao.
    delayMicroseconds(480);  // Chờ thêm 480 microseconds.
    set_pin_as_input(ONEWIRE_PORT_DATA, ONEWIRE_PIN_DATA);  // Cấu hình chân dữ liệu thành chế độ nhập.
    delayMicroseconds(480);  // Chờ thêm 480 microseconds.
}

// Ghi 1 bit vào DS18B20
void onewire_write_bit(uint8_t bit) {
    set_pin_as_output(ONEWIRE_PORT_DATA, ONEWIRE_PIN_DATA);
    if (bit) { //Nếu bit la 1
        HAL_GPIO_WritePin(ONEWIRE_PORT_DATA, ONEWIRE_PIN_DATA, GPIO_PIN_RESET);
        delayMicroseconds(1);
        HAL_GPIO_WritePin(ONEWIRE_PORT_DATA, ONEWIRE_PIN_DATA, GPIO_PIN_SET);
        delayMicroseconds(60);
    } else {
        HAL_GPIO_WritePin(ONEWIRE_PORT_DATA, ONEWIRE_PIN_DATA, GPIO_PIN_RESET);
        delayMicroseconds(60);
        HAL_GPIO_WritePin(ONEWIRE_PORT_DATA, ONEWIRE_PIN_DATA, GPIO_PIN_SET);
        delayMicroseconds(1);
    }
    set_pin_as_input(ONEWIRE_PORT_DATA, ONEWIRE_PIN_DATA);
}

// Đọc 1 bit từ DS18B20
uint8_t onewire_read_bit(void) {
    uint8_t bit = 0;
    set_pin_as_output(ONEWIRE_PORT_DATA, ONEWIRE_PIN_DATA);
    HAL_GPIO_WritePin(ONEWIRE_PORT_DATA, ONEWIRE_PIN_DATA, GPIO_PIN_RESET);
    delayMicroseconds(1);
    HAL_GPIO_WritePin(ONEWIRE_PORT_DATA, ONEWIRE_PIN_DATA, GPIO_PIN_SET);
    set_pin_as_input(ONEWIRE_PORT_DATA, ONEWIRE_PIN_DATA);
    delayMicroseconds(15);
    bit = HAL_GPIO_ReadPin(ONEWIRE_PORT_DATA, ONEWIRE_PIN_DATA);
    delayMicroseconds(45);
    return bit;
}

// Ghi 1 byte vào DS18B20
void onewire_write_byte(uint8_t data) {
    // Ghi một byte vào DS18B20.
    for (uint8_t i = 0; i < 8; i++) {
        onewire_write_bit(data & 0x01);  // Ghi bit thấp nhất của byte.
        data >>= 1;  // Dịch chuyển byte sang phải 1 bit.
    }
}

// Đọc 1 byte từ DS18B20
uint8_t onewire_read_byte(void) {
    // Đọc một byte từ DS18B20.
    uint8_t data = 0;
    for (uint8_t i = 0; i < 8; i++) {
        data |= (onewire_read_bit() << i);  // Đọc từng bit và dịch chuyển vào byte.
    }
    return data;  // Trả về giá trị byte đọc được.
}

// Đọc nhiệt độ từ DS18B20
float read_temperature(void) {
    uint8_t temp_LSB, temp_MSB;
    int16_t temp;

    // Khởi động lại cảm biến
    onewire_reset();

    // Gửi lệnh Skip ROM
    onewire_write_byte(0xCC);

    // Gửi lệnh Convert T
    onewire_write_byte(0x44);

    // Chờ cảm biến hoàn thành quá trình chuyển đổi nhiệt độ
    delay(750);

    // Khởi động lại cảm biến
    onewire_reset();

    // Gửi lệnh Skip ROM
    onewire_write_byte(0xCC);

    // Gửi lệnh Read Scratchpad
    onewire_write_byte(0xBE);

    // Đọc 2 byte dữ liệu nhiệt độ
    temp_LSB = onewire_read_byte();
    temp_MSB = onewire_read_byte();

    // Chuyển đổi dữ liệu nhiệt độ
    temp = ((int16_t)temp_MSB << 8) | temp_LSB;

    // Trả về giá trị nhiệt độ dạng float
    return (float)temp / 16.0;
}

//_________________________________END SETUP ONEWIRE___________________________


void setup() {
  Serial.begin(115200);
  Serial.println("RAK3172_Canopus -  OneWire Protocol");
  Serial.println("------------------------------------------------------");
  Serial.println("This library support DS18B20");
  Serial.println("------------------------------------------------------");
  init_io(); //Enable I/O
  enable_Vss3();
  startTime = millis();
}


/*
_____________________________CAU HINH FRAME TRUYEN_______________________________________
 [BEGIN]    [LEN]   [FRAME TYPE]    [FUNCTION]    [MAC ADDR]    [DATA]    [CRC]   [END]

BEGIN: OXFE
END  : OXEF
MAC  : MAC ADDRESS NODE
LEN  : Length frame
CRC  : CRC 8 bit frame
*/
void loop() {
  float temperature = read_temperature();
  Serial.print("Temperature: ");
  Serial.print(temperature, 2);  
  Serial.println(" *C");
  delay(100); 
}


