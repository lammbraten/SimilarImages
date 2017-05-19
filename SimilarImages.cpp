#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;



int main(){
	Mat img = imread("data\\coffee1.jpg");
	if (img.empty())
		return -1;

	imshow("test", img);

	waitKey(0);
    return 0;
}

