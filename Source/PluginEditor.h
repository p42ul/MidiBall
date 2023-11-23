/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class MidiBallAudioProcessorEditor : public juce::AudioProcessorEditor, private juce::Timer
{
public:
	MidiBallAudioProcessorEditor(MidiBallAudioProcessor&);
	~MidiBallAudioProcessorEditor() override;

	//==============================================================================
	void paint(juce::Graphics&) override;
	void resized() override;
	void timerCallback() override;



private:
	// This reference is provided as a quick way for your editor to
	// access the processor object that created it.

	void sendMidi();
	MidiBallAudioProcessor& audioProcessor;
	std::unique_ptr<juce::MidiOutput> midiOutput;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiBallAudioProcessorEditor)
};
