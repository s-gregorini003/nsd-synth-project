/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/


#ifndef SOUNDCOMPONENT_H_INCLUDED
#define SOUNDCOMPONENT_H_INCLUDED
#include "SquareVoice.h"

//==============================================================================
class MainComponent   : public Component,
						public AudioIODeviceCallback,
						public MidiInputCallback,
						public Button::Listener
						//public Slider::Listener
	
{
public:
	MainComponent() 
		: midiKeyboardState(),
		midiKeyboardComponent(midiKeyboardState, MidiKeyboardComponent::Orientation::horizontalKeyboard)
	
	{
		audioDeviceManager.initialiseWithDefaultDevices(0, 2);
		audioDeviceManager.addMidiInputCallback(String::empty, this);
		audioDeviceManager.addAudioCallback(this);

		StringArray devices = MidiInput::getDevices();
		for (int i = 0; i < devices.size(); ++i)
			audioDeviceManager.setMidiInputEnabled(devices[i], true);

		midiKeyboardState.addListener(&midiCollector);

		synth.addSound(new SquareSound());

		for (int i = 0; i < maxNumVoices; ++i)
		{
			synth.addVoice(new SquareVoice());
		}

		addAndMakeVisible(midiKeyboardComponent);

		masterPower = true;

		

		// Make sure you set the size of the component after
		// you add any child components.
		setSize(1200, 600);
	}



	MainComponent::~MainComponent()
	{
		// This shuts down the audio device and clears the audio source.
		audioDeviceManager.removeAudioCallback(this);
		audioDeviceManager.removeMidiInputCallback(String::empty, this);
	}


	//==============================================================================
	void paint(Graphics& g)
	{
		// (Our component is opaque, so we must completely fill the background with a solid colour)
		g.fillAll(Colours::black);

		// You can add your drawing code here!
	}

	void resized() override
	{
		// This is called when the MainContentComponent is resized.
		// If you add any child components, this is where you should
		// update their positions.

		const int margin = 11;
		juce::Rectangle<int> r = getLocalBounds();
		midiKeyboardComponent.setBounds(r.removeFromBottom(8 * margin));

	}

	//===============================================================================
	void audioDeviceIOCallback(const float** inputChannelData, int inputNumChannels, float** outputChannelData, int numOutputChannels, int numSamples) override
	{
		AudioBuffer<float> buffer(outputChannelData, numOutputChannels, numSamples);
		buffer.clear();

		MidiBuffer incomingMidi;
		midiCollector.removeNextBlockOfMessages(incomingMidi, numSamples);

		if (masterPower) {
			synth.renderNextBlock(buffer, incomingMidi, 0, numSamples);
		}
	}

	//===============================================================================
	void audioDeviceAboutToStart(AudioIODevice* device) override
	{
		const double sampleRate = device->getCurrentSampleRate();
		midiCollector.reset(sampleRate);
		synth.setCurrentPlaybackSampleRate(sampleRate);
	}

	//===============================================================================
	virtual void audioDeviceStopped() override
	{
	
	}

	//===============================================================================
	void handleIncomingMidiMessage(MidiInput *source, const MidiMessage &message) override
	{
		midiCollector.addMessageToQueue(message);
	}

	//===============================================================================
	void buttonClicked(Button *buttonThatWasClicked)
	{
		/*if (buttonThatWasClicked == &squareOsc) {
			sineOsc.setToggleState(true, dontSendNotification);

			testVoice.setOscillator(1);
		}*/
	}

	//===============================================================================
	void sliderValueChanged(Slider *slider)
	{
		
	}

private:

	AudioDeviceManager audioDeviceManager;
	Synthesiser synth;
	MidiMessageCollector midiCollector;
	MidiKeyboardState midiKeyboardState;
	MidiKeyboardComponent midiKeyboardComponent;
	const int maxNumVoices = 16;
	//SquareVoice testVoice;

	bool masterPower;
	//TextButton squareOsc;
	//TextButton sineOsc;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

#endif  // SOUNDCOMPONENT_H_INCLUDED