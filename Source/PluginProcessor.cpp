/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MeterAudioProcessor::MeterAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

MeterAudioProcessor::~MeterAudioProcessor()
{
}

//==============================================================================
const juce::String MeterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MeterAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MeterAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MeterAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MeterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MeterAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MeterAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MeterAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MeterAudioProcessor::getProgramName (int index)
{
    return {};
}

void MeterAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MeterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    leftLevel.reset(sampleRate, 0.5);
    rightLevel.reset(sampleRate, 0.5);
}

void MeterAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MeterAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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




void MeterAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    leftLevel.skip(buffer.getNumSamples());
    rightLevel.skip(buffer.getNumSamples());


    maxValue = std::max(maxValue, buffer.getMagnitude(0, buffer.getNumSamples()));

    maxValueDB = juce::Decibels::gainToDecibels(maxValue);
    


    if (isRMS == true)
    {
        const auto leftDB = juce::Decibels::gainToDecibels(buffer.getRMSLevel(0, 0, buffer.getNumSamples()));
        const auto rightDB = juce::Decibels::gainToDecibels(buffer.getRMSLevel(1, 0, buffer.getNumSamples()));
        if (leftDB < leftLevel.getCurrentValue())
        {
            leftLevel.setTargetValue(leftDB);
        }
        else
        {
            leftLevel.setCurrentAndTargetValue(leftDB);
        }

        if (rightDB < rightLevel.getCurrentValue())
        {
            rightLevel.setTargetValue(rightDB);
        }
        else
        {
            rightLevel.setCurrentAndTargetValue(rightDB);
        }
    }
    else
    {
        const auto leftDB = juce::Decibels::gainToDecibels(buffer.getMagnitude(0,0, buffer.getNumSamples()));
        const auto rightDB = juce::Decibels::gainToDecibels(buffer.getMagnitude(1,0, buffer.getNumSamples()));


        if (leftDB < leftLevel.getCurrentValue())
        {
            leftLevel.setTargetValue(leftDB);
        }
        else
        {
            leftLevel.setCurrentAndTargetValue(leftDB);
        }

        if (rightDB < rightLevel.getCurrentValue())
        {
            rightLevel.setTargetValue(rightDB);
        }
        else
        {
            rightLevel.setCurrentAndTargetValue(rightDB);
        }
    }


    

}

//==============================================================================
bool MeterAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MeterAudioProcessor::createEditor()
{
    return new MeterAudioProcessorEditor (*this);
}

//==============================================================================
void MeterAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void MeterAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

double MeterAudioProcessor::getLeftLevel()
{
    return leftLevel.getNextValue();
}

double MeterAudioProcessor::getRightLevel()
{
    return rightLevel.getNextValue();
}

void MeterAudioProcessor::getCurrentMode(bool state)
{
    isRMS = state;
}

float MeterAudioProcessor::getMaxValue()
{
    return maxValueDB;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MeterAudioProcessor();
}
