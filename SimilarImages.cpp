#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "include\Histogramm.h"

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

vector<Mat> readImages(const char *folder) {

	vector<String> fn;
	vector<Mat> data;
	glob(folder, fn, true);
	for (size_t k = 0; k<fn.size(); ++k){
		Mat img = imread(fn[k]);
		Mat img_hsv;
		img.convertTo(img_hsv, CV_BGR2HSV);

		if (img_hsv.empty())
			continue; 
		data.push_back(img_hsv);
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
	vector<Mat> images = readImages("data");

	//showImages(images);

	vector<Histogramm*> histograms;
	for (Mat image : images)
		histograms.push_back(new Histogramm(image));
	//Histogramm *h = new Histogramm(images.at(0));
	for(Histogramm *h : histograms)
		h->print_histogram(10);
	/*waitKey(0);
	histograms.at(1)->print_histogram(10);
	waitKey(0);
	histograms.at(2)->print_histogram(10);
	waitKey(0);
	histograms.at(3)->print_histogram(10);*/
//	h->print_histogram();

	waitKey(0);
    return 0;
}

