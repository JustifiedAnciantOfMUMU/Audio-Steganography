#pragma once
#include <JuceHeader.h>
#include "Codec.h"
#include "GUI_AudioBufferPlayer.h"
#include "GUI_Codec.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent
{
public:
    //==============================================================================
    /** constructor - initialises Gui components, AudioBufferPlayer and  Codec.
    @param audioPlayerBuffer - reference to AudioBufferPlayer
    @param codec - reference to Codec
    @see juce::AudioAppComponent*/
    MainComponent(AudioBufferPlayer& audioPlayerBuffer, Codec& codec);

    /**Destructor - calls AudioAppComponent function shutdownAudio().
    @see juce::AudioAppCompononent::shutdownAudio*/
    ~MainComponent() override;
    //==============================================================================
    void paint (juce::Graphics&) override;

    /** resized - called when Window is resized. inherited from AudioAppComponent
    @see juce::AudioAppComponent*/
    void resized() override;

    /** Function Run when Open Button Clicked - opens juce::FileChooser to select file then calls MainComponent::loadFile(juce::File)
    @see MainComponent::loadFile*/
    void openButtonClicked();

    /** loads referenced juce::File into an AudioBuffer<float> and calls for AudioBufferPlayer instance to be setup.
    @param file - The juce::File to be copied into AudioBuffer<float> */
    void loadFile(juce::File& file);

    /** prepareToPlay - inherited from juce::AudioAppComponent
    @param samplesPerBlockExpected - integer value of samples per block expected during playback
    @param sampleRate - double playback sample rate
    @see juce::AudioAppComponent*/
    virtual void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

    /**releaseResources - inherited from juce::AudioAppComponent
    @see juce::AudioAppComponent*/
    virtual void releaseResources() override;

    /** prepareToPlay - inherited from juce::AudioAppComponent
    @param bufferToFill - const refernce to juce::AudioSourceChannelInfo instance
    @see juce::AudioAppComponent*/
    virtual void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;

private:
    juce::AudioFormatManager m_formatManager;

    //GUi
    GUI_AudioBufferPlayer playerGui;
    GUI_Codec cypherGui;
    TextButton openButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)

};
