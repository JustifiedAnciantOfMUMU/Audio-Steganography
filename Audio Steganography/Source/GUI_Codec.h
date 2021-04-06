#pragma once
#include <JuceHeader.h>
#include "Codec.h"


/**GUI_Codec - gui for Codec class. inherits from juce::Component, juce::Button::Listener, juce::ComboBoc::Listener, juce::TextEditor::Listener
@see juce::Component @see juce::Button::Listener @see juce::ComboBoc::Listener @see juce::TextEditor::Listener*/
class GUI_Codec : public Component,
				  public Button::Listener,
                  public ComboBox::Listener,
                  public TextEditor::Listener
{
public:
    /**Constructor - initialises components and member variables
    @param codec - reference to instance of Codec class*/
    GUI_Codec(Codec& codec);

    /**Destructor - stops thread*/
    ~GUI_Codec();

    /**resized - inherited function from juce::Componant. called when window size chaned */
    void resized() override;

    /**saveCypherKey - saves cypher key buy saving properties of juce::ValueTree in .key file.*/
    void saveCypherKey();

    /**importCypherKey - loads existing values from .key cypher key file into juce::ValueTree*/
    void importCypherKey();


private:
    Codec& m_codec;

    ValueTree valueTree;
    static const Identifier CypherKey;
    static const Identifier CypherType;

    StringArray cypherTypes;
    ComboBox cypherSelector;
    TextButton importKeyButton;
    TextButton exportKeyButton;
    TextButton encodeButton;
    TextButton decodeButton;
    TextEditor textSource;

    /**buttonClicked - inherited function from juce::Button::Listner. called when button is pressed
    @param button - pointer to instance of Button class
    @see juce::Button::Listner */
    void buttonClicked(Button* button) override;

    /**comboBoxChanged - inherited function from juce::ComboBox::Listner. called when juce::ComboBox has changed
    @param comboBoxThatHasChanged - pointer to instance of ComboBox class
    @see juce::ComboBox::Listner */
    virtual void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;

    /**textEditorTextChanged - inherited function from juce::TextEditor::Listner. called when text in juce::TextEditor text has changed
    @param textEditor - reference to instance of TextEditor
    @see juce::TextEditor::Listner */
    virtual void textEditorTextChanged(TextEditor& textEditor) override;

    /**textEditorEscapeKeyPressed - inherited function from juce::TextEditor::Listner. called when ESC key pressed when juce::TextEditor is in use
    @param textEditor - reference to instance of TextEditor
    @see juce::TextEditor::Listner */
    virtual void textEditorEscapeKeyPressed(TextEditor& textEditor) override;

    /**errorHandler - looks at Codec::Error and shows apropriate warning popup box
    @Param error - Enum Codec::Error of possible error*/
    void errorHandler(Codec::Error error);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GUI_Codec)
};

