# Voice controlled wheelchair with Home Automation Integration

We are using Platform.io to build this project, it is available in VS code as an extension.

To understand the code the best place to start is `src/Application.cpp`. This is a very simple state machine that switches between either waiting for a wake word or trying to interpret the user's command.

From there you can look at `src/state_machine/DetectWakeWordState.cpp' and `src/state_machine/RecogniseCommandState.cpp`.

## Config options

Set up your WiFi in the `config.h` file.

There are a number of options in this file that you can modify to suit your own setup.

For I2S Microphone, you need to tell the system which channel you have configured the microphone on (left or right - generally these devices default to left).

```
// Which channel is the I2S microphone on? I2S_CHANNEL_FMT_ONLY_LEFT or I2S_CHANNEL_FMT_ONLY_RIGHT
#define I2S_MIC_CHANNEL I2S_CHANNEL_FMT_ONLY_LEFT
// #define I2S_MIC_CHANNEL I2S_CHANNEL_FMT_ONLY_RIGHT
```

define the pins you have connected to the microphone:
```
#define I2S_MIC_SERIAL_CLOCK GPIO_NUM_4
#define I2S_MIC_SERIAL_DATA GPIO_NUM_15
#define I2S_MIC_LEFT_RIGHT_CLOCK GPIO_NUM_16
```

Select the pins for indicator lights and buzzer
```
// Define LED Back Indicator Pins
#define Red_Indicator_R           GPIO_NUM_12     // Right Indicator RED
#define Red_Indicator_L           GPIO_NUM_13    // Left Indicator RED
#define Green_Indicator           GPIO_NUM_14   // Green Indicator

#define buzzer                    GPIO_NUM_17   // Buzzer
```

Set up the pins for the motor driver to control motors.
```
// Define motor control pins for Right Motor
#define R_Motor_Clk               GPIO_NUM_21     //  Right Motor ClockWise
#define R_Motor_A_Clk             GPIO_NUM_19    //  Right Motor Anti ClockWise
// Define motor control pins for Left Motor
#define L_Motor_Clk               GPIO_NUM_17     //  Left Motor ClockWise
#define L_Motor_A_Clk             GPIO_NUM_18    //  Left Motor Anti ClockWise
```

Define the pins for Ultrasonic sensor
```
// Define Ultrasonic Pins
#define echoPin                   GPIO_NUM_35    //ultrasonic input
#define triggerPin                GPIO_NUM_32   // Ultrasonic output
```

Define the devices you want to control in `main.cpp` at `line 82`, I have added four you can modify your own but you have to create your own `wit.ai KEY`.
```
intent_processor->addDevice("fan", GPIO_NUM_27);
intent_processor->addDevice("door", GPIO_NUM_26);
intent_processor->addDevice("light", GPIO_NUM_25);
intent_processor->addDevice("kitchen light", GPIO_NUM_33);
```

You can add two more devices with these extra pins.
```
//intent_processor->addDevice("extra", GPIO_NUM_22);
//intent_processor->addDevice("extra", GPIO_NUM_23);
```

Finally, we have the access key for wit.ai - I will leave my key active for as long as possible. You can create your own key at [wit.ai](https://wit.ai/) or use the default.
```
// command recognition settings,
#define COMMAND_RECOGNITION_ACCESS_KEY "3TQVVUWJKMNFQGZ3DEZU62DN52UK3NZM"
```

If you're having problems detecting the wake word `Marvin` then you can try lowering the detection threshold:

`/src/state_machine/DetectWakeWordState.cpp` line 59.

```
if (output > 0.95)
```

Change 0.95 to something lower and you will increase the sensitivity. It's also worth logging this value out if you are having problems so you can see how well the detector is picking up the work.

OR

You can create your own wake word detection model by referring [diy-alexa](https://github.com/atomic14/diy-alexa/)


To understand the implementation process refer to this [Video](https://youtu.be/re-dSV_a0tM?si=jpFHV01kjxktbOmR) by `atomic14`.


## Diagram




## Acknowledgements
Special thanks to [atomic14](https://github.com/atomic14/) for the original repository, [diy-alexa](https://github.com/atomic14/diy-alexa/), which helped make this project possible.
