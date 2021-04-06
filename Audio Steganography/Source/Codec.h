#pragma once
#include <JuceHeader.h>
#include "lockingAudioSampleBuffer.h"
#include "Cypher.h"

/**Codec Class - Encodes and Decodes string into an audio buffer. The string is extracted by scanning the audio buffer for any samples where the left channel LSB is 0 and the right channel LSB is not 0,
    converting the LSB from ascii to char then adding as next operator in string. A Union is used to change the LSB of float sample.
@see juce::Thread*/
class Codec : public juce::Thread 
{
public:
    /**State - Emun of possible class states*/
    enum class State
    {
        None,
        Encoding,
        Decoding
    };

    /**Error - Emun of possible class errors*/
    enum class Error 
    {
        None,
        LargeString,
        NoString,
        NoAudio,
        KeyExportFailed,
        KeyImportFailed,
        Unknown
    };

    /**Constructor - initialises member variables and starts thread
    @param audioBuffer - reference to LockingAudioSampleBuffer*/
    Codec(LockingAudioSampleBuffer& audioBuffer);

    /**detructor - class destructor*/
    ~Codec();

    /**Function Inserts a string of chars into the LSB of LockedAudioSampleBuffer samples.
    @param string -std::string the String to be encoded into AudioBuffer
    @return Codec::Error*/
	Error encodeMessage(const std::string& string);

    /**Function extracts a string of chars from the LSB of LockedAudioSampleBuffer samples.
    @return returns std::string*/
	std::string decodeMessage();

    /**setEncryption - sets the Cypher used.
    @param encrypter - pointer to an instance of the Cypher base class*/
    void setCypher(Cypher* encrypter);

    /**getnumViableSamples.
    @return int - number of samples in LockingAudioSampleBuffer that data can be encoded into*/
    int getnumViableSamples();

    /**getState.
    @return State - the State of the Codec Class instance*/
    State getState();

    /**run - inherited from juce::Thread.
    @see juce::Thread*/
    void run() override;

    Error m_error;
private:
    /**run - Reformats the LockingAudioSampleBuffer. to avoid chance additional char's being extracted from audio buffer*/
    void reformatAudioBuffer();

    /**bufferLengthChanged - checks if the LockingAudioBuffer has changed length.
    @return bool - true if the buffer has changed length*/
    bool bufferLengthChanged();

    /**numViableSamples - calculates the number of samples in LockedAudioSampleBuffer that data can be encoded into.*/
    void numViableSamples();

    /**encode - encodes string into data
    @param string - reference to std::string*/
    void encode(const std::string& string);

    /**decode - Extracts encoded string from LockingAudioSampleBuffer and saves it to member vairable.*/
    void decode();

    /*A Union is used to change the LSB of float sample.*/
    union FloatData
    {
        float floatValue;
        struct Parts
        {
            uint8_t one;
            uint8_t two;
            uint8_t three;
            uint8_t four;
        } bytes;
    };

    Cypher* m_cypher;
    LockingAudioSampleBuffer& m_audioBuffer;
    std::string m_stringToEncode;
    std::string m_decryptedString;
    State m_state;
    
    int m_numViableSamples;
    int m_audioBufferLength;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Codec)
};