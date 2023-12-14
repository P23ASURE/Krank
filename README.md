# AnalogFattenerAudioProcessor

## Introduction
The "AnalogFattener" plugin is an audio processor developed using the JUCE framework. This plugin is designed to enrich the audio signal with an analog "fattening" effect, adding warmth and body to the sound.

## Features
The plugin offers several parameters to modulate the effect:

- **Crank (`crankValue`):** Controls the intensity of the effect, increasing the amplitude of the signal.
- **Color (`colorValue`):** Modifies the tonal characteristics of the signal through trigonometric processing, adding "color" to the sound.
- **Boost (`boostValue`):** Increases the overall level of the signal, bringing it more to the forefront.
- **Limit (`limitValue`):** Sets a maximum threshold for the signal, preventing unwanted volume peaks and protecting from overexposures.

## Usage
1. Add the plugin to your effects chain in the DAW software.
2. Adjust the `Crank`, `Color`, `Boost`, and `Limit` parameters to achieve the desired sound.
3. Experiment with different settings to explore a wide range of sound effects.

## Signal Level Visualization
The plugin includes a level meter that uses a smoothed RMS (Root Mean Square) value to display the average amplitude of the signal in a visually fluid manner. This is useful for monitoring the impact of the effect on the audio signal.


![Krank](https://github.com/P23ASURE/Krank/blob/master/krank_screenshot.png)
