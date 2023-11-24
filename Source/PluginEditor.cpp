/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

using namespace juce;
//==============================================================================
MidiBallAudioProcessorEditor::MidiBallAudioProcessorEditor(MidiBallAudioProcessor& p)
	: AudioProcessorEditor(&p), audioProcessor(p)
{
	// Make sure that before the constructor has finished, you've set the
	// editor's size to whatever you need it to be.
	setSize(400, 300);
	if (PluginHostType::getPluginLoadedAs() == AudioProcessor::wrapperType_Standalone)
	{
		addAndMakeVisible(midiDropdown);
		midiDropdown.changeWidthToFitText(24);
		midiDropdown.setTopLeftPosition(20, 20);
		midiDropdown.setTriggeredOnMouseDown(true);
		midiDropdown.setAlwaysOnTop(true);
		midiDropdown.onClick = [this] { showMenu(); };
	}
}

MidiBallAudioProcessorEditor::~MidiBallAudioProcessorEditor()
{
}

void MidiBallAudioProcessorEditor::showMenu()
{
	Array<MidiDeviceInfo> midiDevices = MidiOutput::getAvailableDevices();
	PopupMenu m;
	
	int i = 1;
	m.addItem(i++, "None (disable)", true, outputId == i);
	for (MidiDeviceInfo device : midiDevices)
	{
		m.addItem(i++, device.name, true, outputId == i);
	}

	m.showMenuAsync(PopupMenu::Options().withTargetComponent(midiDropdown),
		ModalCallbackFunction::forComponent(menuItemChosenCallback, this));
}

void MidiBallAudioProcessorEditor::menuItemChosenCallback(int result, MidiBallAudioProcessorEditor* editor)
{
	if (result != 0 && editor != nullptr)
	{
		editor->audioProcessor.setMidiOutput(result - 2);
		editor->outputId = result;
	}
}

//==============================================================================
void MidiBallAudioProcessorEditor::paint(Graphics& g)
{
	// (Our component is opaque, so we must completely fill the background with a solid colour)
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));

	g.setColour(Colours::white);
	g.setFont(15.0f);
	g.drawFittedText("Hello World!", getLocalBounds(), Justification::centred, 1);
}

void MidiBallAudioProcessorEditor::resized()
{
	// This is generally where you'll want to lay out the positions of any
	// subcomponents in your editor..
}