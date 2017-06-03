#include "../include/Distances.h"

/*
*	city block distance
*/
double Distances::L1_norm(Histogramm &h1, Histogramm &h2) {
	double result = 0;
	
	for (int i = 0; i < h1.size(); i++)
		result += abs(h1.getBins()[i] - h2.getBins()[i]);

	return result;
}

/* 
* Euklid
*/
double Distances::L2_norm(Histogramm &h1, Histogramm &h2) {
	double result = 0;

	for (int i = 0; i < h1.size(); ++i) 
		result += pow(abs(h1.getBins()[i] - h2.getBins()[i]), 2);

	return sqrt(result);
}

int calcHits(Histogramm h, double threshold) {
	int hits = 0;

	for (int i = 0; i < h.size(); ++i) {
		if (h.getBins()[i] > threshold) {
			hits++;
		}
	}



	return hits;
}

double Distances::Hamming_Distance(Histogramm &h1, Histogramm &h2, double threshold){
	int h1_hits = calcHits(h1, threshold);
	int h2_hits = calcHits(h2, threshold);

	return (double)((double)abs(h1_hits - h2_hits) / (double)(abs(h1_hits) * abs(h2_hits)));
}
