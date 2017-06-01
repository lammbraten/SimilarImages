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
	void print_histogram(int rows);
	~Histogramm();
	

private:
	const int HUE_BINS = 16;
	const int SAT_BINS = 3;
	const int VAL_BINS = 3;
	const int MAX_BINS = HUE_BINS*SAT_BINS*VAL_BINS;


	struct bin {
		int h_val;
		int s_val;
		int v_val;
	};

	Mat *src;
	Mat h_image;
	Mat s_image;
	Mat v_image;

	int *bins;
	int *h_vals;
	int *s_vals;
	int *v_vals;

	int *set_val_for_Channel(Mat channel, int bins);
	int calc_bin_position(uchar val, int max_val, int max_bin);
	void print_Channel_Histogram(uchar *vals, int bins);
	void print_histogramm_row(int id, int h, int s, int v);
	int get_bin_number_of(int h, int s, int v);

	void calc_bins();
};

