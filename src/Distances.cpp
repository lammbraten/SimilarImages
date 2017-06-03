#include "../include/Distances.h"

/*
*	city bloc distance
*/
double Distances::L1_norm(Histogramm &h1, Histogramm &h2) {
	double result = 0;
	
	for (int i = 0; i < h1.size(); i++)
		result += abs(h1.getBins()[i] - h2.getBins()[i]);

	return result;
}


double Distances::L2_norm(Histogramm &h1, Histogramm &h2) {
	double result = 0;

	for (int i = 0; i < h1.size(); ++i) 
		result += pow(abs(h1.getBins()[i] - h2.getBins()[i]), 2);

	return sqrt(result);
}