#include "../include/Tamura.h"


Tamura::Tamura(int k_min, int k_max, Mat hue_image) {
	this->k_min = k_min;
	this->k_max = k_max;
	this->hue_image = hue_image;

	for (int k = k_min; k < k_max; k++) {
		mean_gray_vals.push_back(calc_mean_gray_val(k));
		double *mean_gray_val_diff_h = new double[hue_image.rows * hue_image.cols];
		double *mean_gray_val_diff_v = new double[hue_image.rows * hue_image.cols];

		gray_val_differences(k, mean_gray_vals.back(), mean_gray_val_diff_h, mean_gray_val_diff_v);

		gray_val_differences_h.push_back(mean_gray_val_diff_h);
		gray_val_differences_v.push_back(mean_gray_val_diff_v);
	}
}

double Tamura::calc_Sbest() {
	double result = 0;
	double *s_best = new double[hue_image.rows * hue_image.cols];
	int w = hue_image.cols;
	int h = hue_image.rows;

	int best_k = k_min;
	for (int x = 0; x < hue_image.cols; ++x) {
		for (int y = 0; y < hue_image.rows; ++y) {
			for (int k = k_min; k < k_max; ++k) {
				if(max(gray_val_differences_h.at(best_k - k_min)[x + y * hue_image.cols], gray_val_differences_v.at(best_k - k_min)[x + y * hue_image.cols]) 
					< max(gray_val_differences_h.at(k - k_min)[x + y * hue_image.cols], gray_val_differences_v.at(k - k_min)[x + y * hue_image.cols]))
					best_k = k;
			}
			s_best[x + y * hue_image.cols] = pow(2.0, (double)best_k);
			best_k = k_min;
		}
	}

	for (int i = 0; i < w - 1; ++i) 
		for (int j = 0; j < h - 1; ++j) 
			result += s_best[i + j * hue_image.cols];

	this->sBest = result / (w*h);
	return sBest;
}


double *Tamura::calc_mean_gray_val(int k) {
	double *mean_gray_val_mat = new double[hue_image.rows * hue_image.cols];

	int k_pow = pow(2, k - 1);

	for (int x = k_pow; x < (hue_image.cols - k_pow); x++) 
		for (int y = k_pow; y < (hue_image.rows - k_pow); y++) {
			mean_gray_val_mat[x + (y * hue_image.cols)]= 0;
			for (int i = x - k_pow; i < x + k_pow; i++)
				for (int j = y - k_pow; j < y + k_pow; j++)
					mean_gray_val_mat[x + (y * hue_image.cols)] += (double)hue_image.at<uchar>(i, j) / pow(2, 2 * k);
		}
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

Mat3b Tamura::calcHist() {
	int bins = 256;
	int histSize[] = {bins};
	float lranges[] = {0, 256};
	const float* ranges[] = {lranges};
	Mat hist;
	int channels[] = {0};
	cv::calcHist(&hue_image, 1, channels, Mat(), hist, 1, histSize, ranges, true, false);

	int const hist_height = 256;
	Mat3b hist_image = Mat3b::zeros(hist_height, bins);
	Mat hist_norm;
	normalize(hist, hist_norm, 0, 1, NORM_MINMAX, -1, Mat());

	double max_val = 0;
	minMaxLoc(hist_norm, 0, &max_val);

	for (int b = 0; b < bins; b++) {
		float const binVal = hist_norm.at<float>(b);
		int const height = cvRound((4 * binVal)*hist_height / max_val);
		line(hist_image, Point(b, hist_height - height), Point(b, hist_height), Scalar::all(255));
	}

	return hist_image;
}

double Tamura::calcDist(Tamura other) {
	cout << "F_crs: " << Vec2d(this->sBest, other.sBest) << endl;
	return fabs(this->sBest - other.sBest);


}
