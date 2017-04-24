SLEPC_DIR=/opt/slepc/slepc-devel-dbg
PETSC_DIR=/opt/petsc/petsc-devel-dbg
include $(SLEPC_DIR)/lib/slepc/conf/slepc_common

example1: example1.o chkopts
	-${CLINKER} -o example1 example1.o ${SLEPC_EPS_LIB}
