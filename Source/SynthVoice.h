/*
  ==============================================================================

    SineVoice.h
    Created: 9 Apr 2019 11:39:15am
    Author:  Silvio

  ==============================================================================
*/

#ifndef SYNTHVOICE_H_INCLUDED
#define SYNTHVOICE_H_INCLUDED
#include "Osc.h"
#include "ADSREnv.h"


//===============================================================================
/* Demo synth sound */
class SynthSound : public SynthesiserSound
{
public:

	bool appliesToNote(int /*midiNoteNumber*/) override { return true; }
	bool appliesToChannel(int /*midiChannel*/) override { return true;  }

};

//===============================================================================
/* Synth voice that plays a sine wave */
class SynthVoice : public SynthesiserVoice
{
public:
	SynthVoice()
	{
		samplingRate = getSampleRate();
		osc1.setSampleRate(samplingRate);
		ampEnv.setSampleRate(samplingRate);
		
	}
	
	bool canPlaySound(SynthesiserSound* sound) override
	{
	return dynamic_cast<SynthSound*> (sound) != nullptr;
	}

	void startNote(int midiNoteNumber, float velocity, SynthesiserSound *sound, int currentPitchWheelPosition) override
	{
		DBG("startNote");

		currentAngle = 0.0;
		level = velocity * 0.15;
		tailOff = 0.0;


		//auto cyclesPerSecond = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
		//auto cyclesPerSample = cyclesPerSecond / getSampleRate();
		
		 
		const double noteFreqHz = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
		osc1.setFrequency(noteFreqHz);
		
		DBG(midiNoteNumber);

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
		const int _CC_ = controllerNumber;
		const int _CV_ = newControllerValue;

		switch (_CC_) {

				//Waveforms
			case 100:
				osc1Wave = _CV_;
				break;

			case 101:
				osc2Wave = _CV_;
				break;
				

		

		}
	}

	void renderNextBlock(AudioBuffer<float>&outputBuffer, int startSample, int numSamples) override
	{

			if (osc1.getFrequency() != 0.0)
			{
				while (--numSamples >= 0) {

					osc1.setWave(osc1Wave);
					osc2.setWave(osc2Wave);
					const double oscOut = (osc1.getSignal() + osc2.getSignal()) / 2.;

					const double currentSample = (double) (oscOut * level * ampEnv.getCurve());
					
					for (int i = outputBuffer.getNumChannels(); --i >= 0;)
						

						outputBuffer.addSample(i, startSample, currentSample);

						++startSample;
					

					if (ampEnv.envStatus() == false) {

						clearCurrentNote();
						osc1.reset();
						break;

					}
				}

			}
		

		
	}



private:
	
	double currentAngle = 0.0;
	double angleDelta = 0.0;
	double level;
	double samplingRate;
	double tailOff = 0.0;
	const double voiceAttenuator = 0.6;
	int osc1Wave = 1;
	int osc2Wave = 1;

	Osc osc1, osc2;
	ADSRenv ampEnv;

};



#endif //SYNTHVOICE_H_INCLUDED