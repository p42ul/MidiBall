/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Ball.h"

//==============================================================================
MidiBallAudioProcessor::MidiBallAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
	)
#endif
{
	areaWidth = defaultWidth;
	areaHeight = defaultHeight;
	startTimerHz(60);
}

MidiBallAudioProcessor::~MidiBallAudioProcessor()
{
}

void MidiBallAudioProcessor::timerCallback()
{
	updateBalls();

	/*MidiMessage message = juce::MidiMessage::noteOn(1, 60, 1.0f);
	if (PluginHostType::getPluginLoadedAs() == AudioProcessor::wrapperType_Standalone)
	{
		sendMidi(message);
	}
	else
	{
		midiQueue.push(message);
	}*/

}

//==============================================================================
const juce::String MidiBallAudioProcessor::getName() const
{
	return JucePlugin_Name;
}

bool MidiBallAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

bool MidiBallAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

bool MidiBallAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
	return true;
#else
	return false;
#endif
}

double MidiBallAudioProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

int MidiBallAudioProcessor::getNumPrograms()
{
	return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
	// so this should be at least 1, even if you're not really implementing programs.
}

int MidiBallAudioProcessor::getCurrentProgram()
{
	return 0;
}

void MidiBallAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String MidiBallAudioProcessor::getProgramName(int index)
{
	return {};
}

void MidiBallAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void MidiBallAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
}

void MidiBallAudioProcessor::releaseResources()
{
	// When playback stops, you can use this as an opportunity to free up any
	// spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MidiBallAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
	juce::ignoreUnused(layouts);
	return true;
#else
	// This is the place where you check if the layout is supported.
	// In this template code we only support mono or stereo.
	// Some plugin hosts, such as certain GarageBand versions, will only
	// load plugins that support stereo bus layouts.
	if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
		&& layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
		return false;

	// This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
	if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
		return false;
#endif

	return true;
#endif
}
#endif

void MidiBallAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	// Because this is a MIDI device, it should have no audio channels.
	jassert(buffer.getNumChannels() == 0);

	while (!midiQueue.empty())
	{
		midiMessages.addEvent(midiQueue.front(), 0);
		midiQueue.pop();
	}
}

//==============================================================================
bool MidiBallAudioProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MidiBallAudioProcessor::createEditor()
{
	return new MidiBallAudioProcessorEditor(*this);
}

//==============================================================================
void MidiBallAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.
}

void MidiBallAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	// You should use this method to restore your parameters from this memory block,
	// whose contents will have been created by the getStateInformation() call.
}

void MidiBallAudioProcessor::setMidiOutput(int id)
{
	if (id == -1)
		standaloneMidiOutput = nullptr;
	else
		standaloneMidiOutput = MidiOutput::openDevice(id);
}

void MidiBallAudioProcessor::addBall()
{
	Random random;
	Ball ball;
	ball.x = areaWidth / 2;
	ball.y = areaHeight / 2;
	ball.radius = (random.nextFloat() * 50.0f) + 5.0f;
	ball.note = random.nextInt(Range(32, 127));
	ball.dx = (random.nextFloat() - 0.5f) * 4.0f;
	ball.dy = (random.nextFloat() - 0.5f) * 4.0f;
	ball.color = Colour::Colour(random.nextInt(256), random.nextInt(256), random.nextInt(256));
	balls.push_back(ball);
}

void MidiBallAudioProcessor::updateBalls()
{
	for (Ball& ball : balls)
	{
		// Bounce against edge of window
		if (ball.x < 0)
		{
			ball.dx = std::abs(ball.dx);
			bounce(ball);
		}

		if (ball.x > areaWidth - ball.radius)
		{
			ball.dx = -std::abs(ball.dx);
			bounce(ball);
		}

		if (ball.y < 0)
		{
			ball.dy = std::abs(ball.dy);
			bounce(ball);
		}

		if (ball.y > areaHeight - ball.radius)
		{
			ball.dy = -std::abs(ball.dy);
			bounce(ball);
		}


		// Apply velocity
		ball.x += ball.dx;
		ball.y += ball.dy;
	}
}

void MidiBallAudioProcessor::bounce(Ball& ball)
{
	MidiMessage message = juce::MidiMessage::noteOn(1, ball.note, 1.0f);
	if (PluginHostType::getPluginLoadedAs() == AudioProcessor::wrapperType_Standalone)
	{
		sendMidi(message);
	}
	else
	{
		midiQueue.push(message);
	}
}

void MidiBallAudioProcessor::sendMidi(MidiMessage message)
{
	if (standaloneMidiOutput)
		standaloneMidiOutput->sendMessageNow(message);
}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new MidiBallAudioProcessor();
}

