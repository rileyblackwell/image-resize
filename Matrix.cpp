#include <cassert>
#include "Matrix.h"
using namespace std;

// REQUIRES: mat points to a Matrix
//           0 < width && width <= MAX_MATRIX_WIDTH
//           0 < height && height <= MAX_MATRIX_HEIGHT
// MODIFIES: *mat
// EFFECTS:  Initializes *mat as a Matrix with the given width and height.
// NOTE:     Do NOT use new or delete here.
void Matrix_init(Matrix* mat, int width, int height) {
  assert(0 < width && width <= MAX_MATRIX_WIDTH);
  assert(0 < height && height <= MAX_MATRIX_HEIGHT);
  mat->width = width;
  mat->height = height;
}

// REQUIRES: mat points to a valid Matrix
// MODIFIES: os
// EFFECTS:  First, prints the width and height for the Matrix to os:
//             WIDTH [space] HEIGHT [newline]
//           Then prints the rows of the Matrix to os with one row per line.
//           Each element is followed by a space and each row is followed
//           by a newline. This means there will be an "extra" space at
//           the end of each line.
void Matrix_print(const Matrix* mat, ostream& os) {
  const int height = Matrix_height(mat);
  const int width = Matrix_width(mat);
  os << width << " " << height << endl;
  for (int r = 0; r < height; ++r){
    for (int c = 0; c < width; ++c){
      os << *Matrix_at(mat, r, c) << " ";
    }
    os << endl;
  }  
}

// REQUIRES: mat points to an valid Matrix
// EFFECTS:  Returns the width of the Matrix.
int Matrix_width(const Matrix* mat) {
  return mat->width;
}

// REQUIRES: mat points to a valid Matrix
// EFFECTS:  Returns the height of the Matrix.
int Matrix_height(const Matrix* mat) {
  return mat->height;
}

// REQUIRES: mat points to a valid Matrix
//           ptr points to an element in the Matrix
// EFFECTS:  Returns the row of the element pointed to by ptr.
int Matrix_row(const Matrix* mat, const int* ptr) {
  const int* start_ptr = &mat->data[0];
  const int index = (ptr - start_ptr); 
  return index / Matrix_width(mat); // floor division
}

// REQUIRES: mat points to a valid Matrix
//           ptr point to an element in the Matrix
// EFFECTS:  Returns the column of the element pointed to by ptr.
int Matrix_column(const Matrix* mat, const int* ptr) {
  const int* start_ptr = &mat->data[0];
  const int index = (ptr - start_ptr); 
  return index % Matrix_width(mat);
}

// REQUIRES: mat points to a valid Matrix
//           0 <= row && row < Matrix_height(mat)
//           0 <= column && column < Matrix_width(mat)
//
// MODIFIES: (The returned pointer may be used to modify an
//            element in the Matrix.)
// EFFECTS:  Returns a pointer to the element in the Matrix
//           at the given row and column.
int* Matrix_at(Matrix* mat, int row, int column) {
  assert(0 <= row && row < Matrix_height(mat));
  assert(0 <= column && column < Matrix_width(mat));   
  const int index = Matrix_width(mat) * row + column; 
  int* element_ptr = &mat->data[index];
  return element_ptr;
}

// REQUIRES: mat points to a valid Matrix
//           0 <= row && row < Matrix_height(mat)
//           0 <= column && column < Matrix_width(mat)
//
// EFFECTS:  Returns a pointer-to-const to the element in
//           the Matrix at the given row and column.
const int* Matrix_at(const Matrix* mat, int row, int column) {
  assert(0 <= row && row < Matrix_height(mat));
  assert(0 <= column && column < Matrix_width(mat));   
  const int index = Matrix_width(mat) * row + column; 
  const int* c_element_ptr = &mat->data[index];
  return c_element_ptr;
}

// REQUIRES: mat points to a valid Matrix
// MODIFIES: *mat
// EFFECTS:  Sets each element of the Matrix to the given value.
void Matrix_fill(Matrix* mat, int value) {
  const int height = Matrix_height(mat);
  const int width = Matrix_width(mat);
  for (int r = 0; r < height; ++r){
    for (int c = 0; c < width; ++c){
      int* element_ptr = Matrix_at(mat, r, c);
      *element_ptr = value;
    }
  }
}

// REQUIRES: mat points to a valid Matrix
// MODIFIES: *mat
// EFFECTS:  Sets each element on the border of the Matrix to
//           the given value. These are all elements in the first/last
//           row or the first/last column.
void Matrix_fill_border(Matrix* mat, int value) {
  const int height = Matrix_height(mat);
  const int width = Matrix_width(mat);
  const int max_row = height - 1;
  const int max_column = width - 1;
  for (int r = 0; r < height; ++r){
    for (int c = 0; c < width; ++c){
      if (r == 0 || r == max_row || c == 0 || c == max_column){
        int* element_ptr = Matrix_at(mat, r, c);
        *element_ptr = value;
      }  
    }
  }   
}

// REQUIRES: mat points to a valid Matrix
// EFFECTS:  Returns the value of the maximum element in the Matrix
int Matrix_max(const Matrix* mat) {
  int max_value = *Matrix_at(mat, 0, 0);
  const int height = Matrix_height(mat);
  const int width = Matrix_width(mat);
  for (int r = 0; r < height; ++r){
    for (int c = 0; c < width; ++c){
      const int* element_ptr = Matrix_at(mat, r, c);
      if (*element_ptr > max_value){
        max_value = *element_ptr;
      }
    }
  }  
  return max_value;
}

// REQUIRES: mat points to a valid Matrix
//           0 <= row && row < Matrix_height(mat)
//           0 <= column_start && column_end <= Matrix_width(mat)
//           column_start < column_end
// EFFECTS:  Returns the column of the element with the minimal value
//           in a particular region. The region is defined as elements
//           in the given row and between column_start (inclusive) and
//           column_end (exclusive).
//           If multiple elements are minimal, returns the column of
//           the leftmost one.
int Matrix_column_of_min_value_in_row(const Matrix* mat, int row,
                                      int column_start, int column_end) {
  assert(0 <= row && row < Matrix_height(mat));
  assert(0 <= column_start && column_end <= Matrix_width(mat));
  assert(column_start < column_end);
  int min_value = *Matrix_at(mat, row, column_start);
  int min_column = column_start;
  for (int c = column_start; c < column_end; ++c){
    // The simple case: a new min value is found and the column is a saved as the new best. 
    if (min_value > *Matrix_at(mat, row, c)){
      min_column = c;
    } // The equal values case: A value is equal to the min value.  If the column is less than min_column then a new best column is saved.
    else if (min_value == *Matrix_at(mat, row, c)){ 
      if (min_column > c){
        min_column = c;
      }
    }
  }
  return min_column;
}

// REQUIRES: mat points to a valid Matrix
//           0 <= row && row < Matrix_height(mat)
//           0 <= column_start && column_end <= Matrix_width(mat)
//           column_start < column_end
// EFFECTS:  Returns the minimal value in a particular region. The region
//           is defined as elements in the given row and between
//           column_start (inclusive) and column_end (exclusive).
int Matrix_min_value_in_row(const Matrix* mat, int row,
                            int column_start, int column_end) {
  assert(0 <= row && row < Matrix_height(mat));
  assert(0 <= column_start && column_end <= Matrix_width(mat));
  assert(column_start < column_end);
  int min_value = *Matrix_at(mat, row, column_start);
  for (int c = column_start; c < column_end; ++c){
    if (min_value > *Matrix_at(mat, row, c)){
      min_value = *Matrix_at(mat, row, c);
    }
  }
  return min_value;
}
