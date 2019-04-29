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
						public Button::Listener,
						public Slider::Listener,
						public ComboBox::Listener
						
	
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

		//==============================================================================

		//Buttons initialisation
		squareOscButton1.setClickingTogglesState(true);
		squareOscButton1.setToggleState(true, dontSendNotification);
		squareOscButton1.setButtonText("square");
		squareOscButton1.addListener(this);
		addAndMakeVisible(squareOscButton1);

		sineOscButton1.setClickingTogglesState(true);
		sineOscButton1.setToggleState(false, dontSendNotification);
		sineOscButton1.setButtonText("sine");
		sineOscButton1.addListener(this);
		addAndMakeVisible(sineOscButton1);

		squareOscButton2.setClickingTogglesState(true);
		squareOscButton2.setToggleState(true, dontSendNotification);
		squareOscButton2.setButtonText("square");
		squareOscButton2.addListener(this);
		addAndMakeVisible(squareOscButton2);

		sineOscButton2.setClickingTogglesState(true);
		sineOscButton2.setToggleState(false, dontSendNotification);
		sineOscButton2.setButtonText("sine");
		sineOscButton2.addListener(this);
		addAndMakeVisible(sineOscButton2);

		//==============================================================================

		//ComboBoxes initialisation
		wavesOsc1.setEditableText(false);
		wavesOsc1.addItem("square", 1);
		wavesOsc1.addItem("sine", 2);
		wavesOsc1.addItem("saw", 3);
		wavesOsc1.addItem("pulse", 4);
		wavesOsc1.setJustificationType(Justification::centred); 
		wavesOsc1.setSelectedId(1);
		wavesOsc1.addListener(this);
		addAndMakeVisible(wavesOsc1);

		filterTypeMenu.setEditableText(false);
		filterTypeMenu.addItem("none", 1);
		filterTypeMenu.addItem("low pass", 2);
		filterTypeMenu.addItem("high pass", 3);
		filterTypeMenu.addItem("band pass", 4);
		filterTypeMenu.setJustificationType(Justification::centred);
		filterTypeMenu.setSelectedId(1);
		filterTypeMenu.addListener(this);
		addAndMakeVisible(filterTypeMenu);

		lfoDestMenu.setEditableText(false);
		lfoDestMenu.addItem("none", 1);
		lfoDestMenu.addItem("amp", 2);
		lfoDestMenu.addItem("cutoff", 3);
		lfoDestMenu.setJustificationType(Justification::centred);
		lfoDestMenu.setSelectedId(1);
		lfoDestMenu.addListener(this);
		addAndMakeVisible(lfoDestMenu);

		//==============================================================================

		//Sliders initialisation
		attack.setSliderStyle(Slider::SliderStyle::LinearVertical);
		attack.setRange(0.1, 10.0, 0.1);
		attack.setValue(1.0);
		attack.addListener(this);
		addAndMakeVisible(attack);

		decay.setSliderStyle(Slider::SliderStyle::LinearVertical);
		decay.setRange(0.1, 10.0, 0.1);
		decay.setValue(0.1);
		decay.addListener(this);
		addAndMakeVisible(decay);

		sustain.setSliderStyle(Slider::SliderStyle::LinearVertical);
		sustain.setRange(0.1, 1.0, 0.1);
		sustain.setValue(1.0);
		sustain.addListener(this);
		addAndMakeVisible(sustain);

		release.setSliderStyle(Slider::SliderStyle::LinearVertical);
		release.setRange(0.1, 10.0, 0.1);
		release.setValue(0.1);
		release.addListener(this);
		addAndMakeVisible(release);



		cutoffFreq.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
		cutoffFreq.setRange(20.0, 10000.0, 1.0);
		cutoffFreq.setValue(400.0);
		cutoffFreq.setSkewFactorFromMidPoint(1000.0);
		cutoffFreq.addListener(this);
		addAndMakeVisible(cutoffFreq);

		resonance.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
		resonance.setRange(1, 10);
		resonance.setValue(2);
		resonance.addListener(this);
		addAndMakeVisible(resonance);

		

		lfoRate.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
		lfoRate.setRange(1.0, 15.0, 1.0);
		lfoRate.setValue(2.);
		lfoRate.addListener(this);
		addAndMakeVisible(lfoRate);

		lfoDepth.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
		lfoDepth.setRange(1., 10.0, 1.);
		lfoDepth.setValue(1.);
		lfoDepth.addListener(this);
		addAndMakeVisible(lfoDepth);

		// Make sure you set the size of the component after
		// you add any child components.
		setSize(1200, 800);
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

		wavesOsc1.setBounds(500, 300, 200, 40);

		attack.setBounds(500, 50, 20, 150);
		decay.setBounds(540, 50, 20, 150);
		sustain.setBounds(580, 50, 20, 150);
		release.setBounds(620, 50, 20, 150);

		filterTypeMenu.setBounds(30, 200, 100, 20);
		cutoffFreq.setBounds(30, 240, 70, 70);
		resonance.setBounds(100, 240, 70, 70);

		lfoDestMenu.setBounds(30, 500, 100, 20);
		lfoRate.setBounds(30, 540, 70, 70);
		lfoDepth.setBounds(100, 540, 70, 70);

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
			sineOscButton2.setToggleState(false, dontSendNotification);
			squareOscButton2.setState(Button::buttonDown);
			sineOscButton2.setState(Button::buttonNormal);
			
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
		if (slider == &attack) {

			for (int i = 0; i < maxNumVoices; ++i)
			{
				synth.getVoice(i)->controllerMoved(102, (float) attack.getValue());

				DBG("attackSlider value changed");
			}
		}

		if (slider == &decay) {

			for (int i = 0; i < maxNumVoices; ++i)
			{
				synth.getVoice(i)->controllerMoved(103, (float)decay.getValue());

				DBG("decaySlider value changed");
			}
		}

		if (slider == &sustain) {

			for (int i = 0; i < maxNumVoices; ++i)
			{
				synth.getVoice(i)->controllerMoved(104, (float)sustain.getValue());

				DBG("sustainSlider value changed");
			}
		}

		if (slider == &release) {

			for (int i = 0; i < maxNumVoices; ++i)
			{
				synth.getVoice(i)->controllerMoved(105, (float)release.getValue());

				DBG("releaseSlider value changed");
			}
		}

		if (slider == &cutoffFreq) {

			for (int i = 0; i < maxNumVoices; ++i)
			{
				synth.getVoice(i)->controllerMoved(107, (float)cutoffFreq.getValue());

				DBG("cutoffSlider value changed");
			}
		}

		if (slider == &resonance) {

			for (int i = 0; i < maxNumVoices; ++i)
			{
				synth.getVoice(i)->controllerMoved(108, (float)resonance.getValue());

				DBG("resSlider value changed");
			}
		}

		if (slider == &lfoRate) {

			for (int i = 0; i < maxNumVoices; ++i)
			{
				synth.getVoice(i)->controllerMoved(110, (float)lfoRate.getValue());

				DBG("lfoFreqSlider value changed");
			}
		}

		if (slider == &lfoDepth) {

			for (int i = 0; i < maxNumVoices; ++i)
			{
				synth.getVoice(i)->controllerMoved(111, (float)lfoDepth.getValue());

				DBG("lfoDepthSlider value changed");
			}
		}
	}

	
	//===============================================================================


	void comboBoxChanged(ComboBox * comboBoxThatHasChanged)
	{

		if (filterTypeMenu.getSelectedId() == 1) {

			int filterType = 1;

			for (int i = 0; i < maxNumVoices; ++i)
			{
				synth.getVoice(i)->controllerMoved(106, (int)filterType);

				DBG("No filter selected");
			}
		}

		else if (filterTypeMenu.getSelectedId() == 2) {

			int filterType = 2;

			for (int i = 0; i < maxNumVoices; ++i)
			{
				synth.getVoice(i)->controllerMoved(106, (int) filterType);

				DBG("Low pass filter selected");
			}
		}

		else if (filterTypeMenu.getSelectedId() == 3) {

			int filterType = 3;

			for (int i = 0; i < maxNumVoices; ++i)
			{
				synth.getVoice(i)->controllerMoved(106, (int)filterType);

				DBG("High pass filter selected");
			}
		}

		else if (filterTypeMenu.getSelectedId() == 4) {

			int filterType = 4;

			for (int i = 0; i < maxNumVoices; ++i)
			{
				synth.getVoice(i)->controllerMoved(106, (int)filterType);

				DBG("Band pass filter selected");
			}
		}

		if (lfoDestMenu.getSelectedId() == 1) {

			int lfoDest = 1;

			for (int i = 0; i < maxNumVoices; ++i)
			{
				synth.getVoice(i)->controllerMoved(109, (int)lfoDest);

				DBG("No LFO selected");
			}
		}

		else if (lfoDestMenu.getSelectedId() == 2) {

			int lfoDest = 2;

			for (int i = 0; i < maxNumVoices; ++i)
			{
				synth.getVoice(i)->controllerMoved(109, (int)lfoDest);

				DBG("Amp LFO selected");
			}
		}

		else if (lfoDestMenu.getSelectedId() == 3) {

			int lfoDest = 3;

			for (int i = 0; i < maxNumVoices; ++i)
			{
				synth.getVoice(i)->controllerMoved(109, (int)lfoDest);

				DBG("Cutoff LFO selected");
			}
		}
	}


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
	ComboBox wavesOsc1, wavesOsc2, filterTypeMenu, lfoDestMenu;
	Slider attack, decay, sustain, release, cutoffFreq, resonance, lfoRate, lfoDepth;

	bool masterPower;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

#endif  // SOUNDCOMPONENT_H_INCLUDED