#include <cstdlib>
#include <iostream>
#include <math.h>
#include <cfloat>

using namespace std;

void pointswap(double* x, double* y, int a, int b);
double close_distance(double* x_sorted_by_x, double* y_sorted_by_x, int start, int end, double* x_sorted_by_y, double* y_sorted_by_y, int num);
double point_distance(double* x, double* y, int a, int b);
void mergesort_by_x(double* x, double* y, int start, int end);
void merge_two_part_by_x(double* x, double* y, int start, int mid, int end);
void mergesort_by_y(double* x, double* y, int start, int end);
void merge_two_part_by_y(double* x, double* y, int start, int mid, int end);

int main(void) {
	int run = 0;
	cin >> run;

	for (int i = 0; i < run; i++) {
		int number = 0;
		cin >> number;
		double d;
		double* x_sorted_by_x = new double[number];
		double* y_sorted_by_x = new double[number];
		double* x_sorted_by_y = new double[number];
		double* y_sorted_by_y = new double[number];

		for (int j = 0; j < number; j++) {
			cin >> x_sorted_by_x[j] >> y_sorted_by_x[j];
			x_sorted_by_y[j] = x_sorted_by_x[j];
			y_sorted_by_y[j] = y_sorted_by_x[j];
		}
		/* point sorted by x */
		mergesort_by_x(x_sorted_by_x, y_sorted_by_x, 0, number - 1);
		/* point sorted by y */
		mergesort_by_y(x_sorted_by_y, y_sorted_by_y, 0, number - 1);
		d = close_distance(x_sorted_by_x, y_sorted_by_x, 0, number - 1, x_sorted_by_y, y_sorted_by_y, number);
		cout << d << endl;
		delete[] x_sorted_by_x;
		delete[] y_sorted_by_x;
		delete[] x_sorted_by_y;
		delete[] y_sorted_by_y;
	}

	return(0);
}

void pointswap(double* x, double* y, int a, int b) {
	double tempx = x[a];
	double tempy = y[a];
	x[a] = x[b];
	y[a] = y[b];
	x[b] = tempx;
	y[b] = tempy;
}

double close_distance(double* x_sorted_by_x, double* y_sorted_by_x, int start, int end, double* x_sorted_by_y, double* y_sorted_by_y, int num) {
	/* the number of point is small enough */
	if ((end - start + 1) <= 3) {
		double d = DBL_MAX;
		double temp;
		for (int i = start; i <= end; i++) {
			for (int j = i + 1; j <= end; j++) {
				temp = point_distance(x_sorted_by_x, y_sorted_by_x, i, j);
				if (d > temp) {
					d = temp;
				}
			}
		}
		return d;
	}

	int mid = floor((start + end) / 2.0);
	double dl, dr, d;
	
	double* left_x_sorted_by_y = new double[num];
	double* left_y_sorted_by_y = new double[num];
	double* right_x_sorted_by_y = new double[num];
	double* right_y_sorted_by_y = new double[num];
	int divide_to_left = 0, divide_to_right = 0;
	/* divide point sorted by y into left part and right part */
	for (int i = 0; i < num; i++) {
		if (x_sorted_by_y[i] <= x_sorted_by_x[mid]) {
			left_x_sorted_by_y[divide_to_left] = x_sorted_by_y[i];
			left_y_sorted_by_y[divide_to_left] = y_sorted_by_y[i];
			divide_to_left++;
		}
		else if(x_sorted_by_y[i] > x_sorted_by_x[mid]) {
			right_x_sorted_by_y[divide_to_right] = x_sorted_by_y[i];
			right_y_sorted_by_y[divide_to_right] = y_sorted_by_y[i];
			divide_to_right++;
		}
		
	}
	
	dl = close_distance(x_sorted_by_x, y_sorted_by_x, start, mid, left_x_sorted_by_y, left_y_sorted_by_y, divide_to_left);
	dr = close_distance(x_sorted_by_x, y_sorted_by_x, mid + 1, end, right_x_sorted_by_y, right_y_sorted_by_y, divide_to_right);
	
	delete[] left_x_sorted_by_y;
	delete[] left_y_sorted_by_y;
	delete[] right_x_sorted_by_y;
	delete[] right_y_sorted_by_y;

	if (dl >= dr) {
		d = dr;
	}
	else {
		d = dl;
	}

	int central_box_size = 0;
	double* box_point_x = new double[num];
	double* box_point_y = new double[num];
	/* put point sorted by y which closer than min d into box */
	for (int i = 0; i < num; i++) {
		if (abs(x_sorted_by_y[i] - x_sorted_by_x[mid]) < d) {
			box_point_x[central_box_size] = x_sorted_by_y[i];
			box_point_y[central_box_size] = y_sorted_by_y[i];
			central_box_size++;
		}
	}
	/* compute the distance of point in the box */
	for (int i = 0; i < central_box_size; i++) {
		/* because sorted by y, at most compute distance between behind 7 points */
		int j = i + 1;
		while (j < i + 8 && j < central_box_size) {
			if (abs(box_point_y[i] - box_point_y[j]) < d) {
				double temp = point_distance(box_point_x, box_point_y, i, j);
				if (temp < d) {
					d = temp;
				}
			}
			j++;
		}
	}
	delete[] box_point_x;
	delete[] box_point_y;
	
	return(d);
}

double point_distance(double* x, double* y, int a, int b) {
	double temp;
	temp = pow(x[a] - x[b], 2) + pow(y[a] - y[b], 2);
	return(sqrt(temp));
}

void mergesort_by_x(double* x, double* y, int start, int end) {
	if (start < end) {
		int mid = (start + end) / 2;
		mergesort_by_x(x, y, start, mid);
		mergesort_by_x(x, y, mid + 1, end);
		merge_two_part_by_x(x, y, start, mid, end);
	}
}

void merge_two_part_by_x(double* x, double* y, int start, int mid, int end) {

	int num = end - start + 1;
	double* tempx = new double[num];
	double* tempy = new double[num];

	int left_count = start, right_count = mid + 1, temp_count = 0;
	while (left_count <= mid && right_count <= end) {
		if (x[left_count] <= x[right_count]) {
			tempx[temp_count] = x[left_count];
			tempy[temp_count] = y[left_count];
			left_count++;
			temp_count++;
		}
		else {
			tempx[temp_count] = x[right_count];
			tempy[temp_count] = y[right_count];
			right_count++;
			temp_count++;
		}
	}

	if (left_count <= mid) {
		for (int i = temp_count; i < num; i++) {
			tempx[i] = x[left_count];
			tempy[i] = y[left_count];
			left_count++;
		}
	}
	else {
		for (int i = temp_count; i < num; i++) {
			tempx[i] = x[right_count];
			tempy[i] = y[right_count];
			right_count++;
		}
	}

	for (int i = start; i <= end; i++) {
		x[i] = tempx[i - start];
		y[i] = tempy[i - start];
	}
	delete[] tempx;
	delete[] tempy;

}

void mergesort_by_y(double* x, double* y, int start, int end) {
	if (start < end) {
		int mid = (start + end) / 2;
		mergesort_by_y(x, y, start, mid);
		mergesort_by_y(x, y, mid + 1, end);
		merge_two_part_by_y(x, y, start, mid, end);
	}
}

void merge_two_part_by_y(double* x, double* y, int start, int mid, int end) {

	int num = end - start + 1;
	double* tempx = new double[num];
	double* tempy = new double[num];

	int left_count = start, right_count = mid + 1, temp_count = 0;
	while (left_count <= mid && right_count <= end) {
		if (y[left_count] <= y[right_count]) {
			tempx[temp_count] = x[left_count];
			tempy[temp_count] = y[left_count];
			left_count++;
			temp_count++;
		}
		else {
			tempx[temp_count] = x[right_count];
			tempy[temp_count] = y[right_count];
			right_count++;
			temp_count++;
		}
	}

	if (left_count <= mid) {
		for (int i = temp_count; i < num; i++) {
			tempx[i] = x[left_count];
			tempy[i] = y[left_count];
			left_count++;
		}
	}
	else {
		for (int i = temp_count; i < num; i++) {
			tempx[i] = x[right_count];
			tempy[i] = y[right_count];
			right_count++;
		}
	}

	for (int i = start; i <= end; i++) {
		x[i] = tempx[i - start];
		y[i] = tempy[i - start];
	}
	delete[] tempx;
	delete[] tempy;

}