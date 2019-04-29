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
#include "Filter.h"



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
		osc2.setSampleRate(samplingRate);
		env1.setSampleRate(samplingRate);
		filt1.setSampleRate(samplingRate);

		filt1Cutoff = 400.;
		filt1Res = 0.1;
		lfoFreq = 10.;
		lfoDepth = 0.1;
	}
	
	bool canPlaySound(SynthesiserSound* sound) override
	{
	return dynamic_cast<SynthSound*> (sound) != nullptr;
	}

	void startNote(int midiNoteNumber, float velocity, SynthesiserSound *sound, int currentPitchWheelPosition) override
	{
		env1.noteOn();
		DBG("startNote");

		const double noteFreqHz = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
		osc1.setFrequency(noteFreqHz);
		osc2.setFrequency(noteFreqHz);

		DBG(midiNoteNumber);

		level = velocity * 0.15;
		

	}

	void stopNote(float velocity, bool allowTailOff) override
	{
			env1.noteOff();
	}

	void pitchWheelMoved(int /*newPitchWheelValue*/) override
	{

	}

	void getLFO(float *rate, float *depth, float *selection)
	{
		lfoDest = *selection;
		lfoFreq = *rate;
		lfoDepth = *depth;
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

				//Envelope Parameters
			case 102:
				env1Attack = _CV_;
				break;
				
			case 103:
				env1Decay = _CV_;
				break;

			case 104:
				env1Sustain = _CV_;
				break;

			case 105:
				env1Release = _CV_;
				break;

				//Filter Parameters
			case 106:
				filt1Type = _CV_;
				break;

			case 107:
				filt1Cutoff = _CV_;
				break;

			case 108:
				filt1Res = _CV_;
				break;

				//LFO Parameters
			case 109:
				lfoDest = _CV_;
				break;

			case 110:
				lfoFreq = _CV_;
				break;

			case 111:
				lfoDepth = _CV_;
				break;
		

		}
	}

	double getSound()
	{
		osc1.setWave(osc1Wave);
		osc2.setWave(osc2Wave);
		env1.setParameters({ env1Attack, env1Decay, env1Sustain, env1Release });
		
		/*
		LOG 29 Apr

			Lfo depth doesn't work between 0.1 and 0.9, don't know why.
			Problem solved setting the slider range from 1 to 10 and then dividing
			lfoDepth by 10 in the lfoFiltCutoff equation.

		*/

		if (lfoDest == 2) //LFO controlling amp
		{
			
			double wave1 = level * osc1.getSignal() * (lfo.coswave(lfoFreq) * lfoDepth + 1);
			double wave2 = level * osc1.getSignal() * (lfo.coswave(lfoFreq) * lfoDepth + 1);

			filt1.setCoefficients(filt1.setFilterType(filt1Type, filt1Cutoff, filt1Res));
			
			double filtered1 = filt1.processSingleSampleRaw(wave1);
			double filtered2 = filt1.processSingleSampleRaw(wave2);

			return ((env1.getNextSample()*(filtered1 + filtered2)) / 2);
		}

		else if (lfoDest == 3)  //LFO controlling cutoff
		{
			double wave1 = level * osc1.getSignal();
			double wave2 = level * osc2.getSignal();

			/*
			LOG 28 Apr
				Setting up the statements for lfo controlling amp, cutoff or nothing.
				The system returns an error if the frequency of the signal is <= 0.
				That happens when the depth of the lfo is significant and the note frequency is low.

				Here's the math behind the solution:

				-1.0 < lfo.coswave(lfoFreq) < 1.0

				-0.5 < lfo.coswave(lfoFreq) / 2 < 0.5

				-0.5 < (lfo.coswave(lfoFreq) / 2 ) * (lfoDepth / 10) < 0.5

																	[considering that
																	lfoDepth/10. is a value
																	between 0.1 and 1]

				0.5 <  (lfo.coswave(lfoFreq) / 2 ) * (lfoDepth / 10) + 1 < 1.5

				So, in idle status the frequency of the signal is multiplied by 1 and doesn't change.
				When the lfo works at full range, the frequency oscillates between its 0.5*freq and 1.5*freq.
			*/

			float lfoFiltCutoff = (lfo.coswave(lfoFreq) / 2. ) * (lfoDepth / 10.) + 1.; 


			filt1.setCoefficients(filt1.setFilterType(filt1Type, filt1Cutoff * lfoFiltCutoff, filt1Res));

			double filtered1 = filt1.processSingleSampleRaw(wave1);
			double filtered2 = filt1.processSingleSampleRaw(wave2);
		
			return ((env1.getNextSample()*(filtered1 + filtered2)) / 2);
		}

		else //LFO not controlling anything
		{
			double wave1 = level * osc1.getSignal();
			double wave2 = level * osc2.getSignal();

			filt1.setCoefficients(filt1.setFilterType(filt1Type, filt1Cutoff, filt1Res));

			double filtered1 = filt1.processSingleSampleRaw(wave1);
			double filtered2 = filt1.processSingleSampleRaw(wave2);

			return ((env1.getNextSample()*(filtered1 + filtered2)) / 2);
		}

	}

	void renderNextBlock(AudioBuffer<float>&outputBuffer, int startSample, int numSamples) override
	{
		if (osc1.getFrequency() != 0.0 && osc2.getFrequency() != 0.0)
		{
			while (--numSamples >= 0) {
				double sound = getSound();
				for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel) {
					outputBuffer.addSample(channel, startSample, sound);
				}
				++startSample;

			}
			
		}
		/*
			if (osc1.getFrequency() != 0.0 && osc2.getFrequency() != 0.0)
			{
					
				while (--numSamples >= 0) {

					osc1.setWave(osc1Wave);
					osc2.setWave(osc2Wave);
					env1.setParameters({env1Attack, env1Decay, env1Sustain, env1Release });
					
					filt1.setCoefficients(filt1.setFilterType(filt1Type, filt1Cutoff, filt1Res));

					//filt1.setCoefficients(IIRCoefficients::makeLowPass(44100, filt1Cutoff, filt1Res));
					const double oscOut = (osc1.getSignal() + osc2.getSignal()) / 2.;

					filtSample = filt1.processSingleSampleRaw(oscOut);
					envSample = env1.getNextSample() * filtSample *level;

					

					for (int i = outputBuffer.getNumChannels(); --i >= 0;)
							outputBuffer.addSample(i, startSample, envSample);

						++startSample;
					
						

					if (env1.isActive() == false) {

						clearCurrentNote();
						osc1.reset();
						osc2.reset();
						break;

					}
				}

			}
		*/
		
		
	}



private:
	
	
	double level;
	double samplingRate;
	
	double envSample, filtSample;
	int status;
	int osc1Wave = 1;
	int osc2Wave = 1;
	float env1Attack, env1Decay, env1Sustain, env1Release;
	int filt1Type;
	float filt1Cutoff, filt1Res;
	int lfoDest;
	float lfoDepth, lfoFreq;

	Osc osc1, osc2;
	maxiOsc lfo;
	ADSR env1;
	Filter filt1;
	


};



#endif //SYNTHVOICE_H_INCLUDED