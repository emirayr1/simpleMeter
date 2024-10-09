/*
  ==============================================================================

    HorizontalMeter.h
    Created: 5 Oct 2024 12:08:18pm
    Author:  Emir

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>


namespace Gui
{
    class HorizontalMeter : public juce::Component
    {
        
    public:
        juce::Image grill;

        HorizontalMeter()
        {
            grill = juce::ImageCache::getFromMemory(BinaryData::MeterGrill_png, BinaryData::MeterGrill_pngSize);
        }
        void paint(juce::Graphics& g) override
        {
            auto bounds = getLocalBounds().toFloat();

            g.setColour(juce::Colours::white.withBrightness(0.4f));
            g.fillRoundedRectangle(bounds, 5.f);

            if (!grill.isNull())  // Resim yüklenmiþse çiz
            {
                g.drawImage(grill, bounds);
            }

            g.setColour(juce::Colours::white);

            // map level from -60.f / +6.f to 0 / width
            const auto scaledY = juce::jmap(level, -60.f, +6.f, 0.f, static_cast<float>(getHeight()));


            auto fillBounds = bounds;
            fillBounds.setHeight(scaledY);

            g.fillRoundedRectangle(fillBounds.withY(getHeight() - scaledY), 5.f);

        }

        void setLevel(const float value) { level = value; }
    private:
        float level = -60.f;
    };
}

