#include "Codec.h"

Codec::Codec(LockingAudioSampleBuffer& audioBuffer): Thread("Codec",0), m_audioBuffer(audioBuffer), m_cypher(nullptr), m_state(Codec::State::None), m_error(Codec::Error::None),m_decryptedString(""), m_stringToEncode("")
{
    startThread();
}

Codec::~Codec()
{
    if (nullptr != m_cypher)
    {
        delete(m_cypher);
    }
}

//fuction to insert string into audiobuffer
Codec::Error Codec::encodeMessage(const std::string& string)
{
    size_t stringLength = string.length();
    DBG(stringLength);

    // if possible encode else pass relevant error
    if (stringLength != 0 && m_audioBufferLength != 0 && stringLength < m_numViableSamples)
    {
        m_stringToEncode = string;
        notify();
        m_error = Error::None;
    }
    else if(stringLength == 0)
    {
        m_error = Error::NoString;
    }
    else if (m_audioBufferLength == 0)
    {
        m_error = Error::NoAudio;
    }
    else
    {
        m_error = Error::LargeString;
    }

    return m_error;
}

void Codec::run()
{
    while (!threadShouldExit()) // thread is active
    {
        if (!m_stringToEncode.empty()) //if encode button pressed there will be text in member variable
        {
            std::string stringToEncode = m_stringToEncode;  
            m_stringToEncode.clear(); // clears
            m_state = State::Encoding;
            encode(stringToEncode); //calss encode
            m_state = State::None;
        }
        else
        {
            //DBG("Thread - Decode");
            m_audioBuffer.Lock(); //locks audio buffer

            if (bufferLengthChanged())
            {
                numViableSamples();
            }

            if (m_audioBuffer.getNumSamples() > 0)
            {
                m_state = State::Decoding;
                decode();
                m_state = State::None;
            }

            m_audioBuffer.Unlock(); //locks audio buffer
        }

        sleep(100);
    }
}

void Codec::encode(const std::string& string)
{
	DBG("running encoding");
	std::string encryptedString = m_cypher->encryptString(string); //calls for string to be encrypted using selected Cypher
    DBG(encryptedString);
	if (!encryptedString.empty())
	{
		reformatAudioBuffer(); //formatts audiobuffer to change the left channel LSB of any sample where left channel LSB = 0 and right channel is >0
		DBG("buffer Formatted");

		FloatData referenceSample;
		FloatData dataSample;
		int stringIndex = 0;

		for (int i = 0; i < m_audioBuffer.getNumSamples(); i++)
		{
			referenceSample.floatValue = m_audioBuffer.getSample(0, i);
			dataSample.floatValue = m_audioBuffer.getSample(1, i);

            //if both float values != 0 and the are still string operators to be encoded
			if ((referenceSample.floatValue != 0.0f && dataSample.floatValue != 0.0f) && stringIndex < encryptedString.length())
			{
				char asciiCode = encryptedString[stringIndex];//convert next operator into its aski value
				dataSample.bytes.one = 0; //set left channel LSB to 0 and right channel LSB to ascii value
				m_audioBuffer.setSample(0, i, dataSample.floatValue);
				dataSample.bytes.one = asciiCode;
				m_audioBuffer.setSample(1, i, dataSample.floatValue);
				stringIndex++;
			}
		}
		DBG("finished");
	}
	else
	{
		DBG("no Text available to encode");
        reformatAudioBuffer();
	}
}

//Function to Formatt audio buffer in a way that elimates contamination of data when decoding
void Codec::reformatAudioBuffer()
{
    FloatData referenceSample;
    FloatData dataSample;

    for (int i = 0; i < m_audioBuffer.getNumSamples(); i++)
    {
        referenceSample.floatValue = m_audioBuffer.getSample(0, i);
        dataSample.floatValue = m_audioBuffer.getSample(1, i);

        if (referenceSample.floatValue != 0.0f && dataSample.floatValue != 0.0f && referenceSample.bytes.one == 0)
        {
            referenceSample.bytes.one = 1;
            m_audioBuffer.setSample(0, i, referenceSample.floatValue);
        }
    }
}

//fuction to extract sting from audio buffer
std::string Codec::decodeMessage()
{
    return m_decryptedString;
}

void Codec::decode()
{
    //DBG("running decoding");
    FloatData referenceSample;
    FloatData dataSample;
    std::string string;

    for (int i = 0; i < m_audioBuffer.getNumSamples(); i++)
    {
        int8_t askiiCode = 0;
        referenceSample.floatValue = m_audioBuffer.getSample(0, i);
        dataSample.floatValue = m_audioBuffer.getSample(1, i);

        //if there both the left and right channels in audio buffer are not zero and the LSB of the left channel is 0 and the Right channel LSM !0
        if ((referenceSample.floatValue != 0.0f && dataSample.floatValue != 0.0f) && (referenceSample.bytes.one == 0 && dataSample.bytes.one != 0))
        {
            //gets the ascii value stored in the right LSB and converts to Char
            askiiCode = dataSample.bytes.one;
            //DBG(juce::String::formatted("value: %d", dataSample.bytes.one));
            char charactor = askiiCode;
            //DBG(juce::String::formatted("value: %c", charactor));
            string.operator+=(charactor); //adds char to string
        }
    }
    std::string decryptedString = m_cypher->decryptString(string);
    //DBG("Decode Finished");
    m_decryptedString = decryptedString;
}

void Codec::setCypher(Cypher* encrypter)
{
    if (nullptr != m_cypher) 
    {
        delete(m_cypher);
    }
    
    m_cypher = encrypter;
}

bool Codec::bufferLengthChanged() 
{
    bool changed = false;

	if (m_audioBuffer.getNumSamples() != m_audioBufferLength) //if buffer length has changes gets new buffer length
	{
		m_audioBufferLength = m_audioBuffer.getNumSamples();
        changed = true;
	}

	return changed; 
}

void Codec::numViableSamples()
{
    FloatData referenceSample;
    FloatData dataSample;
    int numViableSamples = 0;

    //cycles through every sample in buffer
    for (int i = 0; i < m_audioBuffer.getNumSamples(); i++)
    {
        referenceSample.floatValue = m_audioBuffer.getSample(0, i);
        dataSample.floatValue = m_audioBuffer.getSample(1, i);

        //if sample meets encoding criters
        if (referenceSample.floatValue != 0.0f && dataSample.floatValue != 0.0f)
        {
            numViableSamples++; //number of viable samples increases by one
        }
    }
    //DBG(m_numViableSamples);
    m_numViableSamples = numViableSamples;
    return;
}

//returns Codec State
Codec::State Codec::getState()
{
    return m_state;
}

//returns number of viable Samples
int Codec::getnumViableSamples() {

    return m_numViableSamples;
}
