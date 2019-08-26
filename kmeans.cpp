#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdio.h>
#include <iostream>
#include <chrono>

#include <unistd.h>

#include <float.h>
#include <math.h>

using namespace std;

//int* k_means(float data[10][2], int n, int m, int k, float t, float** centroids) {
int* k_means(float** data, int n, int m, int k, float t, float** centroids) {

	/* output cluster label for each data point */
	int * labels = (int * ) calloc(n, sizeof(int));

	int h, i, j; /* loop counters, of course :) */
	int * counts = (int * ) calloc(k, sizeof(int)); /* size of each cluster */
	float old_error, error = DBL_MAX; /* sum of squared euclidean distance */
	float ** c = centroids ? centroids : (float ** ) calloc(k, sizeof(float * ));
	float ** c1 = (float * * ) calloc(k, sizeof(float * )); /* temp centroids */

	//assert(data && k > 0 && k <= n && m > 0 && t >= 0); /* for debugging */

	/****
	 ** initialization */

	for (h = i = 0; i < k; h += n / k, i++) {
		c1[i] = (float * ) calloc(m, sizeof(float));
		if (!centroids) {
			c[i] = (float * ) calloc(m, sizeof(float));
		}
		/* pick k points as initial centroids */
		for (j = m; j-- > 0; c[i][j] = data[h][j]);
	}

	/****
	 ** main loop */

	do {
		/* save error from last step */
		old_error = error, error = 0;

		/* clear old counts and temp centroids */
		for (i = 0; i < k; counts[i++] = 0) {
			for (j = 0; j < m; c1[i][j++] = 0);
		}

		for (h = 0; h < n; h++) {
			/* identify the closest cluster */
			float min_distance = DBL_MAX;
			for (i = 0; i < k; i++) {
				float distance = 0;
				for (j = m; j-- > 0; distance += pow(data[h][j] - c[i][j], 2));
				if (distance < min_distance) {
					labels[h] = i;
					min_distance = distance;
				}
			}
			/* update size and temp centroid of the destination cluster */
			for (j = m; j-- > 0; c1[labels[h]][j] += data[h][j]);
			counts[labels[h]]++;
			/* update standard error */
			error += min_distance;
		}

		for (i = 0; i < k; i++) { /* update all centroids */
			for (j = 0; j < m; j++) {
				c[i][j] = counts[i] ? c1[i][j] / counts[i] : c1[i][j];
			}
		}

	} while (fabs(error - old_error) > t);

	/****
	 ** housekeeping */

	for (i = 0; i < k; i++) {
		if (!centroids) {
			free(c[i]);
		}
		free(c1[i]);
	}

	if (!centroids) {
		free(c);
	}
	free(c1);

	free(counts);

	return labels;
}

// main for number adding
int main(int argc, char const *argv[]) {
	if(argc != 3){
		cout<<"Usage: prog data.file knum"<<endl;
		return 1;
	}

	cout << "Start data loading..." << endl;
	auto dataloading = chrono::high_resolution_clock::now();

	string datafilename = argv[1];
	//Fetching number of clusters
	int K = atoi(argv[2]);

	std::ifstream infile(datafilename);
	std::string line;
	std::vector<pair<float, float>> points;
	while (std::getline(infile, line))
	{
		std::istringstream iss(line);
		float a, b;
		if (!(iss >> a >> b)) { continue; } // error
		points.push_back(make_pair(a, b));
	}
	int npoints = points.size();
	int curpoint = 0;
	float* data = NULL;
	data = (float*)malloc(npoints * 2 * sizeof(float));
	for (const pair<float,float> &point : points){
		data[curpoint++] = point.first;
		data[curpoint++] = point.second;
	}
	float **dataparam;
	curpoint = 0;
	dataparam = new float*[npoints]; // dynamic array (size 10) of pointers to int
	for (int i = 0; i < npoints; ++i) {
		dataparam[i] = new float[2];
	  // each i-th pointer is now pointing to dynamic array (size 10) of actual int values
	}
	for (int i = 0; i < npoints; ++i) {
		for (int j = 0; j < 2; ++j){
			dataparam[i][j] = data[curpoint++];
		}
	}

	cout << "Start enclave init..." << endl;
	auto enclaveinit = chrono::high_resolution_clock::now();

    cout << "Before calling secure_operation...npoints: " << npoints <<", K: " << K << endl;

	cout << "Start calculation..." << endl;
	auto calculate = chrono::high_resolution_clock::now();

    printf("DEBUG_E0\n");
	int * labels;
	printf("DEBUG_E1\n");
	int dim = 2;
	labels = k_means(dataparam, npoints, dim, K, 1e-4, NULL);
	printf("DEBUG_E2\n");
	for (int i = 0; i < 20; i++) {
	    printf("data point %d is in cluster %d\n", i, labels[i]);
	}

	free(labels);

    printf("DEBUG_E3\n");
    //printf("%s\n", "DEBUG_A1");
    free(data);
    free(dataparam);
	cout << "Finishing..." << endl;
	auto finish = chrono::high_resolution_clock::now();

	chrono::duration<double> duration1 = enclaveinit - dataloading;
	//chrono::duration<double> duration2 = calculate - enclaveinit;
	chrono::duration<double> duration3 = finish - calculate;
	cout << duration1.count() << ", " << duration3.count() << endl;

    return 0;
}

