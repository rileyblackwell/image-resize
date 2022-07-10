#ifndef PROCESSING_TEST_HELPERS_H
#define PROCESSING_TEST_HELPERS_H

#include "Matrix.h"
#include "processing.h"


// REQUIRES: mat points to a valid Matrix
//           0 <= r && r < Matrix_height(mat)
//           0 <= c && c < Matrix_width(mat)
// EFFECTS: Returns true if the element is on the Matrix border.  Returns false otherwise. 
bool border_element(const Matrix* mat, int r, int c);

#endif // PROCESSING_TEST_HELPERS_H