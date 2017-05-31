#include "../include/Histogramm.h"



using namespace std;

Histogramm::Histogramm(Mat img){
	this->src = &img;

	Mat planes[3];
	split(img, planes);
	this->h_image = planes[0];
	this->s_image = planes[1];
	this->v_image = planes[2];

	h_vals = set_val_for_Channel(h_image, HUE_BINS);
	s_vals =  set_val_for_Channel(s_image, SAT_BINS);
	v_vals = set_val_for_Channel(v_image, VAL_BINS);
	

}

void Histogramm::print_histogram(){
	cout << "N#\t| Hue:\t| Sat:\t| Val" << endl;
	cout << "--------+-------+-------+-------" << endl;
	int maxBins = max(HUE_BINS, max(SAT_BINS, VAL_BINS));
	int h_val, s_val, v_val;
	for (int i = 0; i < maxBins; i++) {
		if (i < HUE_BINS)
			h_val = (int)h_vals[i];
		else
			h_val = -1;
		if (i < SAT_BINS)
			s_val = (int)s_vals[i];
		else
			s_val = -1;
		if (i < VAL_BINS)
			v_val = (int)v_vals[i];
		else
			v_val = -1;
		print_histogramm_row(i, h_val, s_val, v_val);
	}
}

void Histogramm::print_histogramm_row(int id, int h, int s, int v) {
	string h_val = "-";
	string s_val = "-";
	string v_val = "-";
	if (h >= 0)
		h_val = to_string(h);
	if (s >= 0)
		s_val = to_string(s);
	if (v >= 0)
		v_val = to_string(v);


	cout << id << "\t| " << h_val << "\t| " << s_val << "\t| " << v_val << endl;

}




Histogramm::~Histogramm(){
	delete[] h_vals;
	delete[] h_vals;
	delete[] h_vals;
}

int Histogramm::calc_bin_position(int val, int max_val, int max_bin) {
	int bin_size = max_val / max_bin;
	int bin = val / bin_size;
	if(bin < max_bin)
		return bin;
	return bin - 1;
}

int* Histogramm::set_val_for_Channel(Mat channel, int bins) {
	int bin;
	int *vals = new int[bins];
	for (int i = 0; i < bins; i++)
		vals[i] = 0;

	for (int r = 0; r < channel.rows; r++) {
		for (int c = 0; c < channel.cols; c++) {
			bin = calc_bin_position(channel.at<uchar>(Point(c, r)), 255, bins);
			vals[bin] += 1;
		}
	}
	return vals;
}

void Histogramm::print_Channel_Histogram(uchar *vals, int bins){
	for (int i = 0; i < bins; i++)
		cout << i << ") " << (int)vals[i] << endl;
}


