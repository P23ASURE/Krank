/*
  ==============================================================================

    LevelMeterComponent.h
    Created: 27 Nov 2023 11:28:09am
    Author:  Marco

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class LevelMeterComponent : public juce::Component {
public:
    LevelMeterComponent() {}

    void setLevel(float newLevel) {
        level = newLevel;
        repaint(); 
    }

protected:
    void paint(juce::Graphics& g) override {
        auto area = getLocalBounds().toFloat();
        g.setColour(juce::Colours::grey);
        g.fillRect(area); 

        auto filledArea = area.removeFromBottom(area.getHeight() * level);

        
        if (level > 0.8) { 
            g.setColour(juce::Colours::red);
        }
        else {
            g.setColour(juce::Colours::green);
        }

        g.fillRect(filledArea); 
    }

private:
    float level = 0.0f;
};

