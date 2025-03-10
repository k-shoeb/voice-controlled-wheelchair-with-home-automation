/* 
  Voice Control Wheelchair with Home Automation Intigration
*/
 
#include <Arduino.h>
#include <WiFi.h>
#include <driver/i2s.h>
#include <esp_task_wdt.h>
#include "I2SMicSampler.h"
#include "config.h"
#include "Application.h"
#include "IntentProcessor.h"
#include "IndicatorLight.h"

// i2s config for reading from left channel of I2S
i2s_config_t i2sMemsConfigBothChannels = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = 16000,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_MIC_CHANNEL,
    .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_I2S),
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4,
    .dma_buf_len = 64,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0};

// i2s microphone pins
i2s_pin_config_t i2s_mic_pins = {
    .bck_io_num = I2S_MIC_SERIAL_CLOCK,
    .ws_io_num = I2S_MIC_LEFT_RIGHT_CLOCK,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = I2S_MIC_SERIAL_DATA};


// This task does all the heavy lifting for our application
void applicationTask(void *param)
{
  Application *application = static_cast<Application *>(param);
  const TickType_t xMaxBlockTime = pdMS_TO_TICKS(100);
  while (true)
  {
    // wait for some audio samples to arrive
    uint32_t ulNotificationValue = ulTaskNotifyTake(pdTRUE, xMaxBlockTime);
    if (ulNotificationValue > 0)
    {
      application->run();
    }
  }
}

void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.println("Starting up");

  WiFi.mode(WIFI_STA);                   // start up wifi
  WiFi.begin(WIFI_SSID, WIFI_PSWD);     // launch WiFi
  if (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  Serial.printf("Total heap: %d\n", ESP.getHeapSize());
  Serial.printf("Free heap: %d\n", ESP.getFreeHeap());

  // make sure we don't get killed for our long running tasks
  esp_task_wdt_init(10, false);

  // start up the I2S input from an I2S microphone    // Direct i2s input from INMP441
  I2SSampler *i2s_sampler = new I2SMicSampler(i2s_mic_pins, false);

  // indicator light to show when we are listening
  IndicatorLight *indicator_light = new IndicatorLight();

  // and the intent processor
  IntentProcessor *intent_processor = new IntentProcessor();
  
  intent_processor->addDevice("fan", GPIO_NUM_27);
  intent_processor->addDevice("door", GPIO_NUM_26);
  intent_processor->addDevice("light", GPIO_NUM_25);
  intent_processor->addDevice("kitchen light", GPIO_NUM_33);

  // create our application
  Application *application = new Application(i2s_sampler, intent_processor,indicator_light);

  // set up the i2s sample writer task
  TaskHandle_t applicationTaskHandle;
  xTaskCreate(applicationTask, "Application Task", 8192, application, 1, &applicationTaskHandle);

  // start sampling from i2s device 
  i2s_sampler->start(I2S_NUM_0, i2sMemsConfigBothChannels, applicationTaskHandle);
 
  // Set motor control pins as outputs
  pinMode(R_Motor_Clk, OUTPUT);
  pinMode(R_Motor_A_Clk, OUTPUT);
  pinMode(L_Motor_Clk, OUTPUT);
  pinMode(L_Motor_A_Clk, OUTPUT);

  // Set Indicator pins
  pinMode(Red_Indicator_R, OUTPUT);
  pinMode(Red_Indicator_L, OUTPUT);
  pinMode(Green_Indicator, OUTPUT);

  // buzzer pin
  pinMode(buzzer, OUTPUT);

  // setup for ultrasonic sensor
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop()
{
  vTaskDelay(1000);
}
