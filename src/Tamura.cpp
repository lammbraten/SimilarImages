#include "../include/Tamura.h"


Tamura::Tamura(int k_min, int k_max) {
	this->k_min = k_min;
	this->k_max = k_max;

}

Tamura::~Tamura() {

}

Mat Tamura::calc_Sbest() {

	for (int k = k_min; k <= k_max; k++) {
		if (k != 0){
			int k_pow = pow(2, k - 1);
			for(int )
		}
	}

	return Mat();
}
