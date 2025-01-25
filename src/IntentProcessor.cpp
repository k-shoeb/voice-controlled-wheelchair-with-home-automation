#include <Arduino.h>
#include "IntentProcessor.h"
#include "config.h"

int distance;
long duration;

IntentResult IntentProcessor::turnOnDevice(const Intent &intent)
{
    if (intent.intent_confidence < 0.5)
    {
        Serial.printf("Only %.f%% certain on intent\n", 100 * intent.intent_confidence);
        return FAILED;
    }

    if (intent.device_name.empty())
    {
        Serial.println("No device found");
        return FAILED;
    }

    if (intent.device_confidence < 0.5)
    {
        Serial.printf("Only %.f%% certain on device\n", 100 * intent.device_confidence);
        return FAILED;
    }

    if (intent.trait_value.empty())
    {
        Serial.println("Can't work out the intent action");
        return FAILED;
    }


    if (intent.trait_confidence < 0.5)
    {
        Serial.printf("Only %.f%% certain on trait\n", 100 * intent.trait_confidence);
        return FAILED;
    }


    int is_turn_on = intent.trait_value == "on";


    // global device name "lights"
    if (intent.device_name == "lights")
    {
        for (const auto &dev_pin : m_device_to_pin)
        {
            digitalWrite(dev_pin.second, is_turn_on);
        }
    }
    else
    {
        // see if the device name is something we know about
        if (m_device_to_pin.find(intent.device_name) == m_device_to_pin.end())
        {
            Serial.printf("Don't recognise the device '%s'\n", intent.device_name.c_str());
            return FAILED;
        }
        digitalWrite(m_device_to_pin[intent.device_name], is_turn_on);
        Serial.printf("Device '%s' is Switched\n", intent.device_name.c_str());
    }
    // success!
    return SUCCESS;
}


IntentResult IntentProcessor::ultraSonicSensor()
{
   // Set the trigger to LOW
   digitalWrite(triggerPin, LOW);
   delayMicroseconds (2);
   // Set the trigger to HIGH
   digitalWrite(triggerPin, HIGH);
   delayMicroseconds (10);
   digitalWrite(triggerPin, LOW);
   // Read ECHO - sound wave travel time in µs
   duration = pulseIn(echoPin, HIGH);
   delay(10);

   if (duration != 0) {

      // Calculate the distance
      Serial.println("Calculating object distance");
      distance = duration * 0.0343/2;                         // cm per micro second
      Serial.printf("Object Distance is '%d'\n", distance);
        if (distance < 40){
            Serial.println("Detected some obstacle. Now Stoping.....");
            return stop();
        }
        else {
            Serial.println("Following your command");
            return FAILED;
        }
   }
   else {
        Serial.print("Ultraasonic Sensor not Connected");
        return FAILED;
   }
 return FAILED;
 }

IntentResult IntentProcessor::forward()
{
    Serial.println("Moving Forward.....");
    // Run both motors in forward direction
    digitalWrite(R_Motor_Clk, HIGH);    // Motor R
    digitalWrite(R_Motor_A_Clk, LOW);   // Motor R
    digitalWrite(L_Motor_Clk, HIGH);   // Motor L                    
    digitalWrite(L_Motor_A_Clk, LOW);  // Motor L               

    digitalWrite(Red_Indicator_R, LOW);
    digitalWrite(Red_Indicator_L, LOW);
    digitalWrite(Green_Indicator, HIGH);
    return SUCCESS;
}

IntentResult IntentProcessor::backward()
{
    Serial.println("Moving Backward.....");
    // Run both motors in Backward direction
    digitalWrite(R_Motor_Clk, LOW);                          
    digitalWrite(R_Motor_A_Clk, HIGH);  
    digitalWrite(L_Motor_Clk, LOW);       
    digitalWrite(L_Motor_A_Clk, HIGH);                     

    digitalWrite(Red_Indicator_R, HIGH);
    digitalWrite(Red_Indicator_L, HIGH);
    digitalWrite(Green_Indicator, LOW);
    return SUCCESS;
}


IntentResult IntentProcessor::right()
{
    Serial.println("Turning Right.....");
    // Run both motors in Opposite direction for 3 seconds
    digitalWrite(R_Motor_Clk, LOW);                       
    digitalWrite(R_Motor_A_Clk, HIGH); // Motor R
    digitalWrite(L_Motor_Clk, HIGH);                    
    digitalWrite(L_Motor_A_Clk, LOW); // Motor L                        

    digitalWrite(Red_Indicator_R, HIGH);
    digitalWrite(Red_Indicator_L, LOW);
    digitalWrite(Green_Indicator, LOW);
    delay(5000);  
    return stop();

    return SUCCESS;
}


IntentResult IntentProcessor::left()
{
    Serial.println("Turning Left....");
    // Run both motors in Opposite direction
    digitalWrite(R_Motor_Clk, HIGH);                       
    digitalWrite(R_Motor_A_Clk, LOW); // Motor R
    digitalWrite(L_Motor_Clk, LOW);
    digitalWrite(L_Motor_A_Clk, HIGH); // Motor L                        

    digitalWrite(Red_Indicator_R, LOW);
    digitalWrite(Red_Indicator_L, HIGH);
    digitalWrite(Green_Indicator, LOW);
    delay(5000);  
    return stop();

    return SUCCESS;
}

IntentResult IntentProcessor::stop()
{
    Serial.println("Stop.... :) ");
    // Stop both motors
    digitalWrite(R_Motor_Clk, LOW);
    digitalWrite(R_Motor_A_Clk, LOW); // Motor R
    digitalWrite(L_Motor_Clk, LOW);
    digitalWrite(L_Motor_A_Clk, LOW); // Motor L

    digitalWrite(Red_Indicator_R, HIGH);
    digitalWrite(Red_Indicator_L, HIGH);
    digitalWrite(Green_Indicator, HIGH);
    return SUCCESS;
}


IntentResult IntentProcessor::processIntent(const Intent &intent)
{   
    // Define a string variable to store the intent text
    std::string command;

    if (intent.text.empty())
    {
        Serial.println("No text recognised");
        return FAILED;
    }
    Serial.printf("I heard \"%s\"\n", intent.text.c_str());

    // Store intent text into the command variable
    command = intent.text;
    // Convert all characters in command to lower
    std::transform(command.begin(), command.end(), command.begin(), ::tolower); 
    

    if (intent.intent_name.empty())
    {
        Serial.println("Can't work out what you want to do with the device...");
        return FAILED;
    }
    Serial.printf("Intent is %s\n", intent.intent_name.c_str());


    if (intent.intent_name == "turn_on")
    {   
        Serial.printf("Command is %s\n", command.c_str());

        return ultraSonicSensor();    // Check for object

         if(command.find("forward") != std::string::npos)
        {
            // Run both motors in Backward direction
            return forward();
        }

        else  if(command.find("backward") != std::string::npos)
        {
            // Run both motors in Backward direction
            return backward();
        }

        else  if(command.find("left") != std::string::npos)
        {
            // Run both motors in Opposite direction
            return left();
        }

        else  if(command.find("right") != std::string::npos)
        {
            // Run both motors in Opposite direction
            return right();
        }

        else if(command.find("stop") != std::string::npos)
        {
            // Stop both motors
            return stop();
        }

    }

    if (intent.intent_name == "turn_on_and_off")
    {
        return turnOnDevice(intent);
    }
    
    return FAILED;
}

void IntentProcessor::addDevice(const std::string &name, int gpio_pin)
{
    m_device_to_pin.insert(std::make_pair(name, gpio_pin));
    pinMode(gpio_pin, OUTPUT);

}