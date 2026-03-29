#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
// Parameter layout: defines the single "Cutoff Frequency" knob.
// Range: 20 Hz to 20 kHz, default 1 kHz.
// skewFactor 0.25 makes the knob feel logarithmic (more precision at low freqs).
juce::AudioProcessorValueTreeState::ParameterLayout
LowPassFilterAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { "cutoff", 1 },
        "Cutoff Frequency",
        juce::NormalisableRange<float> (20.0f, 20000.0f, 0.1f, 0.25f),
        1000.0f));

    return layout;
}

//==============================================================================
LowPassFilterAudioProcessor::LowPassFilterAudioProcessor()
    : AudioProcessor (BusesProperties()
                          .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                          .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      apvts (*this, nullptr, "Parameters", createParameterLayout())
{
}

LowPassFilterAudioProcessor::~LowPassFilterAudioProcessor() {}

//==============================================================================
// Called once when Logic loads the plugin or changes the sample rate/buffer size.
// We set up the filter here so it's ready to process audio.
void LowPassFilterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate       = sampleRate;
    spec.maximumBlockSize = (juce::uint32) samplesPerBlock;
    spec.numChannels      = (juce::uint32) getTotalNumOutputChannels();

    lowPassFilter.prepare (spec);
    lowPassFilter.reset();
    updateFilter();
}

void LowPassFilterAudioProcessor::releaseResources()
{
    // Nothing to clean up
}

//==============================================================================
// This is called for every buffer of audio (many times per second).
// We update the filter coefficients, then let it process the audio.
void LowPassFilterAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                                juce::MidiBuffer& /*midiMessages*/)
{
    juce::ScopedNoDenormals noDenormals; // prevents CPU spikes from tiny float values

    updateFilter();

    juce::dsp::AudioBlock<float>            block (buffer);
    juce::dsp::ProcessContextReplacing<float> context (block);
    lowPassFilter.process (context);
}

// Reads the current knob value and recalculates the filter coefficients.
void LowPassFilterAudioProcessor::updateFilter()
{
    float  cutoff     = apvts.getRawParameterValue ("cutoff")->load();
    double sampleRate = getSampleRate();

    if (sampleRate > 0.0 && cutoff > 0.0f)
    {
        *lowPassFilter.state =
            *juce::dsp::IIR::Coefficients<float>::makeLowPass (sampleRate, (double) cutoff);
    }
}

//==============================================================================
bool LowPassFilterAudioProcessor::hasEditor() const { return true; }

juce::AudioProcessorEditor* LowPassFilterAudioProcessor::createEditor()
{
    return new LowPassFilterAudioProcessorEditor (*this);
}

//==============================================================================
const juce::String LowPassFilterAudioProcessor::getName() const { return JucePlugin_Name; }
bool LowPassFilterAudioProcessor::acceptsMidi() const  { return false; }
bool LowPassFilterAudioProcessor::producesMidi() const { return false; }
bool LowPassFilterAudioProcessor::isMidiEffect() const { return false; }
double LowPassFilterAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int  LowPassFilterAudioProcessor::getNumPrograms()                              { return 1; }
int  LowPassFilterAudioProcessor::getCurrentProgram()                           { return 0; }
void LowPassFilterAudioProcessor::setCurrentProgram (int)                       {}
const juce::String LowPassFilterAudioProcessor::getProgramName (int)            { return {}; }
void LowPassFilterAudioProcessor::changeProgramName (int, const juce::String&)  {}

//==============================================================================
// Save the current knob positions so Logic can restore them when you reopen a project.
void LowPassFilterAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void LowPassFilterAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState != nullptr && xmlState->hasTagName (apvts.state.getType()))
        apvts.replaceState (juce::ValueTree::fromXml (*xmlState));
}

//==============================================================================
// This is the entry point JUCE uses to create your plugin.
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new LowPassFilterAudioProcessor();
}
