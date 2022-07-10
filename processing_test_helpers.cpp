#include "processing_test_helpers.h"


// REQUIRES: mat points to a valid Matrix
//           0 <= r && r < Matrix_height(mat)
//           0 <= c && c < Matrix_width(mat)
// EFFECTS: Returns true if the element is on the Matrix border.  Returns false otherwise. 
bool border_element(const Matrix* mat, int r, int c){
  if (r == 0 || c == 0 || r == Matrix_height(mat) - 1 || c == Matrix_width(mat) - 1){
    return true;
  }
  return false;
} 