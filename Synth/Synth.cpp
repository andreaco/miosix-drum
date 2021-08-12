#include "Synth.h"
Synth::Synth(AudioDriver &audioDriver) :
AudioProcessor(audioDriver)
{
    oscillator.setSampleRate(getSampleRate());
    oscillator.setFrequency(440);
    oscillator.setMuted(false);

    envelope.setAttackTime(0.01, getSampleRate());
    envelope.setReleaseTime(1.5, getSampleRate());
}


void Synth::process()
{
    auto &buffer = getBuffer();
    float *left = buffer.getWritePointer(0);
    float *right = buffer.getWritePointer(1);

    for (uint32_t i = 0; i < getBufferSize(); i++)
    {
        float env = envelope.nextSample();
        float sample = env * oscillator.nextSample();
        left[i]  = sample;
        right[i] = sample;
    }

}