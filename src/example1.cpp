#include <unistd.h>
#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <cmath>

#include "slepcsvd.h"

#include "get_rss.h"

using namespace std;

string help("Create a random sparse or dense matrix.\n"
    "Use\n\t-n-row # rows\n\t-n-cols # columns\n\t-sparsity float\n\t-mat_type mpidense for a dense matrix\n\t"
            "-mat_type mpiaij for a sparse matrix\n\n");

int main(int argc, char **args) {
    PetscRandom rctx;
    PetscScalar rnd;
    PetscReal sparsity;
    PetscInt n_row, n_col, max_count;
    PetscBool hh = (PetscBool)0;
    PetscBool slow = (PetscBool)0;
    vector<int> cols;
    vector<double> values;
    Mat A;

    cout << "Initialize" << endl;
    PetscInitialize(&argc, &args, NULL, help.c_str());

    PetscOptionsGetBool(NULL, NULL, "-help", &hh, NULL);
    if (hh) {
        PetscFinalize();
        exit(1);
    }
    PetscOptionsGetInt(NULL, NULL, "-max_features", &max_count, NULL);
    PetscOptionsGetReal(NULL, NULL, "-sparsity", &sparsity, NULL);
    PetscOptionsGetInt(NULL, NULL, "-n_row", &n_row, NULL);
    PetscOptionsGetInt(NULL, NULL, "-n_col", &n_col, NULL);
    PetscOptionsGetBool(NULL, NULL, "-slow", &slow, NULL);
    cout << "Got options n_row " << n_row << " n_col " << n_col << endl;

    MatCreate(PETSC_COMM_WORLD, &A);

    // matrix, rows_local, cols_local, rows_global, cols_global - let petsc decide local sizes
    MatSetSizes(A, PETSC_DECIDE, PETSC_DECIDE, n_row, n_col);
    MatSetFromOptions(A);
    MatSetUp(A);

    PetscRandomCreate(PETSC_COMM_WORLD, &rctx);
    PetscRandomSetInterval(rctx, 0.0, 1.0);
    if (slow) {
        // this loop is very, very slow
        cout << "slow" << endl;
        for (PetscInt i = 0; i < n_row; i++) {
            for (PetscInt j = 0; j < n_col; j++) {
                PetscRandomGetValue(rctx, &rnd);
                rnd = rnd > sparsity ? 0.0 : rnd;
                rnd = max_count * rnd;
                MatSetValue(A, i, j, rnd, INSERT_VALUES);
            }
        }
    } else {
        values.reserve(ceil(sparsity * n_col));
        cols.reserve(ceil(sparsity * n_col));
        cout << "fast" << endl;
        for (int i = 0; i < n_row; i++) {
            for (int j = 0; j < n_col; j++) {
                PetscRandomGetValue(rctx, &rnd);
                rnd = rnd > sparsity ? 0.0 : rnd;
                rnd = max_count * rnd;
                if (rnd > 0.0) {
                    values.push_back(rnd);
                    cols.push_back(j);
                }
            }
            MatSetValues(A, 1, &i, cols.size(), cols.data(), values.data(), INSERT_VALUES);
            cols.clear();
            values.clear();
        }
    }
    PetscRandomDestroy(&rctx);
    MatAssemblyBegin(A, MAT_FINAL_ASSEMBLY);
    // perform computations while messages are in transit via MPI
    MatAssemblyEnd(A, MAT_FINAL_ASSEMBLY);
    cout << "Assembled" << endl;

    MatDestroy(&A);
    PetscFinalize();

    cout << "Peak RSS " << mine::getPeakRSSMb() << " Mb" << endl;
}
