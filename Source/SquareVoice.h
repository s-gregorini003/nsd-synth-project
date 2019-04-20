/*
  ==============================================================================

    SineVoice.h
    Created: 9 Apr 2019 11:39:15am
    Author:  Silvio

  ==============================================================================
*/

#ifndef SQUAREVOICE_H_INCLUDED
#define SQUAREVOICE_H_INCLUDED
#include "SquareOsc.h"
#include "ADSREnv.h"


//===============================================================================
/* Demo synth sound */
class SquareSound : public SynthesiserSound
{
public:

	bool appliesToNote(int /*midiNoteNumber*/) override { return true; }
	bool appliesToChannel(int /*midiChannel*/) override { return true;  }

};

//===============================================================================
/* Synth voice that plays a sine wave */
class SquareVoice : public SynthesiserVoice
{
public:
	SquareVoice()
	{
		samplingRate = getSampleRate();
		squOsc.setSampleRate(samplingRate);
		ampEnv.setSampleRate(samplingRate);
		
	}
	
	bool canPlaySound(SynthesiserSound* sound) override
	{
	return dynamic_cast<SquareSound*> (sound) != nullptr;
	}

	void startNote(int midiNoteNumber, float velocity, SynthesiserSound *sound, int currentPitchWheelPosition) override
	{
		currentAngle = 0.0;
		level = velocity * 0.15;
		tailOff = 0.0;


		//auto cyclesPerSecond = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
		//auto cyclesPerSample = cyclesPerSecond / getSampleRate();

		const double noteFreqHz = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
		squOsc.setFrequency(noteFreqHz);

		std::cout << midiNoteNumber << std::endl;

		//angleDelta = cyclesPerSample * 2.0 * MathConstants<double>::pi;
		ampEnv.triggerOn();
	}

	void stopNote(float velocity, bool allowTailOff) override
	{
		ampEnv.triggerOff();


		/*if (allowTailOff) {
			if (tailOff == 0.0)
				tailOff = 1.0;
		}
		else
		{
			clearCurrentNote();
			angleDelta = 0.0;
		}
		*/
	}

	void pitchWheelMoved(int /*newPitchWheelValue*/) override
	{

	}

	void controllerMoved(int controllerNumber, int newControllerValue) override
	{

	}

	void renderNextBlock(AudioBuffer<float>&outputBuffer, int startSample, int numSamples) override
	{
		//if (selectedOsc == 1) {

			if (squOsc.getFrequency() != 0.0)
			{

				while (--numSamples >= 0) {

					const double oscOut = (double)(squOsc.getSignal() * level * ampEnv.getCurve());

					for (int i = outputBuffer.getNumChannels(); --i >= 0;)

						outputBuffer.addSample(i, startSample, oscOut);

					++startSample;

					if (ampEnv.envStatus() == false) {

						clearCurrentNote();
						squOsc.reset();
						break;

					}
				}

			}
		//}

		
	}


	/*void setOscillator(int oscNumber) {

		selectedOsc = oscNumber;
	}*/

private:
	
	double currentAngle = 0.0;
	double angleDelta = 0.0;
	double level;
	double samplingRate;
	double tailOff = 0.0;
	const double voiceAttenuator = 0.6;
	SquareOsc squOsc;
	ADSRenv ampEnv;
	//int selectedOsc;
};


#endif //SINEVOICE_H_INCLUDED