#pragma once

#include "Histogramm.h"

class Distances {

public:
	static int L1_norm(Histogramm h1, Histogramm h2);
	static int L2_norm(Histogramm h1, Histogramm h2);

private:
	Distances() {}

};
