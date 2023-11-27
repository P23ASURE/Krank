/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <math.h>



//==============================================================================
AnalogFattenerAudioProcessor::AnalogFattenerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", AudioChannelSet::stereo(), true)
#endif
    ),
    parameters(*this, nullptr, "Parameters",
        {
            std::make_unique<juce::AudioParameterFloat>("crack", "Crack", 0.0f, 1.0f, 0.5f),
            std::make_unique<juce::AudioParameterFloat>("color", "Color", 0.0f, 1.0f, 0.5f),
            std::make_unique<juce::AudioParameterFloat>("boost", "Boost", 0.0f, 1.0f, 0.5f),
            std::make_unique<juce::AudioParameterFloat>("limit", "Limit", 0.0f, 1.0f, 0.5f)
        })
#endif
{
}

AnalogFattenerAudioProcessor::~AnalogFattenerAudioProcessor()
{
}


float AnalogFattenerAudioProcessor::calcolaRMS(AudioBuffer<float>& buffer) {
    auto numSamples = buffer.getNumSamples();
    auto numChannels = buffer.getNumChannels();

    float sum = 0.0f;

    for (int channel = 0; channel < numChannels; ++channel) {
        auto* channelData = buffer.getReadPointer(channel);

        for (int sample = 0; sample < numSamples; ++sample) {
            sum += channelData[sample] * channelData[sample];
        }
    }

    float mean = sum / (numSamples * numChannels);
    return sqrt(mean);
}

float AnalogFattenerAudioProcessor::smoothRMS(float currentRms, float previousSmoothedRms) {
    float smoothingFactor = 0.05f; // Questo può essere adattato in base alle esigenze
    return previousSmoothedRms + smoothingFactor * (currentRms - previousSmoothedRms);
}


//==============================================================================
const String AnalogFattenerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AnalogFattenerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AnalogFattenerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AnalogFattenerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AnalogFattenerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AnalogFattenerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AnalogFattenerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AnalogFattenerAudioProcessor::setCurrentProgram (int index)
{
}

const String AnalogFattenerAudioProcessor::getProgramName (int index)
{
    return {};
}

void AnalogFattenerAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void AnalogFattenerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void AnalogFattenerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AnalogFattenerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif


template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

void AnalogFattenerAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        const auto inputData = buffer.getReadPointer (channel);
        auto* outputData = buffer.getWritePointer(channel);
        
        // ..do something to the data...
        for (auto sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            float CrankedSignal = outputData[sample] * crankValue;
            float ColoredSignal = fmin(fmax(sin(fmax(fmin(CrankedSignal, 1.0), -1.0) * const1) / const2, -1.0), 1.0);
            float SummedSignal = CrankedSignal + ColoredSignal;
            float dBL = amp_dB * log(abs(SummedSignal)) + boostValue;

            if (dBL > threshold_dB)
            {
                over_dB = dBL - threshold_dB;
                over_dB = a * over_dB + b * over_dB * over_dB;
                dBL = fmin(threshold_dB + over_dB, limitValue);
            }

            outputData[sample] = exp(dBL / amp_dB) * sgn(outputData[sample]);
        }

    }

    float rms = calcolaRMS(buffer); // Calcola RMS qui
    rmsValue = rms;
    smoothedRmsValue = smoothRMS(rmsValue, smoothedRmsValue); // Applica smoothing

}


//==============================================================================
bool AnalogFattenerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* AnalogFattenerAudioProcessor::createEditor()
{
    return new AnalogFattenerAudioProcessorEditor (*this);
}

//==============================================================================
void AnalogFattenerAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // Creare un nuovo oggetto Xml con lo stato del valore albero corrente
    auto state = parameters.copyState();
    std::unique_ptr<XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void AnalogFattenerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // Creare un oggetto Xml dal blocco binario
    std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState != nullptr)
    {
        // Se il blocco binario ha un Xml valido, utilizzalo per impostare lo stato
        if (xmlState->hasTagName(parameters.state.getType()))
        {
            parameters.replaceState(ValueTree::fromXml(*xmlState));
        }
    }
}

float AnalogFattenerAudioProcessor::getSmoothedRmsValue() const {
    return smoothedRmsValue;
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AnalogFattenerAudioProcessor();
}
