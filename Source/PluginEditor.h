/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Components/HorizontalMeter.h"

//==============================================================================
/**
*/
class MeterAudioProcessorEditor  : public juce::AudioProcessorEditor,
    private juce::Timer
{
public:
    MeterAudioProcessorEditor (MeterAudioProcessor&);
    ~MeterAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MeterAudioProcessor& audioProcessor;
    Gui::HorizontalMeter horizontalMeterL, horizontalMeterR;
    juce::ToggleButton modeButton;


    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MeterAudioProcessorEditor)
};
