#ifndef OPERATOR
#define OPERATOR

#include <string.h>
#define OP_AND "AND"
#define OP_OR "OR"
#define OP_SEPARATOR "SEPARATOR"
#define OP_PIPE "PIPE"
#define OP_BACKGROUND "BACKGROUND"
#define OP_EXIT "EXIT"
#define OP_END "END"

#define OP_FLUX_READ "FLUX_READ"
#define OP_FLUX_WRITE_OUT "FLUX_WRITE_OUT"
#define OP_FLUX_WRITE_OUT_DOUBLE "FLUX_WRITE_OUT_DOUBLE"
#define OP_FLUX_WRITE_ERR "FLUX_WRITE_ERR"
#define OP_FLUX_WRITE_ERR_DOUBLE "FLUX_WRITE_ERR_DOUBLE"
#define OP_FLUX_WRITE_BOTH "FLUX_WRITE_BOTH"
#define OP_FLUX_WRITE_BOTH_DOUBLE "FLUX_WRITE_BOTH_DOUBLE"

#define OP_MYLS "MYLS"
#define OP_MYPS "MYPS"


#define isAndOperator(op) strcmp(op, OP_AND) == 0
#define isOrOperator(op) strcmp(op, OP_OR) == 0
#define isSeparatorOperator(op) strcmp(op, OP_SEPARATOR) == 0
#define isPipeOperator(op) strcmp(op, OP_PIPE) == 0
#define isBackgroundOperator(op)  strcmp(op, OP_BACKGROUND) == 0
#define isExit(op) strcmp(op, OP_EXIT) == 0
#define isEndOperator(op) strcmp(op, OP_END) == 0
#define isOpFluxReadOperator(op) strcmp(op, OP_FLUX_READ) == 0
#define isOpFluxWriteOutOperator(op) strcmp(op, OP_FLUX_WRITE_OUT) == 0
#define isOpFluxWriteOutDoubleOperator(op) strcmp(op, OP_FLUX_WRITE_OUT_DOUBLE) == 0
#define isOpFluxWriteErrOperator(op) strcmp(op, OP_FLUX_WRITE_ERR) == 0
#define isOpFluxWriteErrDoubleOperator(op) strcmp(op, OP_FLUX_WRITE_ERR_DOUBLE) == 0
#define isOpFluxWriteBothOperator(op) strcmp(op, OP_FLUX_WRITE_BOTH) == 0
#define isOpFluxWriteBothDoubleOperator(op) strcmp(op, OP_FLUX_WRITE_BOTH_DOUBLE) == 0

#define isOperator(op) isAndOperator(op) || isOrOperator(op) || isSeparatorOperator(op) || isPipeOperator(op) || isBackgroundOperator(op) \
	|| isOpFluxWriteOutOperator(op) || isOpFluxWriteOutDoubleOperator(op) || isOpFluxWriteErrOperator(op) \
	|| isOpFluxWriteErrDoubleOperator(op) || isOpFluxWriteBothOperator(op) || isOpFluxWriteBothDoubleOperator(op) \
	|| isOpFluxReadOperator(op)

#endif
