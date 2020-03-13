/*
 * Functions for the sensor ESP 
 * most taken from examples / stanzas
 * 
 * 
 */



void MPU6050calibration() {
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }
  Serial.println("MPU6050 calibrated");
  delay(1000);  
}


float readSPImax6675(int cspin) {
  uint16_t temp_reading;
  float celsius;
  //use it as you would the regular arduino SPI API
  vspi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
  digitalWrite(cspin, LOW); //pull SS slow to prep other end for transfer
  // Read and Write is done simultaneously, we write 0xff because we just want to read
  // Exact format and bit shiftery is 12 bits from a MAX6675, copied from arduino class
  temp_reading = vspi->transfer(0xff);
  temp_reading <<= 8;
  temp_reading |= vspi->transfer(0xff);
  if (temp_reading & 0x4) {
    // uh oh, no thermocouple attached!
    return NAN; 
    //return -100;
  }
  temp_reading >>= 3;

  celsius = temp_reading * 0.25;

  digitalWrite(cspin, HIGH); //pull ss high to signify end of data transfer
  vspi->endTransaction();
  return celsius;
}


// taken from adafruit mcp3008, use hw SPI
// retval is 0-1023 (depending on Vref)
int readSPImcp3008(uint8_t channel) {
  byte command;
  command = ((0x01 << 7) |             // start bit
             (0x00 << 6) |          // single read
             ((channel & 0x07) << 3)); // channel number

  byte b0, b1, b2;

  vspi->beginTransaction(SPISettings(MCP3008_SPI_MAX, MCP3008_SPI_ORDER, MCP3008_SPI_MODE));
  digitalWrite(2, LOW);

  b0 = vspi->transfer(command);
  b1 = vspi->transfer(0x00);
  b2 = vspi->transfer(0x00);

  digitalWrite(2, HIGH);
  vspi->endTransaction();

//  Serial.println("Finished transaction - b0: " + String(b0) + " b1: " + String(b1) + " b2: " + String(b2));

  return 0x3FF & ((b0 & 0x01) << 9 | (b1 & 0xFF) << 1 | (b2 & 0x80) >> 7);
}

char* wl_status_to_string(wl_status_t status) {
  switch (status) {
    case WL_NO_SHIELD: return "WL_NO_SHIELD";
    case WL_IDLE_STATUS: return "WL_IDLE_STATUS";
    case WL_NO_SSID_AVAIL: return "WL_NO_SSID_AVAIL";
    case WL_SCAN_COMPLETED: return "WL_SCAN_COMPLETED";
    case WL_CONNECTED: return "WL_CONNECTED";
    case WL_CONNECT_FAILED: return "WL_CONNECT_FAILED";
    case WL_CONNECTION_LOST: return "WL_CONNECTION_LOST";
    case WL_DISCONNECTED: return "WL_DISCONNECTED";
  }
}

void printAllInfo(Measurement *Meas)
{
    time_t now;
    Serial.println("\n-------------------------");
    Serial.println(String(localTimeString(time(&now))));
    Serial.println("-------------------------\n");
    Serial.println("W1 TempC    : " + String(Meas->w1_tempC) + " C");
    Serial.println("SPI1 TempC  : " + String(Meas->max6675_1_tempC) + " C");
    Serial.println("SPI2 TempC  : " + String(Meas->max6675_2_tempC) + " C");
    Serial.println("Chan0 Input : " + String(Meas->analog0));
    Serial.println("Chan0 Volt  : " + String(Meas->afrvoltage0));
    Serial.println("RSSI        : " + String(Meas->rssi) + " dBm");
    Serial.println("Wifi Status : " + String(wl_status_to_string(Meas->wlanstatus)));
    Serial.println("Free Heap   : " + String(ESP.getFreeHeap()));
    Serial.println("Timestamp   : " + String(Meas->timestamp));
    Serial.println("RPM Freq    : " + String(Meas->rpmfreq) + " Hz");
    Serial.println("REV Freq    : " + String(Meas->revfreq) + " Hz");
    Serial.println("Last Write  : " + String(Meas->lastwritetime) + " ms");
//    Serial.println("rpmcounter  : " + String(rpmpulses));
//    Serial.println("revcounter  : " + String(revolutionpulses));
    Serial.println("-------------------------\n\n\n");
}

char* localTimeString(time_t timestamp)
{
  struct tm * timeinfo;
  timeinfo = gmtime(&timestamp);
  strftime(timestring, sizeof(timestring), "%Y-%m-%d %H:%M:%S", timeinfo);
  return timestring;
}

void printOnDisplay(Measurement *Meas) {
  struct tm timeinfo;
  unsigned long startmeasMillis=millis();
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain Time");
    return;
  }
  strftime(timestring, sizeof(timestring), "%Y-%m-%d %H:%M:%S", &timeinfo);
#ifdef SSD1306
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, timestring);
  display.drawString(0, 10,"Heap " + String(ESP.getFreeHeap()));
  // strength, formula stolen
  unsigned int strength = 120 - ((5/3) * abs(Meas->rssi));
  // draw the percentage as String
  display.drawProgressBar(0, 22, 120, 10, strength);
//  display.setTextAlignment(TEXT_ALIGN_CENTER);
//  display.drawString(64,42, "RSSI: " + String(Meas->rssi));
//  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 34,"x:" + String(Meas->mpu6050_gyro_x) + " y:" + String(Meas->mpu6050_gyro_y) + " z:" + String(Meas->mpu6050_gyro_z));
  display.drawString(0, 44,String(Meas->w1_tempC) + "C " + String(Meas->spi_tempC) + "C " + String(Meas->mpu6050_temperature) + "C");
  display.drawString(0, 54,String(Meas->bme280_temperature) + "C " + String(Meas->bme280_humidity) + " rel%");
  display.display();
#endif
#ifdef HAVE_ST7735
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(0, 0, 1);
  tft.print(timestring);
  tft.println("                           ");
  tft.print("Heap " + String(ESP.getFreeHeap()) + "  RSSI " + String(Meas->rssi));
  tft.println("                           ");
  tft.print("RPM:" + String(Meas->rpmfreq) + "Hz, AFR:"+ String(Meas->afrvoltage0) + "V");
  tft.println("                           ");
  // strength, formula stolen
  // unsigned int strength = 120 - ((5/3) * abs(Meas->rssi));
  // draw the percentage as String
  // display.drawProgressBar(0, 22, 120, 10, strength);
  // tft.println("                           ");
  tft.print("EGT1 " + String(Meas->max6675_1_tempC) + "C EGT2 " + String(Meas->max6675_2_tempC) + "C");
  tft.println("                           ");
  tft.print("MPU  " + String(Meas->mpu6050_temperature) + "C BME  " + String(Meas->bme280_temperature) +"C");
  tft.println("                           ");
  tft.print(String(Meas->bme280_humidity) + " rel%  " + String(Meas->bme280_airpressure) + " hPa");
  tft.println("                           ");
  tft.println("                           ");
  tft.print("RPM:");
  tft.setTextSize(2);
  tft.print("  " + String(Meas->rpm));
  tft.println("              ");
  tft.setTextSize(1);
// We should catch HEAT < 0.5 V and ERR > 4 V
  tft.print("\nAFR:");
  tft.setTextSize(2);
  if (Meas->afrvoltage0 < 0.5) {
    tft.setTextColor(TFT_RED, TFT_WHITE);
    tft.print("  HEAT");    
  }
  else if (Meas->afrvoltage0 > 4.0) {
    tft.setTextColor(TFT_RED, TFT_WHITE);
    tft.print("  ERR");
  }
  else {
    tft.print("  " + String(Meas->calculatedafr));
  }
  tft.println("              ");
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(1);
  tft.setCursor(0, 10*10, 1);
//  tft.print("x:" + String(Meas->mpu6050_gyro_x) + " y:" + String(Meas->mpu6050_gyro_y) + " z:" + String(Meas->mpu6050_gyro_z) + "");
//  tft.println("                           ");
  tft.setTextColor(TFT_WHITE, TFT_RED);
  tft.print("Sensors time: " + String(Meas->sensorreadtime) + " ms");
  tft.println("                           ");
  tft.setTextColor(TFT_WHITE, TFT_BLUE);
  tft.print("Network time: " + String(Meas->lastwritetime) + " ms");
  tft.println("                           ");
  tft.setTextColor(TFT_WHITE, TFT_DARKGREEN);
  tft.print("Display time: " + String(millis() - startmeasMillis) + " ms");
  tft.println("                           ");
#endif
}

void displayWhileWaitingForTime () {
#ifdef SSD1306
  display.clear();
  display.drawString(0, 10,String(WIFI_SSID) + ": " + String(WiFi.RSSI()) + "dBm");
  display.drawString(0, 20,"Heap : " + String(ESP.getFreeHeap()));
  display.drawString(0, 30,"Time Sync attempt " + String(timeiteration) + ".");
  display.drawString(0, 40,"Epoch : " + String(setuptime));
  display.display();
#endif
}


void printStringOnDisplay(char* inputstring) {
  struct tm timeinfo;
  getLocalTime(&timeinfo);
  strftime(timestring, sizeof(timestring), "%Y-%m-%d %H:%M:%S", &timeinfo);
#ifdef HAVE_SSD1306
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, timestring);
  display.drawString(0, 20, inputstring);
  display.display();
#endif
#ifdef HAVE_ST7735
  // without wrap max strlen is "0 1 2 3 4 5 6 7 8 9 0 1 2 " = 26 chars with font 1
  // to avoid refreshing the whole screen we pad the string to that (otherwise a long
  // string will remain on the screen
  // cursor position is updated by the library
  int i;
  tft.setCursor(0, 0, 1);
  tft.print(timestring);
  tft.println("                           ");
  tft.print(inputstring);
  tft.println("                           ");
#endif
  Serial.println(timestring);
  Serial.println(inputstring);
}
