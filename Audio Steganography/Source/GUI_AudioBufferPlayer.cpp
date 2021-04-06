#include "GUI_AudioBufferPlayer.h"

GUI_AudioBufferPlayer::GUI_AudioBufferPlayer(AudioBufferPlayer& audioPlayer) : m_audioPlayer(audioPlayer)
{
	addAndMakeVisible(&playButton);
	playButton.setButtonText("Play");
	playButton.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
	playButton.addListener(this);
	playButton.setEnabled(false);

	addAndMakeVisible(&stopButton);
	stopButton.setButtonText("Stop");
	stopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
	stopButton.addListener(this);
	stopButton.setEnabled(false);

	addAndMakeVisible(&loopingButton);
	loopingButton.setButtonText("Looping: off");
	loopingButton.setColour(juce::TextButton::buttonColourId, juce::Colours::orange);
	m_looping = false;
	loopingButton.addListener(this);

	addAndMakeVisible(&saveFileButton);
	saveFileButton.setButtonText("Save As .wav");
	saveFileButton.addListener(this);
	saveFileButton.setEnabled(false);

	m_audioPlayer.setListener(*this);
}

GUI_AudioBufferPlayer::~GUI_AudioBufferPlayer()
{
}

void GUI_AudioBufferPlayer::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
	m_audioPlayer.getNextAudioBlock(bufferToFill);
}

void GUI_AudioBufferPlayer::resized()
{
	int height = getHeight();
	int width = getWidth();
	int spacing = width / 7;

	playButton.setBounds(0, 0, (spacing * 3) - 3, (height / 2) - 3);
	stopButton.setBounds((spacing * 3), 0, (spacing * 3) - 3, (height / 2) - 3);
	loopingButton.setBounds(spacing * 6, 0, spacing, (height / 2) - 3);
	saveFileButton.setBounds(0, height / 2, width, height / 2);
}

void GUI_AudioBufferPlayer::buttonClicked(Button* button)
{
	if (button == &playButton)
	{
		m_audioPlayer.setLooping(m_looping);

		if ((m_audioPlayer.currentState() == TransportState::Stopped) || (m_audioPlayer.currentState() == TransportState::Paused))
			m_audioPlayer.changeState(TransportState::Starting);
		else if (m_audioPlayer.currentState() == TransportState::Playing)
			m_audioPlayer.changeState(TransportState::Pausing);
	}
	if (button == &stopButton)
	{
		if (m_audioPlayer.currentState() == TransportState::Paused)
			m_audioPlayer.changeState(TransportState::Stopped);
		else
			m_audioPlayer.changeState(TransportState::Stopping);
	}
	if (button == &loopingButton)
	{
		//toggles looping value then sets looping
		if (m_looping)
		{
			m_looping = false;
			loopingButton.setColour(juce::TextButton::buttonColourId, juce::Colours::orange);
			loopingButton.setButtonText("Looping: off");
		}
		else
		{
			m_looping = true;
			loopingButton.setColour(juce::TextButton::buttonColourId, juce::Colours::orangered);
			loopingButton.setButtonText("Looping: on");
		}

		m_audioPlayer.setLooping(m_looping);
	}
	if (button == &saveFileButton)
	{
		m_audioPlayer.exportBuffer();
	}
}

void GUI_AudioBufferPlayer::stateChanged(TransportState newState)
{
	switch (newState)
	{
	case TransportState::Stopped:
		playButton.setButtonText("Play");
		playButton.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
		stopButton.setButtonText("Stop");
		stopButton.setEnabled(false);
		break;

	case TransportState::Playing:
		playButton.setButtonText("Pause");
		playButton.setColour(juce::TextButton::buttonColourId, juce::Colours::greenyellow);
		stopButton.setButtonText("Stop");
		stopButton.setEnabled(true);
		break;

	case TransportState::Paused:
		playButton.setButtonText("Resume");
		stopButton.setButtonText("Stop");
		break;
	}
}

void GUI_AudioBufferPlayer::setup(AudioFormatReader& afReader)
{
	m_audioPlayer.setup(afReader, m_looping);
	playButton.setEnabled(true);
	saveFileButton.setEnabled(true);
}


