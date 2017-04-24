# High Performance Computing Explorations using PETSc and SLEPc

Example code to explore high performance computing using PETSc and SLEPc. Here are a few resources to get you started with MPI clusters.

[Portable, Extensible Toolkit for Scientific Computation](http://www.mcs.anl.gov/petsc/index.html)

[Scalable Library for Eigenvalue Problem Computations](http://slepc.upv.es/)

Both libraries use MPI.

[Running an MPI Program](http://mpitutorial.com/tutorials/mpi-hello-world/)

[Running an MPI Program in a LAN](http://mpitutorial.com/tutorials/running-an-mpi-cluster-within-a-lan/)

[Launching an Amazon EC2 MPI Cluster](http://mpitutorial.com/tutorials/launching-an-amazon-ec2-mpi-cluster/)

## Compiling the example

The makefile is primitive, and reflects my lack of knowlege and time.

Use ```make example1``` to build the first example. Use ```example1 -help``` to see the options. I've been examining memory usage via

- ```./example1 -n_row 200 -n_col 20000 -sparsity 0.03 -mat_type mpiaij```
- ```./example1 -n_row 200 -n_col 20000 -sparsity 0.03 -mat_type mpidense```
