/*
  ==============================================================================

    Filter.cpp
    Created: 28 Apr 2019 12:05:50am
    Author:  Silvio

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Filter.h"

//==============================================================================
Filter::Filter()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
	
	

	//filterSelection = new AudioProcessorValueTreeState::ComboBoxAttachment(processor.valueTree, "filtertype", filterTypeMenu);
}

Filter::~Filter()
{
}

void Filter::paint (Graphics& g)
{
  

}

void Filter::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void Filter::setSampleRate(double _sampleRate)
{
	sampleRate = _sampleRate;
}

IIRCoefficients Filter::setFilterType(int filterChoice, double cutOff, double res)
{
	if (filterChoice == 1) {

		coefficients = IIRCoefficients::makeLowPass(sampleRate, 22000., 0.1);
		
	}
 

	if (filterChoice == 2) {

		coefficients = IIRCoefficients::makeLowPass(sampleRate, cutOff, res);
		//DBG("is working");
	}

	if (filterChoice == 3) {


		coefficients = IIRCoefficients::makeHighPass(sampleRate, cutOff, res);
	}

	if (filterChoice == 4) {
		

		coefficients = IIRCoefficients::makeBandPass(sampleRate, cutOff, res);
	}


	return coefficients;
}

//==============================================================================





//==============================================================================


//==============================================================================
