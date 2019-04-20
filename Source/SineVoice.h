/*
  ==============================================================================

    SineVoice.h
    Created: 18 Apr 2019 10:05:26pm
    Author:  Silvio

  ==============================================================================
*/


#ifndef SINEOSC_H_INCLUDED
#define SINEOSC_H_INCLUDED
#include "maximilian.h"

class SineOsc 
{
public:
	SineOsc() {
		sineFreq = 100.;
		sampleRate = 44100.;
	}

	~SineOsc() {
	}

	void setFrequency(double _frequency)
	{
		sineFreq = _frequency;
	}

	void setSampleRate(double _sampleRate)
	{
		sampleRate = _sampleRate;
	}

	double getSignal()
	{
		double signal = myOsc.sinewave(sineFreq);

		return signal;
	}

	double getFrequency()
	{
		return sineFreq;
	}

	void reset() 
	{
		sineFreq = 0.0;
	}

private:

	maxiOsc myOsc;
	double sineFreq;
	double sampleRate;
};

#endif //SINEOSC_H_INCLUDED