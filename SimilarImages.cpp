#include "targetver.h"

#include <stdio.h>
#include <iomanip> 
#include <tchar.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "include\Histogramm.h"
#include "include\Distances.h"
#include "SimilarImages.h"

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


bool compareByDst(const Result &a, const Result &b)
{
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
		img.convertTo(img_hsv, CV_BGR2HSV);

		if (img_hsv.empty())
			continue; 
		cout << fn[k] << endl;
		data.push_back(new Histogramm(img_hsv, fn[k]));
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
	cout << "(14) \t compare with Crosstal-matrix" << endl;
	cout << "(15) \t compare by average color" << endl;
	cout << "(16) \t compare by average variance" << endl;
	cout << "(17) \t compare by chi-square" << endl;
	cout << "(18) \t compare by JD" << endl;
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

void showResultImages(vector<Result> results) {
	cout << "Result Images:" << endl;
	for (Result r : results) {
		imshow(r.name, r.img);
		cout << "Nr. " << r.pos << ": \t" << r.name << "\t\tdist: " << r.dst << endl;
	}
	waitKey(0);
}

void showOnlyResultList(vector<Result> results) {
	cout << "Result List:" << endl;
	for (Result r : results) 
		cout << "Nr. " << r.pos << ": \t" << r.name << "\t\tdist: "<< r.dst<< endl;

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


vector<Result> calcL1() {
	int id = getReferenceImageId();

	vector<Result> results;

	for (Histogramm *h : histograms) {
		Result r;
		r.name = h->getFilename();
		r.img = h->getImage();
		r.dst = Distances::L1_norm(*h, *histograms.at(id));
		r.pos = -1;

		results.push_back(r);
	}

	return results;
}

int main() {
	int val;
	histograms = readImages("data");

	while (true) {
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
			showResultImages(getTopTenResults(calcL1()));

			break;
		case 12:
			showImages();
			break;
		case 13:
			showImages();
			break;
		default:
			break;
		}
	}



	double dist;
	double *ct_mat = Distances::calc_crosstalk_matrix();
	double d_max = Distances::calc_d_max(ct_mat);


	for(Histogramm *h : histograms)
	//	cout << "Dist "<< h->getFilename() << ": " << Distances::Hamming_Distance(*histograms.at(0), *h, 0.01) << endl;
	//	cout << "Dist " << h->getFilename() << ": " << Distances::calc_dist_from_ct_mat(*histograms.at(0), *h, ct_mat, d_max) << endl;
		cout << "Dist " << h->getFilename() << ": " << Distances::chi_sqaured(*histograms.at(0), *h) << endl;

	//	cout << "Dist " << h->getFilename() << ": " << Distances::jeffrey_divergence(*histograms.at(0), *h) << endl;


	waitKey(0);
	waitKey(0);

    return 0;
}

