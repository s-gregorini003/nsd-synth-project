/*
  ==============================================================================

    oscSQU.h
    Created: 9 Apr 2019 12:39:20pm
    Author:  Silvio

  ==============================================================================
*/


#ifndef SQUAREOSC_H_INCLUDED
#define SQUAREOSC_H_INCLUDED
#include "maximilian.h"

class SquareOsc 
{
public:
	SquareOsc() {
		squareFreq = 100.;
		sampleRate = 44100.;
	}

	~SquareOsc() {
	}

	void setFrequency(double _frequency)
	{
		squareFreq = _frequency;
	}

	void setSampleRate(double _sampleRate)
	{
		sampleRate = _sampleRate;
	}

	double getSignal()
	{
		double signal = myOsc.square(squareFreq);

		return signal;
	}

	double getFrequency()
	{
		return squareFreq;
	}

	void reset() 
	{
		squareFreq = 0.0;
	}

private:

	maxiOsc myOsc;
	double squareFreq;
	double sampleRate;
};

#endif //SQUAREOSC_H_INCLUDED