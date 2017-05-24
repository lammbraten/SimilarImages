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
		Mat im = imread(fn[k]);
		im.convertTo(im, CV_BGR2HSV);
			
		if (im.empty()) 
			continue; 
		data.push_back(im);
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
	vector<Mat> images = readImages("data2");

	showImages(images);

	Histogramm *h = new Histogramm(images.at(0));

	h->print_histogram();

	waitKey(0);
    return 0;
}

