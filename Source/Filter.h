/*
  ==============================================================================

    Filter.h
    Created: 28 Apr 2019 12:05:50am
    Author:  Silvio

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "maximilian.h"

//==============================================================================
/*
*/
class Filter  : public Component,
				public IIRFilter
				
{
public:
    Filter();
    ~Filter();

    void paint (Graphics&) override;
    void resized() override;
	void setSampleRate(double _sampleRate);
	IIRCoefficients setFilterType(int filterChoice, double cutOff, double res);



private:

	double sampleRate;
	Slider cutoffFreq, resonance;
	IIRCoefficients coefficients;
	

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Filter)
};
