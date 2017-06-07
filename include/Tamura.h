#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

class Tamura {
public:
	Tamura(int k_min, int k_max);
	~Tamura();
	Mat calc_Sbest();

private:
	int k_min, k_max;
	double calc_Eh(int k, int x, int y);
	double calc_Ev(int k, int x, int y);
};

