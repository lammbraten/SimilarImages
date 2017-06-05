#include "targetver.h"

#include <stdio.h>
#include <iomanip> 
#include <tchar.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#include "include\Histogramm.h"
#include "include\Distances.h"

using namespace std;
using namespace cv;


static string MENULINE = "====================";
vector<Histogramm *> histograms;

struct Result {
	string name;
	Mat img;
	double dst;
	int pos;
};


bool compareByDst(const Result &a, const Result &b){
	return a.dst < b.dst;
}

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
		size_t pos = fn[k].find("\\");
		string filename = fn[k].substr(pos);

		img.convertTo(img_hsv, CV_BGR2HSV);

		if (img_hsv.empty())
			continue; 
		cout << fn[k] << endl;
		data.push_back(new Histogramm(img_hsv, filename));
	}
	return data;
}

void showImages() {
	int i = 0;
	cout << "show Images" << endl;

	for (Histogramm *img : histograms) {
		imshow(img->getFilename(), img->getImage());
		cout << "Nr. " << ++i << ": \t" << img->getFilename() << endl;
	}

	waitKey(0);
}

void printHistogramms() {
	for (Histogramm *img : histograms) {
		img->print_histogram();
		system("pause");
	}
}

//TODO: calc all metrics and sort images

int printMenu() {
	int val;
	cout << "Menu:" << endl;
	cout << MENULINE << endl;
	cout << "(0) \t help" << endl;
	cout << "(1) \t show all images" << endl;
	cout << "(2) \t show all histogramms" << endl;
	cout << "(11) \t compare by L1-norm" << endl;
	cout << "(12) \t compare by L2-norm" << endl;
	cout << "(13) \t compare by Binary Set Hamming Distance" << endl;
	cout << "(14) \t compare with Crosstalk-matrix" << endl;
	cout << "(15) \t compare by average color" << endl;
	cout << "(16) \t compare by average variance" << endl;
	cout << "(17) \t compare by chi-square" << endl;
	cout << "(18) \t compare by JD" << endl;
	cout << "(100) \t end" << endl;
	cout << MENULINE << endl;

	cin >> val; 
	return val;
}

void printHelp() {
	cout << "Help for SimiliarImages!" << endl << endl; 
	cout << "Choose the wanted distance-function from the menu to calculate and show it by a reference image." << endl << endl;
	cout << "created by Marius Brueckner at hsnr from May-June 2017" << endl;
	cout << MENULINE << endl; 

	system("pause");
}

int getReferenceImageId() {
	int val;
	int i;
	do {
		i = 0;
		cout << "Please choose the reference image from the list below:" << endl;
		cout << MENULINE << endl;
		for (Histogramm *img : histograms)
			cout << "(" << i++ << ") \t" << img->getFilename() << endl;
		cout << MENULINE << endl;

		cin >> val;
	} while (val < 0 || val > i);
	return val;
}

double getThreshold() {
	double threshold;

	do {
		threshold = 0;
		cout << "Please enter a threshold between 0.0 and 1.0" << endl;

		cin >> threshold;
	} while (threshold < 0 || threshold > 1.0);


	return threshold;
}

void showResultImages(vector<Result> results) {
	Mat tmp;
	string windowname;

	int start_x = 600;
	int start_y = 200;
	int size = 200;
	int row_size = 5;
	int act_x = start_x;
	int act_y = start_y;
	int i = 0;
	cout << "Result Images:" << endl;
	for (Result r : results) {

		windowname = to_string(r.pos) + " " + r.name;
		resize(r.img, tmp, Size(size, size));
		imshow(windowname, tmp);

		moveWindow(windowname, act_x, act_y);

		i++;
		if (i % row_size == 0) {
			act_x = start_x;
			act_y += size;
		} else {
			act_x += size;
		}

		cout << "Nr " << r.pos << ": \t" << setw(40) << r.name << "\t dist: " << r.dst << endl;
	}
	waitKey(0);
}

void showOnlyResultList(vector<Result> results) {
	cout << "Result List:" << endl;
	for (Result r : results) 
		cout << "Nr " << r.pos << ": \t" << setw(40) << r.name << "\t dist: "<< r.dst << endl;

	system("pause");
}

vector<Result> getTopTenResults(vector<Result> results) {
	vector<Result> answer;
	sort(results.begin(), results.end(), compareByDst);

	for (int i = 0; i <= 10; i++) {
		if (i >= results.size())
			break;
		Result r = results.at(i);
		r.pos = i;
		answer.push_back(r);
	}

	return answer;
}

void showResults(vector<Result> results) {
	int val;
	cout << "show only list, or show list and images?" << endl;
	cout << MENULINE << endl;
	cout << "(0) only list" << endl;
	cout << "(1) list with images" << endl;
	cout << MENULINE << endl;
	
	cin >> val;

	if (val == 0)
		showOnlyResultList(results);
	else
		showResultImages(results);

}



vector<Result> calc_default(double (func)(Histogramm&, Histogramm&)) {
	int id = getReferenceImageId();

	vector<Result> results;

	for (Histogramm *h : histograms) {
		Result r;
		r.name = h->getFilename();
		r.img = h->getImage();
		r.dst = func(*histograms.at(id), *h);
		r.pos = -1;

		results.push_back(r);
	}

	return results;
}

vector<Result> calc_hamming() {
	int id = getReferenceImageId();
	double threshold = getThreshold();
	vector<Result> results;

	for (Histogramm *h : histograms) {
		Result r;
		r.name = h->getFilename();
		r.img = h->getImage();
		r.dst = Distances::Hamming_Distance(*histograms.at(id), *h, threshold);
		r.pos = -1;

		results.push_back(r);
	}

	return results;
}

vector<Result> calc_crosstalk() {
	int id = getReferenceImageId();
	double *ct_mat = Distances::calc_crosstalk_matrix();
	double d_max = Distances::calc_d_max(ct_mat);
	vector<Result> results;

	for (Histogramm *h : histograms) {
		Result r;
		r.name = h->getFilename();
		r.img = h->getImage();
		r.dst = Distances::calc_dist_from_ct_mat(*histograms.at(id), *h, ct_mat, d_max);
		r.pos = -1;

		results.push_back(r);
	}

	return results;
}

int main() {
	int val;
	int end = 0;
	histograms = readImages("data");

	while (!end) {
		 val = printMenu();

		switch (val){	
		case 0: 
			printHelp();
			break;
		case 1:
			showImages();
			break;
		case 2:
			printHistogramms();
			break;
		case 11:
			showResults(getTopTenResults(calc_default(Distances::L1_norm)));
			break;
		case 12:
			showResults(getTopTenResults(calc_default(Distances::L2_norm)));
			break;
		case 13:
			showResults(getTopTenResults(calc_hamming()));
			break;
		case 14:
			showResults(getTopTenResults(calc_crosstalk()));
			break;
		case 15:
			showResults(getTopTenResults(calc_default(Distances::avg_color_dist)));
			break;
		case 16:
			showResults(getTopTenResults(calc_default(Distances::avg_color_var)));
			break;
		case 17:
			showResults(getTopTenResults(calc_default(Distances::chi_sqaured)));
			break;
		case 18:
			showResults(getTopTenResults(calc_default(Distances::jeffrey_divergence)));
			break;
		case 100:
			end = 1;
			break;
		default:
			break;
		}
	}
    return 0;
}

