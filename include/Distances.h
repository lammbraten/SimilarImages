#pragma once

#include "Histogramm.h"

class Distances {

public:
	static double L1_norm(Histogramm &h1, Histogramm &h2);
	static double L2_norm(Histogramm &h1, Histogramm &h2);
	static double Hamming_Distance(Histogramm &h1, Histogramm &h2, double threshold);
	static double *calc_crosstalk_matrix();
	static double calc_d_max(double *ct_mat);
	static double calc_dist_from_ct_mat(Histogramm &h1, Histogramm &h2, double *ct_mat, double d_max);
	static double avg_color_dist(Histogramm &h1, Histogramm &h2);
	static double avg_color_var(Histogramm &h1, Histogramm &h2);
	static double chi_sqaured(Histogramm &h1, Histogramm &h2);
	static double jeffrey_divergence(Histogramm &h1, Histogramm &h2);

private:
	Distances() {}

};
