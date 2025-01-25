// WiFi credentials
#define WIFI_SSID "SSID"      //SSID
#define WIFI_PSWD "XXXXXXXX"   //Password

// I2S Microphone Settings
// Which channel is the I2S microphone on? I2S_CHANNEL_FMT_ONLY_LEFT or I2S_CHANNEL_FMT_ONLY_RIGHT

#define I2S_MIC_CHANNEL           I2S_CHANNEL_FMT_ONLY_LEFT
//#define I2S_MIC_CHANNEL         I2S_CHANNEL_FMT_ONLY_RIGHT

#define I2S_MIC_SERIAL_CLOCK      GPIO_NUM_4      //SCK
#define I2S_MIC_SERIAL_DATA       GPIO_NUM_15    //SD
#define I2S_MIC_LEFT_RIGHT_CLOCK  GPIO_NUM_16   //L/R Clock WS

// Define LED Back Indicator Pins
#define Red_Indicator_R           GPIO_NUM_12     // Right Indicator RED
#define Red_Indicator_L           GPIO_NUM_13    // Left Indicator RED
#define Green_Indicator           GPIO_NUM_14   // Green Indicator 

// Define motor control pins for Right Motor 
#define R_Motor_Clk               GPIO_NUM_21     //  Right Motor ClockWise
#define R_Motor_A_Clk             GPIO_NUM_19    //  Right Motor Anti ClockWise
// Define motor control pins for Left Motor 
#define L_Motor_Clk               GPIO_NUM_17     //  Left Motor ClockWise
#define L_Motor_A_Clk             GPIO_NUM_18    //  Left Motor Anti ClockWise

#define buzzer                    GPIO_NUM_17    // Buzzer 

// Define Ultrasonic Pins
#define echoPin                   GPIO_NUM_35    //ultrasonic input
#define triggerPin                GPIO_NUM_32   // Ultrasonic output

//intent_processor->addDevice("door", GPIO_NUM_26);
//intent_processor->addDevice("fan", GPIO_NUM_27);
//intent_processor->addDevice("light", GPIO_NUM_25);
//intent_processor->addDevice("kitchen light", GPIO_NUM_33);

//intent_processor->addDevice("extra", GPIO_NUM_22);
//intent_processor->addDevice("extra", GPIO_NUM_23);

// command recognition settings
#define COMMAND_RECOGNITION_ACCESS_KEY "3TQVVUWJKMNFQGZ3DEZU62DN52UK3NZM"  // You can use this Key or create your own from https://www.wit.ai/