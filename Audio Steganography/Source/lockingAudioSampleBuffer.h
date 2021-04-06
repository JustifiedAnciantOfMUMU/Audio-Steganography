#pragma once
#include <JuceHeader.h>

/**LockingAudioSampleBuffer - juce::AudioSampleBuffer with juce::CriticalSection to assist with problematic cross thread acess
@see juce::AudioSampleBuffer*/
class LockingAudioSampleBuffer : public juce::AudioSampleBuffer
{
public:
    /**lock - enters the critical section of the juce::AudioSampleBuffer
    @see juce::CriticalSection*/
    void Lock();

    /**unlock - exits the critical section of the juce::AudioSampleBuffer
    @see juce::CriticalSection*/
    void Unlock();
private:
    CriticalSection m_loadLock;
};