#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
 * The "brain" of the plugin. This handles all audio processing.
 * It receives stereo audio, runs it through a low-pass IIR filter,
 * and outputs the filtered result.
 */
class LowPassFilterAudioProcessor : public juce::AudioProcessor
{
public:
    LowPassFilterAudioProcessor();
    ~LowPassFilterAudioProcessor() override;

    //==========================================================================
    // Audio lifecycle — called by the host (Logic Pro)
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==========================================================================
    // Editor (the UI window)
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==========================================================================
    // Plugin metadata
    const juce::String getName() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==========================================================================
    // Program (preset) handling — we only have one preset (default)
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==========================================================================
    // State saving/loading (so Logic can remember your knob positions)
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==========================================================================
    // The parameter tree — holds the cutoff knob value
    juce::AudioProcessorValueTreeState apvts;

private:
    // ProcessorDuplicator applies the same IIR filter to every channel (L + R)
    juce::dsp::ProcessorDuplicator<
        juce::dsp::IIR::Filter<float>,
        juce::dsp::IIR::Coefficients<float>> lowPassFilter;

    void updateFilter();
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LowPassFilterAudioProcessor)
};
