# sgx-kmeans
## Build and run sgx-kmeans
Modify the Makefile for the values of SGX_SDK and SGX_MODE first.
```
  make clean; make
  ./app inputdata.txt 7
```
It printed out the cluster assignments for the first 20 data points for results verification.

And it printed out three time duration (in seconds) for data loading; enclave creation; and actual calculation respectively.

## Generate test data
```
  python gen_points.py inputdata.txt 1000000
```
## kmeans without sgx
```
  g++ kmeans.cpp -o kmeans
```
or using other compilers such as icpc from Intel.

To run:
```
  ./kmeans inputdata.txt 7
```
It printed out the cluster assignments for the first 20 data points for results verification.

And it printed out two time duration (in seconds) for data loading, and actual calculation respectively.
