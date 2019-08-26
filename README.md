# sgx-kmeans
## Build and run sgx-kmeans
```
  make clean; make
  ./app inputdata.txt 7
```
## Generate test data
```
  python gen_points.py inputdata.txt 1000000
```
## kmeans without sgx
```
  g++ kmeans.cpp -o kmeans
```
or using other compilers such as icpc from Intel.
