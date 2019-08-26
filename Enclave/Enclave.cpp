#include <stdio.h>
#include "Enclave_t.h"

#include <stdlib.h>
#include <assert.h>
#include <float.h>
#include <math.h>

int* k_means(float** data, int n, int m, int k, float t, float** centroids) {

	// output cluster label for each data point
	int * labels = (int * ) calloc(n, sizeof(int));

	int h, i, j;
	int * counts = (int * ) calloc(k, sizeof(int)); // size of each cluster
	float old_error, error = DBL_MAX; // sum of squared euclidean distance
	float ** c = centroids ? centroids : (float ** ) calloc(k*m, sizeof(float * ));
	float ** c1 = (float * * ) calloc(k*m, sizeof(float * )); // temp centroids

	/****
	 ** initialization */

	for (h = i = 0; i < k; h += n / k, i++) {
		c1[i] = (float * ) calloc(m, sizeof(float));
		if (!centroids) {
			c[i] = (float * ) calloc(m, sizeof(float));
		}
		// pick k points as initial centroids
		for (j = m; j-- > 0; c[i][j] = data[h][j]);
	}

	do {
		// save error from last step
		old_error = error, error = 0;

		// clear old counts and temp centroids
		for (i = 0; i < k; counts[i++] = 0) {
			for (j = 0; j < m; c1[i][j++] = 0);
		}

		for (h = 0; h < n; h++) {
			// identify the closest cluster
			float min_distance = DBL_MAX;
			for (i = 0; i < k; i++) {
				float distance = 0;
				for (j = m; j-- > 0; distance += pow(data[h][j] - c[i][j], 2));
				if (distance < min_distance) {
					labels[h] = i;
					min_distance = distance;
				}
			}

			// update size and temp centroid of the destination cluster
			for (j = m; j-- > 0; c1[labels[h]][j] += data[h][j]);
			counts[labels[h]]++;
			// update standard error
			error += min_distance;

		}

		for (i = 0; i < k; i++) { // update all centroids
			for (j = 0; j < m; j++) {
				c[i][j] = counts[i] ? c1[i][j] / counts[i] : c1[i][j];
			}
		}

	} while (fabs(error - old_error) > t);


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


void secure_kmeans(float** data, int npoints, int k, size_t ndata) {
	char message[200];
	snprintf(message, 200, "data size: %d, # cluster: %d", npoints, k);
	print_message(message);

	print_message("DEBUG_E0");
	int * labels;
	print_message("DEBUG_E1");
	int dim = 2;
	labels = k_means(data, npoints, dim, k, 1e-4, NULL);
	print_message("DEBUG_E2");
	for (int i = 0; i < 20; i++) {
	    snprintf(message, 200, "data point %d is in cluster %d", i, labels[i]);
	    print_message(message);
	}

	free(labels);
    print_message("DEBUG_E3");
}
