///////////////////////////////////////////////////////////////
//
//  Speaker.cpp
//
//  Simple class for controlling a speaker using one pin (+ ground) or two pins
//  BobaBlox Library
//
//  The MIT License (MIT)
//  
//  Copyright (c) 2014-2015 Robert Gallup
//  
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//  
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//  
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//
///////////////////////////////////////////////////////////////

#include "Speaker.h"

const unsigned long MICROS_PER_SECOND = 1000000;

const byte POSITIVE_HALFCYCLE = 0;
const byte NEGATIVE_HALFCYCLE = 1;
	
#define MODE_FROM_HALFCYCLE(h) ((h==POSITIVE_HALFCYCLE)?HIGH:LOW)

// Constructors
Speaker::Speaker (byte pin)
{
  _pin1 = pin;
  pinMode (_pin1, OUTPUT);
  _soundONePin = true;
}

Speaker::Speaker (byte pin1, byte pin2)
{
  _pin1 = pin1;
  pinMode (_pin1, OUTPUT);

  _pin2 = pin2;
  pinMode (_pin2, OUTPUT);

  _soundONePin = false;

  _frequency = 1000;
  _color = .5;
  _calculateWave ();

}

// Beep. Blocks the thread.
void Speaker::beep (unsigned int freq, unsigned int length, float color)
{
	var unsigned long _beepTimer;

	// If frequency or pulse width have changed, calculate wave elements
	color = color % 1;
	if ((_frequency != freq) || (_color != (color))) {
		_color = color;
		_frequency = freq;
		_calculateWave()
	}

	// For length of beep
	beepTimer = millis();
	while ((millis() - _beepTimer) < length) {
		
		// First part of the cycle
		_cycleTimer = micros();
		_cycleSpeaker(POSITIVE_HALFCYCLE);
		while ((micros() - cycleTimer) < _cycleLength[POSITIVE_HALFCYCLE]);

		// Second part of the cycle
		_cycleTimer = micros();
		_cycleSpeaker(NEGATIVE_HALFCYCLE);
		while ((micros() - cycleTimer) < _cycleLength[NEGATIVE_HALFCYCLE]);

	}

}

// Tone. Non-blocking
void Speaker::tone (unsigned int freq, float color)
{
	// If frequency or pulse width have changed, calculate wave elements
	color = color % 1;
	if ((_frequency != freq) || (_color != (color))) {
		_color = color;
		_frequency = freq;
		_calculateWave();
		_soundON = true;
	}

	// If the sound isn't on, we're done
	if (!_soundON) return;

	// Now, to play the tone, check the timer to see if it's time to cycle
	// If so, cycle the speaker and reset the timer to the other _halfCycle
	if ((micros() - _cycleTimer) > _cycleLengh[_halfCycle]) {
		_halfCycle = 1-halfCycle;
		_cycleSpeaker(_halfCycle);
		_cycleTimer = micros();
	}
	
}

// Start tone
void Speaker::start() {
	_soundON = true;
	_cycleTimer = micros();
}

// Stop tone
void Speaker::stop() {
	_soundON = false;
}

// Return current frequency
void Speaker::frequency() {
	return (_frequency);
}

// Return current tone color (pulse width)
void Speaker::color() {
	return (_color);
}

// Private method to calculate wave components
void Speaker::_calculateWave () {

	var cycleLengthTotal = (1.0 / _frequency) * MICROS_PER_SECOND;
	_cycleLength[POSITIVE_HALFCYCLE] = _color * cycleLengthTotal;
	_cycleLength[NEGATIVE_HALFCYCLE] = cycleLengthTotal - _cycleLength[POSITIVE_HALFCYCLE;

}

// Private method to push speaker forward
void Speaker::_cycleSpeaker(byte halfCycle) {

	digitalWrite(_pin1, MODE_FROM_HALFCYCLE(halfCycle));
	if (!_onePin) digitalWrite (!_pin2, MODE_FROM_HALFCYCLE(1-halfCycle));

}
