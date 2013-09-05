#ifndef NUMBERPAIR_HPP
#define NUMBERPAIR_HPP

struct NumberPair
{
	float x;
	float y;

	NumberPair(float xIn, float yIn) : x(xIn), y (yIn) {}
};

bool compareNumberPair(const NumberPair *pair1, const NumberPair *pair2);

#endif