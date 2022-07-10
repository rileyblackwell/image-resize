#include "Matrix_test_helpers.h"
#include "unit_test_framework.h"
#include <sstream>

using namespace std;


// Here's a free test for you! Model yours after this one.
// Test functions have no interface and thus no RMEs, but
// add a comment like the one here to say what it is testing.
// -----
// Fills a 3x5 Matrix with a value and checks
// that Matrix_at returns that value for each element.
TEST(test_fill_basic) {
  Matrix *mat = new Matrix; // create a Matrix in dynamic memory

  const int width = 3;
  const int height = 5;
  const int value = 42;
  Matrix_init(mat, 3, 5);
  Matrix_fill(mat, value);

  for(int r = 0; r < height; ++r){
    for(int c = 0; c < width; ++c){
      ASSERT_EQUAL(*Matrix_at(mat, r, c), value);
    }
  }

  delete mat; // delete the Matrix
}

// ADD YOUR TESTS HERE

// Tests that the height and width members for a Matrix are intialized correctly. 
TEST(test_matrix_init_basic){
  Matrix *mat = new Matrix; // creates a Matrix in dynamic memory
  
  Matrix_init(mat, 3, 5);
 
  ASSERT_TRUE(0 < Matrix_width(mat) && Matrix_width(mat) <= MAX_MATRIX_WIDTH);
  ASSERT_TRUE(0 < Matrix_height(mat) && Matrix_height(mat) <= MAX_MATRIX_HEIGHT);  
  ASSERT_EQUAL(Matrix_width(mat), 3);
  ASSERT_EQUAL(Matrix_height(mat), 5);
 
  delete mat; // deletes the Matrix   
}

// Compares the output from Matrix_print with the correct output. 
TEST(test_matrix_print_basic){
  Matrix *mat = new Matrix; // creates a Matrix in dynamic memory

  Matrix_init(mat, 3, 3);
  Matrix_fill(mat, 1);
  Matrix_fill_border(mat, 2);
  string correct_output = "3 3\n2 2 2 \n2 1 2 \n2 2 2 \n";
  ostringstream ss_output;
  Matrix_print(mat, ss_output);
  
  ASSERT_EQUAL(ss_output.str(), correct_output);
  
  delete mat; // deletes the Matrix    
}

// Tests that Matrix_row and Matrix_column return a correct and valid row and column.
TEST(test_rows_columns_basic){
  Matrix *mat = new Matrix; // creates a Matrix in dynamic memory
  
  Matrix_init(mat, 5, 3);
  int length = Matrix_width(mat) * Matrix_height(mat) - 1;
  
  for (int i = 0; i < length; i++){
    int* index_ptr = &mat->data[i]; // creates a pointer to the ith element of the array.
    int row = Matrix_row(mat, index_ptr);
    int column = Matrix_column(mat, index_ptr);
    // Checks rows
    ASSERT_TRUE(0 <= row && row < Matrix_height(mat));
    if (i < 5){
      ASSERT_EQUAL(row, 0);
    }else if (i < 10) {
      ASSERT_EQUAL(row, 1);
    }else {
      ASSERT_EQUAL(row, 2);
    }
    // Checks columns
    ASSERT_TRUE(0 <= column && column < Matrix_width(mat));
    if(i % 5 == 0){
      ASSERT_EQUAL(column, 0);
    }else if (i % 5 == 1){
      ASSERT_EQUAL(column, 1);
    }else if (i % 5 == 2){
      ASSERT_EQUAL(column, 2);
    }else if (i % 5 == 3 ){
      ASSERT_EQUAL(column, 3);  
    }else{
      ASSERT_EQUAL(column, 4);
    }
  }     
  
  delete mat; // deletes the Matrix 
}

// Tests that Matrix_at returns the correct border and non-border values.
TEST(test_matrix_at_basic){
  Matrix *mat = new Matrix; // creates a Matrix in dynamic memory 
  
  Matrix_init(mat, 3, 3);
  Matrix_fill(mat, 1);
  Matrix_fill_border(mat, 2);

  for(int r = 0; r < Matrix_height(mat); ++r){
    for(int c = 0; c < Matrix_width(mat); ++c){
      // Checks that the non-border element in the Matrix is equal to the value for Matrix_fill.
      if (r == 1 && c == 1){
      ASSERT_EQUAL(*Matrix_at(mat, r, c), 1);
      }
      // Checks that all border elements equal the value for Matrix_fill_border.
      else{
        ASSERT_EQUAL(*Matrix_at(mat, r, c), 2);
      }
    }
  } 
  
  delete mat; // deletes the Matrix        
}

// Tests that const Matrix_at returns the correct border and non-border values. 
TEST(test_const_matrix_at_basic){
  Matrix *mat = new Matrix; // creates a Matrix in dynamic memory 
  
  Matrix_init(mat, 3, 3);
  Matrix_fill(mat, 1);
  Matrix_fill_border(mat, 2);

  const Matrix *const_mat = mat; 

  for(int r = 0; r < Matrix_height(mat); ++r){
    for(int c = 0; c < Matrix_width(mat); ++c){
      // Checks that the non-border element in the Matrix is equal to the value for Matrix_fill.
      if (r == 1 && c == 1){
      ASSERT_EQUAL(*Matrix_at(const_mat, r, c), 1);
      }
      // Checks that all border elements equal the value for Matrix_fill_border.
      else{
        ASSERT_EQUAL(*Matrix_at(const_mat, r, c), 2);
      }
    }
  } 
  
  delete mat; // deletes the Matrix      
}  

// Tests that every row and column pair that is a border of the matrix
// contains a element that equals border_value.  Also asserts non-border row and column pairs
// are equal to the value that the whole matrix was filled with.  
TEST(test_matrix_fill_border_basic){
  Matrix *mat = new Matrix; // creates a Matrix in dynamic memory

  const int width = 5;
  const int height = 3;
  const int value = 1;
  const int border_value = 2;
  Matrix_init(mat, width, height);
  Matrix_fill(mat, value);
  Matrix_fill_border(mat, border_value);
  
  for(int r = 0; r < height; ++r){
    for(int c = 0; c < width; ++c){
      if (r == 0 || r == (height -1) || c == 0 || c == (width -1)){
        ASSERT_EQUAL(*Matrix_at(mat, r, c), border_value);
      }else{
        ASSERT_EQUAL(*Matrix_at(mat, r, c), value);  
      }
    }
  }

  delete mat; // deletes the Matrix 
}

// Tests that every row and column pair in the matrix is less than or equal to the value of Matrix_max.
TEST(test_matrix_max_basic){
  Matrix *mat = new Matrix; // creates a Matrix in dynamic memory

  const int width = 5;
  const int height = 3;
  Matrix_init(mat, width, height);
  Matrix_fill(mat, 1);
  Matrix_fill_border(mat, 2);
  // Sets the maximum value for an element in the matrix.
  int *element = Matrix_at(mat, 0, 0);
  *element = 3;
  const int expected_max = Matrix_max(mat);  
  
  for(int r = 0; r < height; ++r){
    for(int c = 0; c < width; ++c){
      ASSERT_TRUE(expected_max >= *Matrix_at(mat, r, c));
    }
  }
  
  delete mat; // deletes the Matrix 
}

// Tests that the value of every column in row 0 of the Matrix between column_start and column_end is more than
// the value at row 0 and Matrix_column_of_min_value_in_row.  
// If the values are equal then check that the column is greater than Matrix_column_of_min_value_in_row.     
TEST(test_matrix_column_of_min_value_in_row_basic){
  Matrix *mat = new Matrix; // creates a Matrix in dynamic memory

  const int width = 5;
  const int height = 3;
  const int column_start = 0;
  const int column_end = 4;
  const int row = 0;
  Matrix_init(mat, width, height);
  Matrix_fill(mat, 2);
  // Sets 2 elements in row 0 to the minimum value for all elements between column_start and column_end within row 0.
  int *element_column_1 = Matrix_at(mat, 0, 0);
  *element_column_1 = 1;
  int *element_column_2 = Matrix_at(mat, 0, 1);
  *element_column_2 = 1;
  ASSERT_TRUE(0 <= row && row < Matrix_height(mat));
  ASSERT_TRUE(0 <= column_start && column_end <= Matrix_width(mat));
  ASSERT_TRUE(column_start < column_end);
  const int min_column = Matrix_column_of_min_value_in_row(mat, row, column_start, column_end);
  
  for (int c = column_start; c < column_end; ++c){
    // Checks if 2 or more values are the min that the left most value is choosen as the min. 
    if (*Matrix_at(mat, 0, min_column)  == *Matrix_at(mat, 0, c)){
      if (min_column != c){
        ASSERT_TRUE(min_column < c);
      }
    } // Checks the simple case that min_column is the column with the min value for a given range of columns.
    else{ 
        ASSERT_TRUE(*Matrix_at(mat, 0, min_column) < *Matrix_at(mat, 0, c));
    }     
  }
  
  delete mat; // deletes the Matrix  
}
 
// Sets a minimum value of 1 for row 0 zero and checks that Matrix_min_value_in_row returns the min for row 0.   
TEST(test_matrix_min_value_in_row_basic){
  Matrix *mat = new Matrix; // creates a Matrix in dynamic memory

  const int column_start = 0;
  const int column_end = 4;
  const int row = 0;
  Matrix_init(mat, 5, 3);
  Matrix_fill(mat, 2);
  Matrix_fill_border(mat, 3);
  // Sets a minimum element for row 0.
  int *element = Matrix_at(mat, 0, 0);
  *element = 1;
   
  ASSERT_TRUE(0 <= row && row < Matrix_height(mat));
  ASSERT_TRUE(0 <= column_start && column_end <= Matrix_width(mat));
  ASSERT_TRUE(column_start < column_end);
  // Checks that the correct minimum value of 1 is return from Matrix_min_value_in_row.
  ASSERT_TRUE(Matrix_min_value_in_row(mat, row, column_start, column_end) == 1);
   
  delete mat; // deletes the Matrix  
}

 
// You are encouraged to use any functions from Matrix_test_helpers.h as needed.


// NOTE: The unit test framework tutorial in Lab 2 originally
// had a semicolon after TEST_MAIN(). Although including and
// excluding the semicolon are both correct according to the c++
// standard, the version of g++ we are using reports a warning
// when there is a semicolon. Therefore do NOT add a semicolon
// after TEST_MAIN()
TEST_MAIN() // Do NOT put a semicolon here
