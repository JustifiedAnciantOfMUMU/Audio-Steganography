#pragma once
#include <JuceHeader.h>
#include "lockingAudioSampleBuffer.h"

/** TransportState - Enum of possible States of the AudioBufferPlayer.*/
enum class TransportState
{
    Stopped,
    Starting,
    Playing,
    Pausing,
    Paused,
    Stopping
};

class AudioBufferPlayerListener
{
public:
    /** stateChanged - abstract function.
    @param newState - State of an AudioTransportSource stored in Enum TransportSata*/
    virtual void stateChanged(TransportState newState) =0;
};

class AudioBufferPlayer: public juce::ChangeListener
{
public:
    /**Constructor - Inisialises member variables.
    @param audioBuffer - reference to LockingAudioSampleBuffer*/
    AudioBufferPlayer(LockingAudioSampleBuffer& audioBuffer);

    /**Destructor. - realeases reasources of class AudioTransportSource and MemoryAudioSource*/
    ~AudioBufferPlayer();

    /** setup - copys sampels from juce::File into juce::AudioSampleBuffer then set up juce::AudioTransportSource to play from the buffer.
    @param afReader - reference to the reader of the audio file
    @param looping - true if juce::MemoryAudioSource should loop*/
	void setup(AudioFormatReader& afReader, bool looping);

    /** changeState - calls functions in juce::AudioTransportSource depending on TransportState.
    @param newState - state of audioBufferPlayer stored in Enum*/
    void changeState(TransportState newState);

    /** changeListenerCallback - inherited fuction from juce::ChangeListener to receive the callback from change listener.
    @param source - pointer to juce::ChangeBroadcaster
    @see juce::ChangeListener*/
    void changeListenerCallback(ChangeBroadcaster* source) override;

    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);

    void setListener(AudioBufferPlayerListener& listener);

    /** exportBuffer - Saves current juce::AudioSampleBuffer as .wav file at user specified location on disk.*/
    void exportBuffer();

    /** setLooping - sets if juce::MemoryAudioSource depending on inputted bool variable.
    @param looping - bool, true if the juce::MemoryAudioSource should loop*/
    void setLooping(bool looping);

    /** currentState - returns current TransportState.
    @return TransportState - current state of the juce::AudioTransportSource stored as Enum TransportStae*/
    TransportState currentState() const;
private:

    TransportState m_state;
	std::unique_ptr<juce::MemoryAudioSource> m_readerSource;
    std::unique_ptr<juce::AudioTransportSource> m_transportSource;
    AudioBufferPlayerListener* m_listener;
    double m_sampleRate;
    AudioChannelSet m_channelLayout;
    unsigned int m_bitsPerSample;
    LockingAudioSampleBuffer& m_audioBuffer;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioBufferPlayer)
};

