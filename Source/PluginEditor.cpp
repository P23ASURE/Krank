/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
AnalogFattenerAudioProcessorEditor::AnalogFattenerAudioProcessorEditor(AnalogFattenerAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    startTimerHz(30);
    backgroundImage = juce::ImageCache::getFromMemory(BinaryData::background_png, BinaryData::background_pngSize);
    

    setSize(400, 500);

    setLookAndFeel(&sliderLookAndFeel);

    crankSlider.setRange(1,1.2,0.01);
    colorSlider.setRange(1,100,1);
    boostSlider.setRange(0,9,0.1);
    limitSlider.setRange(-9,0,0.1);

    crankSlider.setValue(1);
    colorSlider.setValue(1.0);
    boostSlider.setValue(0.0);
    limitSlider.setValue(-3.0);


    crankSlider.addListener(this);
    colorSlider.addListener(this);
    boostSlider.addListener(this);
    limitSlider.addListener(this);

    crankSlider.setSliderStyle(Slider::Rotary);
    crankSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);

    colorSlider.setSliderStyle(Slider::Rotary);
    colorSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);

    boostSlider.setSliderStyle(Slider::Rotary);
    boostSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);

    limitSlider.setSliderStyle(Slider::Rotary);
    limitSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);

    addAndMakeVisible(crankSlider);
    addAndMakeVisible(colorSlider);
    addAndMakeVisible(boostSlider);
    addAndMakeVisible(limitSlider);

    // Crea gli attachment
    crankAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.getParameters(), "crack", crankSlider);
    colorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.getParameters(), "color", colorSlider);
    boostAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.getParameters(), "boost", boostSlider);
    limitAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.getParameters(), "limit", limitSlider);



    crankLabel.setText("Crank", NotificationType::dontSendNotification);
    crankLabel.setJustificationType(Justification::centredTop);
    crankLabel.attachToComponent(&crankSlider, false);

    colorLabel.setText("Color", NotificationType::dontSendNotification);
    colorLabel.setJustificationType(Justification::centredTop);
    colorLabel.attachToComponent(&colorSlider, false);

    boostLabel.setText("Boost", NotificationType::dontSendNotification);
    boostLabel.setJustificationType(Justification::centredTop);
    boostLabel.attachToComponent(&boostSlider, false);

    limitLabel.setText("Limit", NotificationType::dontSendNotification);
    limitLabel.setJustificationType(Justification::centredTop);
    limitLabel.attachToComponent(&limitSlider, false);

    addAndMakeVisible(levelMeter);
    int meterWidth = 20;
    int meterHeight = getHeight() / 2;
    int meterX = getWidth() - meterWidth - 10; 
    int meterY = (getHeight() - meterHeight) / 2; 

    levelMeter.setBounds(meterX, meterY, meterWidth, meterHeight);

}

void AnalogFattenerAudioProcessorEditor::timerCallback() {
    auto rmsLevel = processor.getSmoothedRmsValue();
    levelMeter.setLevel(rmsLevel);
}

AnalogFattenerAudioProcessorEditor::~AnalogFattenerAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

//==============================================================================
void AnalogFattenerAudioProcessorEditor::paint (Graphics& g)
{
    g.drawImageAt(backgroundImage, 0, 0);
    g.setColour (Colours::white);
    g.setFont (15.0f);
}

void AnalogFattenerAudioProcessorEditor::resized()
{
    crankSlider.setBounds(getWidth() / 2 - 100, getHeight() / 2 - 150, 200, 200);
    colorSlider.setBounds(border-35,(getHeight()-border-35),lowerKnobSize,lowerKnobSize);
    boostSlider.setBounds(getWidth()/2-35,getHeight()-border-35,lowerKnobSize,lowerKnobSize);  
    limitSlider.setBounds(getWidth()-border-35,(getHeight()-border-35),lowerKnobSize,lowerKnobSize);

    int meterWidth = 20;
    int meterHeight = getHeight() / 2;
    int meterX = getWidth() - meterWidth - 10; 
    int meterY = (getHeight() - meterHeight) / 2; 

    levelMeter.setBounds(meterX, meterY, meterWidth, meterHeight);
}
