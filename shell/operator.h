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
#define OP_FLUX_WRITE "FLUX_WRITE"
#define OP_MYLS "MYLS"
#define OP_MYPS "MYPS"
#define OP_SET "SET"
#define OP_SET_DISPLAY "SET_DISPLAY"
#define OP_UNSET "UNSET"

#define isAndOperator(op) strcmp(op, OP_AND) == 0
#define isOrOperator(op) strcmp(op, OP_OR) == 0
#define isSeparatorOperator(op) strcmp(op, OP_SEPARATOR) == 0
#define isPipeOperator(op) strcmp(op, OP_PIPE) == 0
#define isBackgroundOperator(op)  strcmp(op, OP_BACKGROUND) == 0
#define isExit(op) strcmp(op, OP_EXIT) == 0
#define isEndOperator(op) strcmp(op, OP_END) == 0
#define isOpFluxReadOperator(op) strcmp(op, OP_FLUX_READ) == 0
#define isOpFluxWriteOperator(op) strcmp(op, OP_FLUX_WRITE) == 0
#define isOpSetOperator(op) strcmp(op, OP_SET) == 0
#define isOpSetDisplayOperator(op) strcmp(op, OP_SET_DISPLAY) == 0
#define isOpUnset(op) strcmp(op, OP_UNSET) == 0
#define isOperator(op) isAndOperator(op) || isOrOperator(op) || isSeparatorOperator(op) || isPipeOperator(op) || isBackgroundOperator(op) \
	|| isOpFluxWriteOperator(op) || isOpFluxReadOperator(op) || isOpSetOperator(op) || isOpSetDisplayOperator(op) || isOpUnset(op)
#define isOpFluxWriteOperator(op) strcmp(op, OP_FLUX_WRITE) == 0

#endif
