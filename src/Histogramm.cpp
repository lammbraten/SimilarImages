#include "../include/Histogramm.h"

using namespace std;

Histogramm::Histogramm(){
	cout << "Std-Constructor called" << endl;
}

Histogramm::Histogramm(Mat img, string filename){
	this->src = &img;

	Mat planes[3];
	split(img, planes);
	this->h_image = planes[0];
	this->s_image = planes[1];
	this->v_image = planes[2];
	this->filename = filename;

	calc_bins();

}

void Histogramm::normalize_bins() {
	for (int i = 0; i < MAX_BINS; i++)
		bins[i] = bins[i] / bin_value_size;
}

void Histogramm::calc_bins() {
	int h_bin, s_bin, v_bin, bin;
	bins = new double[MAX_BINS];

	for (int i = 0; i < MAX_BINS; i++)
		bins[i] = 0;

	for (int r = 0; r < src->rows; r++) {
		for (int c = 0; c < src->cols; c++) {
			//cout << (int) h_image.at<uchar>(Point(c, r)) << endl;
			h_bin = calc_bin_position(h_image.at<uchar>(Point(c, r)), 256, HUE_BINS);
			s_bin = calc_bin_position(s_image.at<uchar>(Point(c, r)), 256, SAT_BINS);
			v_bin = calc_bin_position(v_image.at<uchar>(Point(c, r)), 256, VAL_BINS);
			bin = get_bin_number_of(h_bin, s_bin, v_bin);
			//cout << bin << "\t|" << h_bin << "\t|" << s_bin << "\t|" << v_bin << "\t|" << bins[bin] << endl;

			bins[bin] += 1;
			bin_value_size++;
		}
	}

	normalize_bins();
}

int Histogramm::get_bin_number_of(int h, int s, int v){
	int h_row = h * SAT_BINS * VAL_BINS;
	int s_row = s * VAL_BINS;
	int v_row = v;

	return h_row + s_row + v_row;
}


void Histogramm::print_histogram(){
	cout << "N#\t| Hue:\t| Sat:\t| Val\t| number" << endl;
	cout << "--------+-------+-------+-------+--------" << endl;

	int i = 0;
	for (int h = 0; h < HUE_BINS; h++) {
		for (int s = 0; s < SAT_BINS; s++) {
			for (int v = 0; v < VAL_BINS; v++) {
				cout << i << "\t|" << h << "\t|" << s << "\t|" << v << "\t|"<< fixed << bins[i] << endl;

				i++;
			}
		}
	}
}

void Histogramm::print_histogram(int rows) {
	cout << "Values: " << bin_value_size << endl;
	cout << "N#\t| Hue:\t| Sat:\t| Val\t| number" << endl;
	cout << "--------+-------+-------+-------+--------" << endl;


	int i = 0;
	for (int h = 0; h < HUE_BINS; h++) {
			for (int s = 0; s < SAT_BINS; s++) {
				for (int v = 0; v < VAL_BINS; v++) {
					cout << i << "\t|" << h << "\t|" << s << "\t|" << v << "\t|"<< fixed << bins[i] << endl;

					i++;
					if (rows - i <= 0)
						return;
				}
			}
		}
}

double *Histogramm::getBins(){
	return this->bins;
}

int Histogramm::size() {
	return this->MAX_BINS;
}

string Histogramm::getFilename()
{
	return filename;
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

}

int Histogramm::calc_bin_position(uchar val, int max_val, int max_bin) {
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


