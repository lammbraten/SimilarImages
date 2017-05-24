#include "../include/Histogramm.h"



using namespace std;

Histogramm::Histogramm(Mat img){
	Mat planes[3];
	split(img, planes);
	this->h_image = planes[0];
	this->s_image = planes[1];
	this->v_image = planes[2];

	h_vals = set_val_for_Channel(h_image);
	s_vals =  set_val_for_Channel(s_image);
	v_vals = set_val_for_Channel(v_image);
	

}

void Histogramm::print_histogram(){
	cout << "N#\t| Hue:\t| Sat:\t| Val" << endl;
	cout << "--------+-------+-------+-------" << endl;
	for (int i = 0; i < 256; i++)
		cout << i << "\t| " << (int)h_vals[i] << "\t| " << (int)s_vals[i] << "\t| " << (int)v_vals[i] << endl;
}




Histogramm::~Histogramm(){
	delete[] h_vals;
	delete[] h_vals;
	delete[] h_vals;
}

uchar* Histogramm::set_val_for_Channel(Mat channel) {
	uchar *vals = new uchar[256];
	for (int i = 0; i < 256; i++)
		vals[i] = 0;

	for (int r = 0; r < channel.rows; r++) {
		for (int c = 0; c < channel.cols; c++) {
			vals[channel.at<uchar>(Point(c,r))] += 1;
		}
	}
	return vals;
}

void Histogramm::print_Channel_Histogram(uchar * vals){
	for (int i = 0; i < 256; i++)
		cout << i << ") " << (int)vals[i] << endl;
}


