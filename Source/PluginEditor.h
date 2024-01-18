/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

using namespace juce;
//==============================================================================
/**
*/
class MidiBallAudioProcessorEditor : public AudioProcessorEditor, private Timer
{
public:
	MidiBallAudioProcessorEditor(MidiBallAudioProcessor&);
	~MidiBallAudioProcessorEditor() override;



	//==============================================================================
	void paint(Graphics&) override;
	void resized() override;
	void timerCallback() override;




private:
	// This reference is provided as a quick way for your editor to
	// access the processor object that created it.


	MidiBallAudioProcessor& audioProcessor;
	TextButton midiDropdown{ "MIDI Devices" };
	TextButton addBallButton{ "Add Ball" };
	TextButton removeBallButton{ "Remove Ball" };
	void showMenu();
	int outputId = 1;

	static void menuItemChosenCallback(int result, MidiBallAudioProcessorEditor* editor);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiBallAudioProcessorEditor)
};
