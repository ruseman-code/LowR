#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
LowPassFilterAudioProcessorEditor::LowPassFilterAudioProcessorEditor (LowPassFilterAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Configure the rotary knob
    cutoffKnob.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    cutoffKnob.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 90, 20);
    cutoffKnob.setTextValueSuffix (" Hz");
    addAndMakeVisible (cutoffKnob);

    // Label above the knob
    cutoffLabel.setText ("Cutoff", juce::dontSendNotification);
    cutoffLabel.setFont (juce::Font (15.0f, juce::Font::bold));
    cutoffLabel.setJustificationType (juce::Justification::centred);
    cutoffLabel.setColour (juce::Label::textColourId, juce::Colours::lightblue);
    addAndMakeVisible (cutoffLabel);

    // Attachment wires the knob to the "cutoff" parameter in the processor
    cutoffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        audioProcessor.apvts, "cutoff", cutoffKnob);

    setSize (220, 220);
}

LowPassFilterAudioProcessorEditor::~LowPassFilterAudioProcessorEditor() {}

//==============================================================================
void LowPassFilterAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Dark navy background
    g.fillAll (juce::Colour (0xff1a1a2e));

    // Title text
    g.setColour (juce::Colours::white);
    g.setFont (juce::Font (18.0f, juce::Font::bold));
    g.drawFittedText ("Low Pass Filter",
                      getLocalBounds().removeFromTop (36),
                      juce::Justification::centred, 1);
}

void LowPassFilterAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced (16);
    area.removeFromTop (36);                     // leave room for the title
    cutoffLabel.setBounds (area.removeFromTop (24));
    cutoffKnob.setBounds  (area);
}
