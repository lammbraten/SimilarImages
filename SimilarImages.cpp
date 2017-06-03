#include "targetver.h"

#include <stdio.h>
#include <iomanip> 
#include <tchar.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "include\Histogramm.h"
#include "include\Distances.h"

using namespace std;
using namespace cv;


int isDir(const char *path) {
	struct stat info;

	if (stat(path, &info) != 0)
		return 0;
	else if (info.st_mode & S_IFDIR)
		return 1;
	else
		return 0;
}

vector<Histogramm *> readImages(const char *folder) {
	vector<String> fn;
	vector<Histogramm *> data;

	glob(folder, fn, true);
	for (size_t k = 0; k<fn.size(); ++k){
		Mat img = imread(fn[k]);
		Mat img_hsv;
		img.convertTo(img_hsv, CV_BGR2HSV);

		if (img_hsv.empty())
			continue; 
		cout << fn[k] << endl;
		data.push_back(new Histogramm(img_hsv, fn[k]));
	}
	return data;
}

void showImages(vector<Mat> images) {
	int i = 1;
	string windowname = "Bild: ";
	for (Mat img : images) {
		string str = windowname + to_string(i);
		imshow(str, img);

		i++;
	}
}

int main() {
	vector<Histogramm *> histograms = readImages("data");

	//showImages(images);

	//for(Histogramm *h : histograms)
	//	h->print_histogram();

	double dist;
	double *ct_mat = Distances::calc_crosstalk_matrix();
	double d_max = Distances::calc_d_max(ct_mat);

	for(Histogramm *h : histograms)
	//	cout << "Dist "<< h->getFilename() << ": " << Distances::Hamming_Distance(*histograms.at(0), *h, 0.01) << endl;
		cout << "Dist " << h->getFilename() << ": " << Distances::calc_dist_from_ct_mat(*histograms.at(0), *h, ct_mat, d_max) << endl;


	waitKey(0);
	waitKey(0);

    return 0;
}

