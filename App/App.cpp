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
#include "Enclave_u.h"
#include "sgx_urts.h"
#include "sgx_utils/sgx_utils.h"

using namespace std;

/* Global EID */
sgx_enclave_id_t global_eid = 0;

// OCALL implementations
void print_message(const char* str) {
    std::cout << str << std::endl;
}

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
	dataparam = new float*[npoints];
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

    if (initialize_enclave(&global_eid, "enclave.token", "enclave.signed.so") < 0) {
        std::cout << "Fail to initialize enclave." << std::endl;
        return 1;
    }

    cout << "Before calling secure_operation...npoints: " << npoints <<", K: " << K << endl;

	cout << "Start calculation..." << endl;
	auto calculate = chrono::high_resolution_clock::now();

	size_t ndata = npoints * 2;
    secure_kmeans(global_eid, dataparam, npoints, K, ndata);
    printf("%s\n", "DEBUG_A1");
    free(data);
    free(dataparam);
	cout << "Finishing..." << endl;
	auto finish = chrono::high_resolution_clock::now();

	chrono::duration<double> duration1 = enclaveinit - dataloading;
	chrono::duration<double> duration2 = calculate - enclaveinit;
	chrono::duration<double> duration3 = finish - calculate;
	cout << duration1.count() << ", " << duration2.count() << ", " << duration3.count() << endl;

    return 0;
}
