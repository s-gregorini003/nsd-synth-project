/*
  ==============================================================================

    oscSQU.h
    Created: 9 Apr 2019 12:39:20pm
    Author:  Silvio

  ==============================================================================
*/


#ifndef OSC_H_INCLUDED
#define OSC_H_INCLUDED
#include "maximilian.h"

class Osc
{
public:
	Osc() {
		oscFreq = 100.;
		sampleRate = 44100.;
		signal = myOsc.sinewave(oscFreq);
	}

	~Osc() {
	}

	void setFrequency(double _frequency)
	{
		oscFreq = _frequency;
	}

	void setSampleRate(double _sampleRate)
	{
		sampleRate = _sampleRate;
	}

	double getFrequency()
	{
		return oscFreq;
	}

	void setWave(int wave) 
	{
		/*
		wave:

		1 = Square Wave
		2 = Sine Wave
		3 = Saw Wave
		*/
		if (wave == 1) {
			signal = myOsc.square(oscFreq);
		}

		if (wave == 2) {
			signal = myOsc.sinewave(oscFreq);
		}
	}

	double getSignal()
	{		
		return signal;
	}

	void reset() 
	{
		oscFreq = 0.0;
	}

private:

	maxiOsc myOsc;
	double oscFreq;
	double sampleRate;
	double signal;
};

#endif //OSC_H_INCLUDED