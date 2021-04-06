#include "AudioBufferPlayer.h"


AudioBufferPlayer::AudioBufferPlayer(LockingAudioSampleBuffer& audioBuffer) : m_state(TransportState::Stopped), m_sampleRate(0), m_audioBuffer(audioBuffer), m_readerSource(nullptr), m_bitsPerSample(0)
{
	m_listener = nullptr;
	m_transportSource = std::make_unique<juce::AudioTransportSource>();
	m_transportSource->addChangeListener(this);
	m_transportSource->setLooping(false);
}

AudioBufferPlayer::~AudioBufferPlayer()
{
	m_transportSource->releaseResources();
	if (m_readerSource != nullptr)
		m_readerSource->releaseResources();
}

//reads selected file into buffer then sets as source to audio transport player
void AudioBufferPlayer::setup(AudioFormatReader& afReader, bool looping)
{
	m_transportSource->stop(); //stops the audio transport source resets read position to 0
	m_transportSource->setSource(nullptr);

	if (m_readerSource != nullptr) 
	{
		m_readerSource->releaseResources();
	}

	//locks audioBuffer
	DBG("AudioBufferPlayer::setup");
	m_audioBuffer.Lock();

	//formats and fills audio buffer
	m_audioBuffer.clear();
	DBG(afReader.lengthInSamples);
	m_audioBuffer.setSize(afReader.numChannels, afReader.lengthInSamples, false, true, false);
	afReader.read(&m_audioBuffer, 0, afReader.lengthInSamples, 0, true, true);
	//setsup positionable audio source for m_transportSource
	m_readerSource = std::make_unique<juce::MemoryAudioSource>(m_audioBuffer, false, false);
	m_transportSource->setSource(m_readerSource.get(), 0, nullptr, m_sampleRate);
	m_transportSource->prepareToPlay(afReader.lengthInSamples, m_sampleRate);
	setLooping(looping);

	//extracts information for saving buffer into .wav file
	m_sampleRate = afReader.sampleRate;
	m_channelLayout = afReader.getChannelLayout();
	m_bitsPerSample = afReader.bitsPerSample;

	m_audioBuffer.Unlock();
}

void AudioBufferPlayer::changeState(TransportState newState)
{
	if (m_state != newState)
	{
		m_state = newState;

		switch (m_state)
		{
		case TransportState::Stopped:
			//resets reading poisition
			m_transportSource->setPosition(0.0);
			m_readerSource->setNextReadPosition(0);
			break;

		case TransportState::Starting:
			m_transportSource->start();
			break;

		case TransportState::Pausing:
			m_transportSource->stop();
			break;


		case TransportState::Stopping:
			m_transportSource->stop();
			break;
		}

		if (nullptr != m_listener)
			m_listener->stateChanged(newState);
	}
}

void AudioBufferPlayer::changeListenerCallback(ChangeBroadcaster* source)
{
	if (source == m_transportSource.get()) //changes stransport state dependant on state of transport source
	{
		if (m_transportSource->isPlaying())
			changeState(TransportState::Playing);
		else if ((m_state == TransportState::Stopping) || (m_state == TransportState::Playing))
			changeState(TransportState::Stopped);
		else if (m_state == TransportState::Pausing)
			changeState(TransportState::Paused);
	}
}

void AudioBufferPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
	if (m_readerSource.get() == nullptr)
	{
		bufferToFill.clearActiveBufferRegion();
		return;
	}

	if (m_state == TransportState::Playing)
	{
		int read = m_transportSource->getNextReadPosition(); //getsnext read position the next block of samples from that point
		m_transportSource->getNextAudioBlock(bufferToFill);

		//Stops transport source looping as issue found with using AudioTransportSource to read from MemoryAudioSource 
		if (m_transportSource->getNextReadPosition() < read && !m_transportSource->isLooping()) //if the next read position is less that last meaning the reader has looped
		{
			DBG(juce::String::formatted("NextReadPos: %d", m_transportSource->getNextReadPosition()));
			m_transportSource->stop(); //stop playing
		}
	}
}

TransportState AudioBufferPlayer::currentState() const
{
	return m_state;
}

void AudioBufferPlayer::setListener(AudioBufferPlayerListener& listener)
{
	m_listener = &listener;
}

//saving audio buffer as .wav file
void AudioBufferPlayer::exportBuffer()
{
	//browsing for file location
	FileChooser fileBrowser("Save Audio As...", {}, "*.wav");

	//creates metadata
	StringPairArray newMetaData;
	newMetaData.clear();
	newMetaData.minimiseStorageOverheads();

	if (fileBrowser.browseForFileToSave(true)) //browers opened with file override existing
	{
		WavAudioFormat waf;
		File saveFile = fileBrowser.getResult();
		FileOutputStream* fos = new FileOutputStream(saveFile);

		//overwrites file if file exists
		if (saveFile.existsAsFile())
		{
			fos->setPosition(0);
			fos->truncate();
		}

		//writing File
		AudioFormatWriter* afw = waf.createWriterFor(fos, m_sampleRate, 2, 32, newMetaData, 0); // 2 channels, 32 bits per sample in order to make compatable with audio sample buffer stops loosing data
		afw->writeFromAudioSampleBuffer(m_audioBuffer, 0, m_audioBuffer.getNumSamples());
		afw->flush();
		delete(afw);
	}
}

//sets weather positionable reader in MemoryAudioSource is looping
void AudioBufferPlayer::setLooping(bool looping)
{
	if (nullptr != m_readerSource)
	{
		m_readerSource->setLooping(looping);
	}
	
}
