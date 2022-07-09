#include "Matrix.h"
#include "Image.h"
#include "processing.h"
#include "unit_test_framework.h"
#include "Matrix_test_helpers.h"
#include "Image_test_helpers.h"
#include "processing_test_helpers.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cassert>

using namespace std;


// Tests that the middle element of a 3x3 Matrix is not a border element.  
// Tests all other elements of the Matrix are border elements. 
TEST(test_border_element_basic){
  Matrix *mat = new Matrix; // create a Matrix in dynamic memory 
  Matrix_init(mat, 3, 3);

  for (int r = 0; r < Matrix_height(mat); ++r){
    for (int c = 0; c < Matrix_width(mat); ++c){ 
      if (r == 1 && c == 1){
        ASSERT_FALSE(border_element(mat, r, c));
      }else{
        ASSERT_TRUE(border_element(mat, r, c));
      }
    }
  } 

  delete mat; // delete the Matrix
}
  
// Tests that the energy for the middle element in a 3x3 energy Matrix is calculated.
// Tests that the energy for the single non-border element is used to fill in the energies for the border elements.  
TEST(test_compute_energy_matrix_basic){
  Image *img = new Image; // create an Image in dynamic memory
  Matrix *energy = new Matrix; // create a Matrix in dynamic memory 
  
  // Intializes a 3x3 Image.
  string input = "P3\n3 3\n255\n";
  input += "0 0 0 100 100 100 0 0 0 \n";
  input += "0 0 0 0 0 0 0 0 0 \n";
  input += "0 0 0 0 0 0 0 0 0 \n";
  istringstream is(input);
  Image_init(img, is);

  compute_energy_matrix(img, energy);

  ASSERT_EQUAL(Image_height(img), Matrix_height(energy));
  ASSERT_EQUAL(Image_width(img), Matrix_width(energy));

  // Checks that the energy of the non-border element in the 3x3 energy Matrix was calculated.
  ASSERT_TRUE(*Matrix_at(energy, 1, 1) > 0);

  // Checks that the single non-border element of the 3x3 energy Matrix is the max element.
  ASSERT_EQUAL(Matrix_max(energy), *Matrix_at(energy, 1, 1));
  
  // Checks that the max element is used to fill in the energies of the border elements. 
  for (int r = 0; r < Matrix_height(energy); ++r){
    for (int c = 0; c < Matrix_width(energy); ++c){ 
       if (border_element(energy, r, c)){
        ASSERT_EQUAL(Matrix_max(energy), *Matrix_at(energy, r, c));  
      }      
    }
  }

  delete img; // delete the Image
  delete energy; // delete the Matrix
}

// Tests that the correct max in the energy Matrix is found.  Tests the max fills in the border elements energy values.
TEST(test_compute_energy_matrix_max){
  Image *img = new Image; // create an Image in dynamic memory
  Matrix *energy = new Matrix; // create a Matrix in dynamic memory 
  
  // Intializes a 4x4 Image.
  string input = "P3\n4 4\n255\n";
  input += "0 0 0 100 100 100 200 200 200 0 0 0 \n";
  input += "0 0 0 0 0 0 0 0 0 0 0 0 \n";
  input += "0 0 0 0 0 0 0 0 0 0 0 0 \n";
  input += "0 0 0 0 0 0 0 0 0 0 0 0 \n";
  istringstream is(input);
  Image_init(img, is);

  compute_energy_matrix(img, energy);

  // Checks that the expected max element (row 1, column 2) is the max for the entire energy Matrix.
  ASSERT_EQUAL(*Matrix_at(energy, 1, 2), Matrix_max(energy));
  
  // Checks that the max element is used to fill in the energies of the border elements. 
  for (int r = 0; r < Matrix_height(energy); ++r){
    for (int c = 0; c < Matrix_width(energy); ++c){ 
      if (border_element(energy, r, c)){
        ASSERT_EQUAL(Matrix_max(energy), *Matrix_at(energy, r, c));  
      }  
    }
  }

  // Checks that the expected second largest element is less than the max.
  // Checks that the expected third largest element is less than the expected second largest element. 
  ASSERT_TRUE(*Matrix_at(energy, 1, 1) < Matrix_max(energy) && *Matrix_at(energy, 2, 1) < *Matrix_at(energy, 1, 1));
  
  delete img; // delete the Image
  delete energy; // delete the Matrix 
}

TEST(test_compute_vertical_cost_matrix_basic){
  Matrix *energy = new Matrix; // create a Matrix in dynamic memory 
  Matrix *cost = new Matrix;  
   
  Matrix_init(energy, 3, 3);
  Matrix_fill(energy, 1);
  Matrix_fill_border(energy, 2);
  
  compute_vertical_cost_matrix(energy, cost);
  
  ASSERT_EQUAL(Matrix_height(cost), Matrix_height(energy));
  ASSERT_EQUAL(Matrix_width(cost), Matrix_width(energy));

  // Tests that the first row of the cost Matrix.
  for (int c = 0; c < Matrix_width(cost); ++c) {
    ASSERT_EQUAL(*Matrix_at(cost, 0, c), *Matrix_at(energy, 0, c));
  }

  // Tests the second row of the cost Matrix.
  for (int c = 0; c < Matrix_width(cost); ++c) {
    int column_start = c - 1;
    int column_end = c + 2; 
    
    if (column_start < 0) {
      column_start = 0;
    }
    if (column_end > Matrix_width(cost)) {
      column_end = Matrix_width(cost);
    }
    
    if (c == 1){
      ASSERT_EQUAL(*Matrix_at(cost, 1, c), 3);  
    }else{
    ASSERT_EQUAL(*Matrix_at(cost, 1, c), 4);
    }
  }

  // Tests that the third row of the cost Matrix.
  for (int c = 0; c < Matrix_width(cost); ++c) {
    int column_start = c - 1;
    int column_end = c + 2; 
    
    if (column_start < 0) {
      column_start = 0;
    }
    if (column_end > Matrix_width(cost)) {
      column_end = Matrix_width(cost);
    }
    
    ASSERT_EQUAL(*Matrix_at(cost, 2, c), 5);
  }
  
  delete energy; // delete the Matrix
  delete cost;    
}

TEST(test_find_minimal_vertical_seam_basic) {
  Matrix *cost = new Matrix; // create a Matrix in dynamic memory   

  Matrix_init(cost, 3, 3);
  Matrix_fill(cost, 1);
  Matrix_fill_border(cost, 2);
  // Sets a unique minimum value in the first row.  Distinguishs the first row from the last row of the Matrix.
  *Matrix_at(cost, 0, 2) = 0;
 
  int seam[3];
  find_minimal_vertical_seam(cost, seam);
  const int correct_seam[] = {2, 1, 0}; 

  ASSERT_TRUE(array_equal(seam, correct_seam, 3));
  
  delete cost; // delete the Matrix
}

TEST(test_find_minimal_vertical_seam_5x5_matrix) {
  Matrix *cost = new Matrix; // create a Matrix in dynamic memory   

  Matrix_init(cost, 5, 5);
  Matrix_fill(cost, 3);

  // Sets a unique minimum value in a row for the seam carve algorithm to find.    
  *Matrix_at(cost, 4, 2) = 1; 
  *Matrix_at(cost, 3, 3) = 2;
  *Matrix_at(cost, 2, 4) = 1;
  *Matrix_at(cost, 0, 3) = 1;
  
  // Sets minimum values that are outside the bounds for the seam carve algorithm.
  *Matrix_at(cost, 3, 0) = 0;
  *Matrix_at(cost, 3, 4) = 0;
  *Matrix_at(cost, 1, 2) = 0;

  int seam[5];
  find_minimal_vertical_seam(cost, seam);
  const int correct_seam[] = {3, 3, 4, 3, 2}; 

  ASSERT_TRUE(array_equal(seam, correct_seam, 5));
  
  delete cost; // delete the Matrix
}

TEST(test_remove_vertical_seam_basic) {
  Image *img = new Image; // create an Image in dynamic memory
  Image *correct_img = new Image;
   
  // Intializes a 3x3 Image.
  string input = "P3\n3 3\n255\n";
  input += "0 0 0 100 100 100 0 0 0 \n";
  input += "100 100 100 0 0 0 200 200 200 \n";
  input += "50 50 50 0 0 0 0 0 0 \n"; 
  istringstream is(input);
  Image_init(img, is); 
  
  const int seam[] = {0, 1, 0};
  remove_vertical_seam(img, seam);
  ASSERT_TRUE(Image_width(img) >= 1);

  // Intializes a 2x3 Image.
  string correct_input = "P3\n2 3\n255\n";
  correct_input += "100 100 100 0 0 0 \n";
  correct_input += "100 100 100 200 200 200 \n";
  correct_input += "0 0 0 0 0 0 \n";
  istringstream correct_is(correct_input);
  Image_init(correct_img, correct_is);

  ASSERT_TRUE(Image_equal(img, correct_img));

  delete img; // delete the image
  delete correct_img; 
}


TEST_MAIN()