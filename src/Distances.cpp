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

int calc_Hits(Histogramm h, double threshold) {
	int hits = 0;

	for (int i = 0; i < h.size(); ++i) {
		if (h.getBins()[i] > threshold) {
			hits++;
		}
	}



	return hits;
}

double Distances::Hamming_Distance(Histogramm &h1, Histogramm &h2, double threshold){
	int h1_hits = calc_Hits(h1, threshold);
	int h2_hits = calc_Hits(h2, threshold);

	return (double)((double)abs(h1_hits - h2_hits) / (double)(abs(h1_hits) * abs(h2_hits)));
}

double *Distances::calc_crosstalk_matrix() {
	double *ct_mat = new double[Histogramm::MAX_BINS * Histogramm::MAX_BINS];
	int i = 0;

	for (int h1 = 0; h1 < Histogramm::HUE_BINS; h1++) 
		for (int h2 = 0; h2 < Histogramm::HUE_BINS; h2++) 
			for (int s1 = 0; s1 < Histogramm::SAT_BINS; s1++) 
				for (int s2 = 0; s2 < Histogramm::SAT_BINS; s2++) 
					for (int v1 = 0; v1 < Histogramm::VAL_BINS; v1++) 
						for (int v2 = 0; v2 < Histogramm::VAL_BINS; v2++) 
							ct_mat[i++] = Histogramm::calc_dCyl(h1, h2, s1, s2, v1, v2);

	return ct_mat;
}

double Distances::calc_d_max(double *ct_mat) {
	//TODO: maybe integrate this to 'calc_crosstalk_matrix()'. 
	//This is only called once, so it isn't that bad.
	double d_max = 0;
	
	for(int i = 0; i < Histogramm::MAX_BINS * Histogramm::MAX_BINS; i++)
		d_max = max(d_max, ct_mat[i]);

	return d_max;
}

double Distances::calc_dist_from_ct_mat(Histogramm &h1, Histogramm &h2, double *ct_mat, double d_max){
	double result = 0;
	double a_ij;

	for (int i = 0; i < Histogramm::MAX_BINS; i++) {
		for (int j = 0; j < Histogramm::MAX_BINS; j++) {
			a_ij = 1 - (ct_mat[i * Histogramm::MAX_BINS + j] / d_max);
		//	cout << a_ij << endl;
			result += (abs(h1.getBins()[i] - h2.getBins()[i]) 
				* a_ij
				* abs(h1.getBins()[j] - h2.getBins()[j]));
		}
	}
	return sqrt(result);
}


