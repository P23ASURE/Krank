/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include <math.h>
#include "LevelMeterComponent.h"

# define M_PI           3.14159265358979323846 
//==============================================================================
/**
*/

class OtherLookAndFeel : public LookAndFeel_V4
{
public:
    OtherLookAndFeel()
    {
        setColour(Slider::thumbColourId, Colours::red);
       
        knobImage = juce::ImageCache::getFromMemory(BinaryData::knob_png, BinaryData::knob_pngSize);
    }

    void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
        const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override
    {
       
        int numFrames = knobImage.getHeight() / knobImage.getWidth();
        int frameIndex = static_cast<int>(std::round(sliderPos * (numFrames - 1)));
        int frameHeight = knobImage.getWidth();

        
        g.drawImage(knobImage,
            x, y, width, height,
            0, frameIndex * frameHeight, knobImage.getWidth(), frameHeight); 
    }

private:
    juce::Image knobImage; 
};


class AnalogFattenerAudioProcessorEditor  : public AudioProcessorEditor,public Slider::Listener, public juce::Timer
{
public:
    AnalogFattenerAudioProcessorEditor (AnalogFattenerAudioProcessor&);
    ~AnalogFattenerAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void timerCallback() override;
    

    void sliderValueChanged(Slider* slider) override
    {
        if (slider == &crankSlider) 
        {
            processor.crankValue = static_cast<float>(crankSlider.getValue());
        }
        if (slider == &colorSlider)
        {
            processor.colorValue = static_cast<float>(colorSlider.getValue());
            processor.const1 = processor.colorValue / 200 * M_PI;
            processor.const2 = sin(processor.colorValue / 200 * M_PI);
        }
        if (slider == &boostSlider)
        {
            processor.boostValue = static_cast<float>(boostSlider.getValue());
        }
        if (slider == &limitSlider)
        {
            processor.limitValue = static_cast<float>(limitSlider.getValue());
            processor.threshold_dB = processor.baseline_threshold_dB + processor.limitValue;
        }
    }

private:

    OtherLookAndFeel sliderLookAndFeel;
      
    Slider crankSlider;
    Slider colorSlider;
    Slider boostSlider;
    Slider limitSlider;
    Label crankLabel;
    Label colorLabel;
    Label boostLabel;
    Label limitLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> crankAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> colorAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> boostAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> limitAttachment;

    const float border = 100.0;
    float lowerKnobSize = 70.0;

    juce::Image backgroundImage;
    juce::Image knobImage;

    float rmsValue = 0.0f; 
    float smoothedRmsValue = 0.0f; 

    LevelMeterComponent levelMeter;

    AnalogFattenerAudioProcessor& processor;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AnalogFattenerAudioProcessorEditor)
};
