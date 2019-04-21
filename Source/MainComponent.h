/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/


#ifndef SOUNDCOMPONENT_H_INCLUDED
#define SOUNDCOMPONENT_H_INCLUDED
#include "SynthVoice.h"



//==============================================================================
class MainComponent   : public Component,
						public AudioIODeviceCallback,
						public MidiInputCallback,
						public Button::Listener
					//	private MidiKeyboardStateListener
						//public Slider::Listener
	
{
public:
	MainComponent() 
		: midiKeyboardState(),
		midiKeyboardComponent(midiKeyboardState, MidiKeyboardComponent::Orientation::horizontalKeyboard)
	
	{
		synth.clearVoices();
		audioDeviceManager.initialise(0, 2, nullptr, true, String(), nullptr);
		audioDeviceManager.addMidiInputCallback(String::empty, this);
		audioDeviceManager.addAudioCallback(this);

		StringArray devices = MidiInput::getDevices();


		for (int i = 0; i < devices.size(); ++i) {
			audioDeviceManager.setMidiInputEnabled(devices[i], true);

			}

		audioDeviceManager.addMidiInputCallback(String(), this);
		midiKeyboardState.addListener(&midiCollector);

		synth.clearSounds();
		synth.addSound(new SynthSound());

		for (int i = 0; i < maxNumVoices; ++i)
		{
			synth.addVoice(new SynthVoice());
		}


		addAndMakeVisible(midiKeyboardComponent);

		masterPower = true;

		addAndMakeVisible(squareOscButton1);
		squareOscButton1.setClickingTogglesState(true);
		squareOscButton1.setToggleState(true, dontSendNotification);
		squareOscButton1.setButtonText("square");
		squareOscButton1.addListener(this);

		addAndMakeVisible(sineOscButton1);
		sineOscButton1.setClickingTogglesState(true);
		sineOscButton1.setToggleState(false, dontSendNotification);
		sineOscButton1.setButtonText("sine");
		sineOscButton1.addListener(this);

		addAndMakeVisible(squareOscButton2);
		squareOscButton2.setClickingTogglesState(true);
		squareOscButton2.setToggleState(true, dontSendNotification);
		squareOscButton2.setButtonText("square");
		squareOscButton2.addListener(this);

		addAndMakeVisible(sineOscButton2);
		sineOscButton2.setClickingTogglesState(true);
		sineOscButton2.setToggleState(false, dontSendNotification);
		sineOscButton2.setButtonText("sine");
		sineOscButton2.addListener(this);


		

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

		squareOscButton1.setBounds(30, 30, 100, 20);
		sineOscButton1.setBounds(180, 30, 100, 20);
		squareOscButton2.setBounds(30, 70, 100, 20);
		sineOscButton2.setBounds(180, 70, 100, 20);

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
	void handleIncomingMidiMessage(MidiInput* /*source*/, const MidiMessage &message) override
	{
		midiCollector.addMessageToQueue(message);
		
	}

	//===============================================================================
	void buttonClicked(Button *buttonThatWasClicked)
	{
		if (buttonThatWasClicked == &squareOscButton1) {
			squareOscButton1.setState(Button::buttonDown);
			sineOscButton1.setState(Button::buttonNormal);
			sineOscButton1.setToggleState(false, dontSendNotification);
			int square = 1;
			for (int i = 0; i < maxNumVoices; ++i)
			{
				synth.getVoice(i)->controllerMoved(100, (int) (square) );

				DBG("squareButton1 pressed");
			}
			
		}

		else if (buttonThatWasClicked == &sineOscButton1) {
			squareOscButton1.setToggleState(false, dontSendNotification);
			sineOscButton1.setState(Button::buttonDown);
			squareOscButton1.setState(Button::buttonNormal);
			int sine = 2;
			for (int i = 0; i < maxNumVoices; ++i)
			{
				synth.getVoice(i)->controllerMoved(100, (int) (sine) );

				DBG("sineButton1 pressed");
			}
		}

		if (buttonThatWasClicked == &squareOscButton2) {
			squareOscButton2.setState(Button::buttonDown);
			sineOscButton2.setState(Button::buttonNormal);
			sineOscButton2.setToggleState(false, dontSendNotification);
			int square = 1;
			for (int i = 0; i < maxNumVoices; ++i)
			{
				synth.getVoice(i)->controllerMoved(101, (int)(square));

				DBG("squareButton2 pressed");
			}

		}

		else if (buttonThatWasClicked == &sineOscButton2) {
			squareOscButton2.setToggleState(false, dontSendNotification);
			sineOscButton2.setState(Button::buttonDown);
			squareOscButton2.setState(Button::buttonNormal);
			int sine = 2;
			for (int i = 0; i < maxNumVoices; ++i)
			{
				synth.getVoice(i)->controllerMoved(101, (int)(sine));

				DBG("sineButton2 pressed");
			}
		}
	}

	//===============================================================================
	void sliderValueChanged(Slider *slider)
	{
		
	}

	
	//===============================================================================


private:

	//===============================================================================
	
		
	AudioDeviceManager audioDeviceManager;
	Synthesiser synth;
	MidiMessageCollector midiCollector;
	MidiKeyboardState midiKeyboardState;
	MidiKeyboardComponent midiKeyboardComponent;
	const int maxNumVoices = 16;

	MidiInputCallback* midiInputCallback;

	
	TextButton squareOscButton1, squareOscButton2, sineOscButton1, sineOscButton2;
	bool masterPower;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

#endif  // SOUNDCOMPONENT_H_INCLUDED