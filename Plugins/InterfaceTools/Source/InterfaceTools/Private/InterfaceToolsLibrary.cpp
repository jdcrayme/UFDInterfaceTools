// Fill out your copyright notice in the Description page of Project Settings.


#include "InterfaceToolsLibrary.h"

float UInterfaceToolsLibrary::NiceNum(float x) {
	float exp = floor(log10(x));
	float f = x / pow(10, exp); // Fractional part
	float tick_frac;

	if (f < 1.5) tick_frac = 1.0;
	else if (f < 3.0) tick_frac = 2.0;
	else if (f < 7.0) tick_frac = 5.0;
	else tick_frac = 10.0;

	return tick_frac * pow(10, exp);
}

void UInterfaceToolsLibrary::GenerateNiceAxisSteps(float min_val, float max_val, int num_ticks_approx, 
						   float& nice_min, float& nice_max, float& tick_spacing) {
	if (min_val > max_val) std::swap(min_val, max_val);
	float range = max_val - min_val;
	if (range == 0) {
		// Handle the case where all data points are the same
		range = std::abs(min_val) * 0.1; // Expand range slightly
		if (range == 0) range = 1.0; // Default range if min_val is 0
	}

	float step = range / num_ticks_approx;
	tick_spacing = NiceNum(step);

	// Adjust the nice minimum and maximum to be multiples of the tick spacing
	nice_min = floor(min_val / tick_spacing) * tick_spacing;
	nice_max = ceil(max_val / tick_spacing) * tick_spacing;

	// Recalculate if the new range is too small
	if (nice_max - nice_min < range) {
		nice_max = ceil(max_val / tick_spacing) * tick_spacing;
	}
}