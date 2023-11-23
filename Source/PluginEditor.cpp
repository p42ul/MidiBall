/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MidiBallAudioProcessorEditor::MidiBallAudioProcessorEditor(MidiBallAudioProcessor& p)
	: AudioProcessorEditor(&p), audioProcessor(p)
{
	// Make sure that before the constructor has finished, you've set the
	// editor's size to whatever you need it to be.
	setSize(400, 300);
	midiOutput = juce::MidiOutput::openDevice(0);
	startTimerHz(1);
}

MidiBallAudioProcessorEditor::~MidiBallAudioProcessorEditor()
{
}

//==============================================================================
void MidiBallAudioProcessorEditor::paint(juce::Graphics& g)
{
	// (Our component is opaque, so we must completely fill the background with a solid colour)
	g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

	g.setColour(juce::Colours::white);
	g.setFont(15.0f);
	g.drawFittedText("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void MidiBallAudioProcessorEditor::resized()
{
	// This is generally where you'll want to lay out the positions of any
	// subcomponents in your editor..
}

void MidiBallAudioProcessorEditor::timerCallback()
{
	sendMidi();
}

void MidiBallAudioProcessorEditor::sendMidi()
{
	juce::MidiMessage message = juce::MidiMessage::noteOn(1, 60, 1.0f);

	// Send the MIDI message
	if (midiOutput)
		midiOutput->sendMessageNow(message);
}