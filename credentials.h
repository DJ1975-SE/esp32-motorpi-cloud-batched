#define WIFI_SSID "yourssid"
#define WIFI_PASSWORD "yourpassword"
#define NTPSERVER "0.de.pool.ntp.org"

// for cloud and local
#define INFLUXDB_URL "the url from influxdatas cloud"
#define INFLUXDB_WRITE_PRECISION WritePrecision::S

// for local v1
#define INFLUXDB_USER "influxdb user"
#define INFLUXDB_PASS "influxdb password"
#define INFLUXDB_DATABASE "influxdb database"

// for cloud
#define INFLUXDB_ORG "generally cloud login, i.e. email address"
#define INFLUXDB_TOKEN "generated from webUI"
#define INFLUXDB_ROWNAME "row name"
#define INFLUXDB_BUCKET "bucket, generated from webUI"

// Fingerprint of Certificate Authority of InfluxData Cloud 2 servers
const char InfluxDbCloud2CAFingerprint[] PROGMEM = "9B:62:0A:63:8B:B1:D2:CA:5E:DF:42:6E:A3:EE:1F:19:36:48:71:1F";

//Linear calculation of AFR
#define AFRFACTOR 7.4

#define REALLY_UPLOAD 1
#define HAVE_INTERRUPTS 1

// I2C Devices
#define HAVE_BME280   1
#define HAVE_MPU6050  1
//#define HAVE_SSD1306   1
#define HAVE_ADS1115  1

// SPI Devices
#define HAVE_MAX6675  1
#define HAVE_ST7735   1
//#define HAVE_MCP3008  1

// W1 find addresses by scanning or NOT readtime over 600ms. drop this
//#define HAVE_W1       1
//DeviceAddress W1_SENSORADDR_1 = {0x28, 0xD7, 0x96, 0xE4, 0x07, 0x00, 0x00, 0x49};
//DeviceAddress W1_SENSORADDR_2 = {0x28, 0xD7, 0x96, 0xE4, 0x07, 0x00, 0x00, 0x49};

//Pinout for ST7735 is in the "User_Setup.h" file in the TFT_eSPI library
//#define TFT_MISO 19
//#define TFT_MOSI 23
//#define TFT_SCLK 18
//#define TFT_CS   15  // Chip select control pin
//#define TFT_DC    4  // Data Command control pin

//Pin for MAX6675 CS
#define MAX6675_1_CS 32
#define MAX6675_2_CS 33

//Pin for RPM counter
#define RPMPIN 25

//Pin for revolution counter
#define REVPIN 26

//we get undefined values at start, limit what we write to the db
#define MAXRPM 50000
#define MAXREV 20000

// Data wire is connected to GPIO15
#define ONE_WIRE_BUS 27

// BME280 Calibration
#define SEALEVELPRESSURE_HPA (1013.25)

//MCP3008 SPI AD Converter
#define MCP3008_SPI_MAX_5V 3600000         ///< SPI MAX Value on 5V pin
#define MCP3008_SPI_MAX_3V 1350000         ///< SPI MAX Value on 3V pin
#define MCP3008_SPI_MAX MCP3008_SPI_MAX_5V ///< SPI MAX Value
#define MCP3008_SPI_ORDER MSBFIRST         ///<  SPI ORDER
#define MCP3008_SPI_MODE SPI_MODE0         ///< SPI MODE
#define MCP3008_VREF 5.1

//ADS1115 I2C AD Converter
  // The ADC input range (or gain) can be changed via the following
  // functions, but be careful never to exceed VDD +0.3V max, or to
  // exceed the upper and lower limits if you adjust the input range!
  // Setting these values incorrectly may destroy your ADC!
  //                                                                ADS1015  ADS1115
  //                                                                -------  -------
  // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  // ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  // ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
  // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
  // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
  // ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV
