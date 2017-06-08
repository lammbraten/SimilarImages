#include "../include/Tamura.h"


Tamura::Tamura(int k_min, int k_max) {
	this->k_min = k_min;
	this->k_max = k_max;


	for (int k = k_min; k < k_max; k++) {
		mean_gray_vals.push_back(calc_mean_gray_val(k));
		double *mean_gray_val_diff_h = new double[hue_image.rows * hue_image.cols];
		double *mean_gray_val_diff_v = new double[hue_image.rows * hue_image.cols];

		gray_val_differences(k, mean_gray_vals.back(), mean_gray_val_diff_h, mean_gray_val_diff_v);

		gray_val_differences_h.push_back(mean_gray_val_diff_h);
		gray_val_differences_v.push_back(mean_gray_val_diff_v);
	}
}

Tamura::~Tamura() {

}


double *Tamura::calc_mean_gray_val(int k){
	double *mean_gray_val_mat = new double[hue_image.rows * hue_image.cols];

	int k_pow = pow(2, k - 1);

	for (int x = k_pow; x < (hue_image.cols - k_pow); x++) 
		for (int y = k_pow; y < (hue_image.rows - k_pow); y++) 
			for (int i = x - k_pow; i < x + k_pow; i++) 
				for (int j = y - k_pow; j < y + k_pow; j++) 
					mean_gray_val_mat[x + (y * hue_image.cols)] = (double) hue_image.at<int>(y, x) / pow(2, 2 * k);

	return mean_gray_val_mat;
}

void Tamura::gray_val_differences(int k, double *A_k, double *E_kh, double *E_kv) {
	int k_pow = pow(2, k - 1);

	for (int x = k_pow; x < (hue_image.cols - k_pow); x++)
		for (int y = k_pow; y < (hue_image.rows - k_pow); y++) {
			E_kh[x + (y * hue_image.cols)] = abs(
				A_k[(x - k_pow) + (y * hue_image.cols)] 
				- A_k[(x + k_pow) + (y * hue_image.cols)]);

			E_kv[x + (y * hue_image.cols)] = abs(
				A_k[x + ((y - k_pow) * hue_image.cols)]
				- A_k[x + ((y + k_pow) * hue_image.cols)]);
		}
}
