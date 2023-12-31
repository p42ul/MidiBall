/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Ball.h"

//==============================================================================
/**
*/
class MidiBallAudioProcessor : public juce::AudioProcessor, private juce::Timer
{
public:
	//==============================================================================
	MidiBallAudioProcessor();
	~MidiBallAudioProcessor() override;
	void timerCallback() override;

	//==============================================================================
	void prepareToPlay(double sampleRate, int samplesPerBlock) override;
	void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
	bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

	void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

	//==============================================================================
	juce::AudioProcessorEditor* createEditor() override;
	bool hasEditor() const override;

	//==============================================================================
	const juce::String getName() const override;

	bool acceptsMidi() const override;
	bool producesMidi() const override;
	bool isMidiEffect() const override;
	double getTailLengthSeconds() const override;

	//==============================================================================
	int getNumPrograms() override;
	int getCurrentProgram() override;
	void setCurrentProgram(int index) override;
	const juce::String getProgramName(int index) override;
	void changeProgramName(int index, const juce::String& newName) override;

	//==============================================================================
	void getStateInformation(juce::MemoryBlock& destData) override;
	void setStateInformation(const void* data, int sizeInBytes) override;
	void setMidiOutput(int id);

	void addBall();
	void removeBall();
	void updateBalls();
	void bounce(Ball& ball);
	std::vector<Ball> balls;

	int areaWidth;
	int areaHeight;


private:
	//==============================================================================
	void sendMidi(juce::MidiMessage);



	std::unique_ptr<juce::MidiOutput> standaloneMidiOutput = nullptr;
	std::queue<juce::MidiMessage> midiQueue;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiBallAudioProcessor);
};
