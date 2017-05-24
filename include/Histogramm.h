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
	Mat h_image;
	Mat s_image;
	Mat v_image;

	uchar *h_vals;
	uchar *s_vals;
	uchar *v_vals;

	uchar *set_val_for_Channel(Mat channel);
	void print_Channel_Histogram(uchar *vals);

};

