#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

class Tamura {
public:
	Tamura(int k_min, int k_max, Mat hue_image);
	double calc_Sbest();

private:
	int k_min, k_max;
	vector<double*> mean_gray_vals;
	vector<double*> gray_val_differences_h;
	vector<double*> gray_val_differences_v;
	Mat hue_image;

	static const int HORIZONTAL = 1;
	static const int VERTICAL = -1;

	double *calc_mean_gray_val(int k);
	void gray_val_differences(int k, double *A_k, double *E_kh, double *E_kv);

};

