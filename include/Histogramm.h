#pragma once

#include <iostream>
#include <iomanip> 
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

class Histogramm {
public:
	static const int HUE_BINS = 16;
	static const int SAT_BINS = 3;
	static const int VAL_BINS = 3;
	static const int MAX_BINS = HUE_BINS*SAT_BINS*VAL_BINS;

	static const int HUE_RANGE = 360;
	static const int SAT_RANGE = 1;
	static const int VAL_RANGE = 1;

	Histogramm();
	Histogramm(Mat img, string filename);
	void print_histogram();
	void print_histogram(int rows);
	double *getBins();
	int size();
	static double getHueCentroidFromBin(int bin);
	static double getSatCentroidFromBin(int bin);
	static double getValCentroidFromBin(int bin);
	static double calc_dCyl(int h_x, int h_y, int s_x, int s_y, int v_x, int v_y);
	string getFilename();
	~Histogramm();
	

private:
	int bin_value_size = 0;

	Mat *src;
	Mat h_image;
	Mat s_image;
	Mat v_image;

	double *bins;
	string filename;

	int *set_val_for_Channel(Mat channel, int bins);
	int calc_bin_position(uchar val, int max_val, int max_bin);
	void print_Channel_Histogram(uchar *vals, int bins);
	void print_histogramm_row(int id, int h, int s, int v);
	int get_bin_number_of(int h, int s, int v);

	void calc_bins();
	void normalize_bins();
};

