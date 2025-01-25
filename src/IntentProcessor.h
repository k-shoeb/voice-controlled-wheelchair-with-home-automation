#ifndef _intent_processor_h_
#define _intent_processor_h_

#include <map>
#include "WitAiChunkedUploader.h"

enum IntentResult
{
    FAILED,
    SUCCESS,
};

class IntentProcessor
{
private:
    std::map<std::string, int> m_device_to_pin;
    //IntentResult processIntent(const Intent &intent);
    IntentResult turnOnDevice(const Intent &intent);
    IntentResult ultraSonicSensor();
    IntentResult forward();
    IntentResult backward();
    IntentResult right();
    IntentResult left();
    IntentResult stop();

public:
    void addDevice(const std::string &name, int gpio_pin);
    IntentResult processIntent(const Intent &intent);
};

#endif
