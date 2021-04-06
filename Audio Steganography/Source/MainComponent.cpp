#include "MainComponent.h"
#include "Cypher_ReverseString.h"

//==============================================================================
MainComponent::MainComponent(AudioBufferPlayer& audioPlayerBuffer, Codec& encoder) : cypherGui(encoder), playerGui(audioPlayerBuffer)
{
    setSize (800, 600);

    //button setup
    addAndMakeVisible(&openButton);
    openButton.setButtonText("Open...");
    openButton.onClick = [this] { openButtonClicked(); };

    addAndMakeVisible(&cypherGui);
    addAndMakeVisible(&playerGui);

    m_formatManager.registerBasicFormats();
    setAudioChannels(0, 2);
 }

MainComponent::~MainComponent()
{
    shutdownAudio();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setFont (juce::Font (16.0f));
    g.setColour (juce::Colours::white);
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    const int SPACE = 5;
    int width = getWidth();
    int height = getHeight();
    
    int openButtonHeight = height / 16;
    int playerGuiHeight = height / 8;
    int encoderGuiHeight = height - (openButtonHeight + playerGuiHeight);

    openButton.setBounds(5, 3, width - 10, openButtonHeight - 3);
    cypherGui.setBounds(5, openButton.getHeight() + 6, width - 10, encoderGuiHeight);
    playerGui.setBounds(5, (openButton.getHeight() + cypherGui.getHeight()) + 11, width - 10, playerGuiHeight - 10);
}

void MainComponent::openButtonClicked() 
{
    juce::FileChooser chooser("Select a Wav file to play...", {}, "*.wav");                                        
    
    if (chooser.browseForFileToOpen())                                         
    {
        File file = chooser.getResult();
        openButton.setButtonText("File:" + file.getFileName());
        loadFile(file);
    }
}

void MainComponent::loadFile(juce::File& file)
{
    //putting file into buffer
    AudioFormatReader* afReader = m_formatManager.createReaderFor(file); //pointer to the audio format reader
    if (afReader != nullptr)
    {
        playerGui.setup(*afReader);
        delete(afReader);
    }
    else
    {
        //if error displays popup message
        AlertWindow::showMessageBox(juce::AlertWindow::AlertIconType::WarningIcon, "Warning: File Not Loaded Correctly", "Unexpected error please open .wav", "OK", this->getParentComponent());
    }
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
}

void MainComponent::releaseResources()
{
}

void MainComponent::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
    playerGui.getNextAudioBlock(bufferToFill);
}




