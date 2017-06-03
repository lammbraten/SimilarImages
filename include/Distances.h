#pragma once

#include "Histogramm.h"

class Distances {

public:
	static double L1_norm(Histogramm &h1, Histogramm &h2);
	static double L2_norm(Histogramm &h1, Histogramm &h2);
	static double Hamming_Distance(Histogramm &h1, Histogramm &h2, double threshold);

private:
	Distances() {}

};
