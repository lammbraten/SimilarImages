#pragma once

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

class Histogramm {
public:
	Histogramm(Mat img);
	void print_histogram();
	~Histogramm();

private:
	const int HUE_BINS = 16;
	const int SAT_BINS = 3;
	const int VAL_BINS = 3;

	Mat *src;
	Mat h_image;
	Mat s_image;
	Mat v_image;

	int *h_vals;
	int *s_vals;
	int *v_vals;

	int *set_val_for_Channel(Mat channel, int bins);
	int calc_bin_position(int val, int max_val, int max_bin);
	void print_Channel_Histogram(uchar *vals, int bins);
	void print_histogramm_row(int id, int h, int s, int v);

};

