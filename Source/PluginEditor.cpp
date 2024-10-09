/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
MeterAudioProcessorEditor::MeterAudioProcessorEditor (MeterAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{

    addAndMakeVisible(horizontalMeterL);
    addAndMakeVisible(horizontalMeterR);
    addAndMakeVisible(modeButton);


    modeButton.setButtonText("RMS");
    modeButton.onClick = [this]()
        {
            audioProcessor.getCurrentMode(modeButton.getToggleState());
        };

    setSize (400, 300);
    startTimerHz(24);

    
}

MeterAudioProcessorEditor::~MeterAudioProcessorEditor()
{
    stopTimer();
}

//==============================================================================
void MeterAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::darkgrey);
    
    DBG(audioProcessor.getMaxValue() << "paintMaxValue");

    g.drawText(
        "Max Value is: " + juce::String(audioProcessor.getMaxValue(), 1) + " dB",
        32, 32, 150, 150,
        juce::Justification::centredLeft
    );
   
    
}

void MeterAudioProcessorEditor::timerCallback()
{
    horizontalMeterL.setLevel(audioProcessor.getLeftLevel());
    horizontalMeterR.setLevel(audioProcessor.getRightLevel());

    horizontalMeterL.repaint();
    horizontalMeterR.repaint();

    repaint();
}

void MeterAudioProcessorEditor::resized()
{

    horizontalMeterL.setBounds(210, 64, 15, 200);
    horizontalMeterR.setBounds(190, 64, 15, 200);
    modeButton.setBounds(225, 32, 220, 15);
}
