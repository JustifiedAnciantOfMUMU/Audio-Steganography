#pragma once
#include <JuceHeader.h>
#include "AudioBufferPlayer.h"


/** GUI_AudioBufferPlayer - the GUI Componant for AudioBufferPlayer.
@see AudioBufferPlayer
@see AudioBufferPlayerListener*/
class GUI_AudioBufferPlayer : public Component,
    public Button::Listener,
    AudioBufferPlayerListener
{
public:
    /**Constructor - initilises all Components.
    @param audioPlayer - Reference to juce::AudioBufferPlayer*/
    GUI_AudioBufferPlayer(AudioBufferPlayer& audioPlayer);

    /**Destructor.*/
    ~GUI_AudioBufferPlayer();

    /** resized - inherited function from juce::Component that is called window is resized.
    @see juce::Button::Component*/
    void resized() override;

    /** buttonClicked - inherited function from juce::Button::Listener that is called when a button is clicked.
    @param button - pointer to the juce::Button pressed
    @see juce::Button::Listener*/
    void buttonClicked(Button* button) override;

    /** getNextAudioBlock - gets next block of sampels.
    @param bufferToFill - reference to const juce::AudioSourceChannelInfo instance*/
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill);

    /** setup - Calls setup fuction in juce::AudioTransportSource and enables Gui buttons.
    @param afReader - reference to a juce::AudioFormatReader& instance
    @see AudioBufferPlayer::setup*/
    void setup(AudioFormatReader& afReader);

private:

    TextButton playButton;
    TextButton stopButton;
    TextButton loopingButton;
    TextButton saveFileButton;

    AudioBufferPlayer& m_audioPlayer;
    bool m_looping;

    /** stateChanged - inherited fuction from AudioBufferPlayerListener. called when 
    @param newState - current TransportState of the juce::AudioTransportSource
    @see AudioBufferPlayerListener::stateChanged*/
    virtual void stateChanged(TransportState newState) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GUI_AudioBufferPlayer)
};