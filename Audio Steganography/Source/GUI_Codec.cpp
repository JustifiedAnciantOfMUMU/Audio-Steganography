#include "GUI_Codec.h"
#include "Cypher_None.h"
#include "Cypher_ReverseString.h"
#include "Cypher_LetterSub.h"

//initialises static member variables
const Identifier GUI_Codec::CypherKey = "CypherKey";
const Identifier GUI_Codec::CypherType = "CypherType";

GUI_Codec::GUI_Codec(Codec& encoder) : m_codec(encoder), cypherTypes({ "Cypher: None","Cypher: Reverse String","Cypher: Letter Substitution" }), valueTree(CypherKey)
{
    //Initialising GUi Components
    addAndMakeVisible(&textSource);
    textSource.setTextToShowWhenEmpty("Type Message to Encode here", juce::Colours::black);
    textSource.setColour(juce::TextEditor::ColourIds::backgroundColourId, juce::Colours::white); 
    textSource.setColour(juce::TextEditor::ColourIds::textColourId, juce::Colours::black);
    textSource.setMultiLine(true, true);
    textSource.addListener(this);

    addAndMakeVisible(&cypherSelector);
    cypherSelector.addItemList(cypherTypes, 1);
    cypherSelector.setSelectedItemIndex(0);
    cypherSelector.addListener(this);
    m_codec.setCypher(new Cypher_None());

    addAndMakeVisible(&importKeyButton);
    importKeyButton.setButtonText("Import Cypher Key");
    importKeyButton.addListener(this);

    addAndMakeVisible(&exportKeyButton);
    exportKeyButton.setButtonText("Export Cypher Key");
    exportKeyButton.addListener(this);

    addAndMakeVisible(&encodeButton);
    encodeButton.setButtonText("Encode File");
    encodeButton.addListener(this);
    encodeButton.setEnabled(true);

    addAndMakeVisible(&decodeButton);
    decodeButton.setButtonText("Decode File");
    decodeButton.addListener(this);
    decodeButton.setEnabled(true);

    //linking value tree to combo box
    valueTree.setProperty(CypherType, cypherSelector.getSelectedIdAsValue(), nullptr);
    Value value = valueTree.getPropertyAsValue(CypherType, nullptr);
    cypherSelector.getSelectedIdAsValue().referTo(value);
}

GUI_Codec::~GUI_Codec()
{
    m_codec.stopThread(200);
}

void GUI_Codec::resized()
{
    int height = getHeight();
    int width = getWidth();

    //setting Components Position on GUI
    cypherSelector.setBounds(0, ((height / 8) * 6) + 3, (width / 3) * 2, (height / 8) - 8);
    importKeyButton.setBounds(((width / 3) * 2) + 3, ((height / 8) * 6) + 3, ((width / 3) / 2) - 3, (height / 8) - 8);
    exportKeyButton.setBounds(((width / 3) * 2.5) + 3, ((height / 8) * 6) + 3, ((width / 3) / 2) - 3, (height / 8) - 8);
    encodeButton.setBounds(0, (height / 8) * 7, (width / 2) - 2, height / 8);
    decodeButton.setBounds((width / 2) + 2, (height / 8) * 7, (width / 2) - 2, height / 8);
    textSource.setBounds(0, 0, width, (height / 8) * 6);
}

void GUI_Codec::saveCypherKey()
{
    //gets file
    FileChooser fileBrowser("Save Audio As...", {}, "*.key");
    if (fileBrowser.browseForFileToSave(true))
    {
        File saveFile = fileBrowser.getResult();
        FileOutputStream fos(saveFile);

        //overriding file
        if (saveFile.existsAsFile())
        {
            fos.setPosition(0); //setting write position to 0 and truncates the file to that point
            fos.truncate();
        }

        if (!fos.failedToOpen())
        {
            //if link is openable write to stream
            valueTree.writeToStream(fos);
            fos.flush();
        }
        else
        {
            //if link is !openable push error code
            m_codec.m_error = Codec::Error::KeyExportFailed;
            errorHandler(m_codec.m_error);
        }
    }
}

void GUI_Codec::importCypherKey()
{
    //browse for file
    FileChooser fileBrowser("Load Cypher Key...", {}, "*.key");
    if (fileBrowser.browseForFileToOpen()) 
    {
        File file = fileBrowser.getResult();
        
        //if file exists
        if (file.existsAsFile())
        {
            ValueTree newvalueTree = ValueTree::readFromStream(FileInputStream(file));
            valueTree.copyPropertiesFrom(newvalueTree, nullptr);
        }
        else
        {
            //error occured
            m_codec.m_error = Codec::Error::KeyImportFailed;
            errorHandler(m_codec.m_error);
        }
    }
    
}

void GUI_Codec::buttonClicked(Button* button)
{
    if (button == &encodeButton) 
    {
        Codec::Error error = m_codec.encodeMessage(textSource.getText().toStdString()); //calls for string to be encoded returns error

        if (error == Codec::Error::None) //if no error clears textsource
        {
            textSource.setText("");
        }
        else
        {
            errorHandler(m_codec.m_error); //if error pass error to errorhandler
        }
    }
    if (button == &decodeButton)
    {
        textSource.setText(juce::String(m_codec.decodeMessage()));
    }
    if (button == &importKeyButton) 
    {
        importCypherKey();
    }
    if (button == &exportKeyButton) 
    {
        saveCypherKey();
    }
}

void GUI_Codec::comboBoxChanged(ComboBox* comboBox)
{
    if(comboBox == &cypherSelector)
    {
        switch (cypherSelector.getSelectedId()) //which Cypher to use when encoding
        {
        case 1:
            m_codec.setCypher(new Cypher_None());
            break;
        case 2:
            m_codec.setCypher(new Cypher_ReverseString());
            break;
        case 3:
            m_codec.setCypher(new Cypher_LetterSub());
            break;
        }

        valueTree.setProperty(CypherType, cypherSelector.getSelectedIdAsValue(), nullptr); //sets ValueTree Property
    }
}

void GUI_Codec::textEditorTextChanged(TextEditor& textEditor) 
{
    std::string currentString = textEditor.getText().toStdString();
    int viableSamples = m_codec.getnumViableSamples();

    if (currentString.length() > viableSamples) //if string is larger that number of encodable samples pushes error
    {
        m_codec.m_error = Codec::Error::LargeString;
        errorHandler(m_codec.m_error);
    }
}

void GUI_Codec::textEditorEscapeKeyPressed(TextEditor& textEditor)
{
    textEditor.setText(""); //removes all text
}

void GUI_Codec::errorHandler(Codec::Error error)
{
    switch (error)
    {
    case Codec::Error::LargeString:
        AlertWindow::showMessageBox(juce::AlertWindow::AlertIconType::WarningIcon, "Warning: Too many Charactors", "You have too much data to be encoded into this .wav file. Either open a file or open a longer audio file", "OK", this->getParentComponent());
        break;
    case Codec::Error::Unknown:
        AlertWindow::showMessageBox(juce::AlertWindow::AlertIconType::WarningIcon, "Warning: Unknown Error", "unknown error stopping encode", "OK", this->getParentComponent());
        break;
    case Codec::Error::NoString:
        AlertWindow::showMessageBox(juce::AlertWindow::AlertIconType::WarningIcon, "Warning: No Text In Text Editor", "There is no data in text editor to encode", "OK", this->getParentComponent());
        break;
    case Codec::Error::NoAudio:
        AlertWindow::showMessageBox(juce::AlertWindow::AlertIconType::WarningIcon, "Warning: No Audio To Encode Into", "Please open .wav file there are no viable samples to ecode into ", "OK", this->getParentComponent());
        break;
    case Codec::Error::KeyExportFailed:
        AlertWindow::showMessageBox(juce::AlertWindow::AlertIconType::WarningIcon, "Warning: Cypher Key Export Failure", "Cypher Key Export Failure", "OK", this->getParentComponent());
        break;
    case Codec::Error::KeyImportFailed:
        AlertWindow::showMessageBox(juce::AlertWindow::AlertIconType::WarningIcon, "Warning: Cypher Key Import Failure", "please select existing file rather than directory", "OK", this->getParentComponent());
        break;
    }
}
