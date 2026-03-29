#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
 * The UI of the plugin — a dark panel with a single rotary knob for cutoff frequency.
 */
class LowPassFilterAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit LowPassFilterAudioProcessorEditor (LowPassFilterAudioProcessor&);
    ~LowPassFilterAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    LowPassFilterAudioProcessor& audioProcessor;

    juce::Slider cutoffKnob;
    juce::Label  cutoffLabel;

    // The attachment keeps the knob in sync with the parameter automatically
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> cutoffAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LowPassFilterAudioProcessorEditor)
};
