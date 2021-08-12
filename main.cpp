#include "miosix.h"
#include "drivers/common/audio.h"
#include "drivers/stm32f407vg_discovery/encoder.h"
#include "drivers/stm32f407vg_discovery/button.h"
#include "audio/audio_processor.h"
#include "drivers/stm32f407vg_discovery/adc_reader.h"
#include "Synth/Synth.h"
#include <cstdint>
#include <util/lcd44780.h>
#include <thread>



typedef miosix::Gpio<GPIOB_BASE,12> d4;
typedef miosix::Gpio<GPIOB_BASE,13> d5;
typedef miosix::Gpio<GPIOB_BASE,14> d6;
typedef miosix::Gpio<GPIOB_BASE,15> d7;
typedef miosix::Gpio<GPIOC_BASE,1> rs;
typedef miosix::Gpio<GPIOC_BASE,2> e;


typedef Encoder<TIM1_BASE, GPIOE_BASE,  9, 11> encoder1;
typedef Encoder<TIM3_BASE, GPIOB_BASE,  4,  5> encoder2;
typedef Encoder<TIM4_BASE, GPIOD_BASE, 12, 13> encoder3;
typedef Encoder<TIM5_BASE, GPIOA_BASE,  0,  1> encoder4;

typedef Button<GPIOD_BASE, 1> button1;

// LCD Initialization
static miosix::Lcd44780 display(rs::getPin(), e::getPin(), d4::getPin(),
                                d5::getPin(), d6::getPin(), d7::getPin(), 2, 16);


//Audio Driver and Synthesizer Objects
static AudioDriver audioDriver;
static Synth synth(audioDriver);


void hardwareUIThreadFunction() {
    float frequency;
    float fm;
    bool gate;

    // Variable Update
    while (true) {
        {
            auto values = AdcReader::readAll();
            miosix::FastMutex mutex;
            gate = button1::risingEdge();
            frequency = values[0] + values[1] + values[2] + values[3];
            fm = encoder2::getValue();


            synth.setFrequency(frequency);
            synth.setFMFreq(fm);
            if(gate)
                synth.gate();

            display.clear();
            display.go(0, 0);
            display.printf("Gate: %0d", gate);

            display.go(0, 1);
            display.printf("F1: %d | F2: %d", (int)frequency,(int) fm);
        }
        miosix::Thread::sleep(500);
    }
}

int main() {
    AdcReader::init();

    button1::init();
    encoder1::init();
    encoder2::init();

    display.clear();
    display.go(0, 0);
    display.printf("Miosix Synth 01");

    // Audio Driver and Synth initialization
    audioDriver.init();
    audioDriver.setAudioProcessable(synth);

    std::thread hardwareInterfaceThread(hardwareUIThreadFunction);

    audioDriver.start();
}
