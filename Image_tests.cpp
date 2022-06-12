#include "Matrix.h"
#include "Image_test_helpers.h"
#include "unit_test_framework.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cassert>

using namespace std; 


// Here's a free test for you! Model yours after this one.
// Test functions have no interface and thus no RMEs, but
// add a comment like the one here to say what it is testing.
// -----
// Sets various pixels in a 2x2 Image and checks
// that Image_print produces the correct output.
TEST(test_print_basic) {
  Image *img = new Image; // create an Image in dynamic memory

  const Pixel red = {255, 0, 0};
  const Pixel green = {0, 255, 0};
  const Pixel blue = {0, 0, 255};
  const Pixel white = {255, 255, 255};

  Image_init(img, 2, 2);
  Image_set_pixel(img, 0, 0, red);
  Image_set_pixel(img, 0, 1, green);
  Image_set_pixel(img, 1, 0, blue);
  Image_set_pixel(img, 1, 1, white);

  // Capture our output
  ostringstream s;
  Image_print(img, s);

  // Correct output
  ostringstream correct;
  correct << "P3\n2 2\n255\n";
  correct << "255 0 0 0 255 0 \n";
  correct << "0 0 255 255 255 255 \n";
  ASSERT_EQUAL(s.str(), correct.str());

  delete img; // delete the Image
}

// IMPLEMENT YOUR TEST FUNCTIONS HERE
// You are encouraged to use any functions from Image_test_helpers.h as needed.

// Tests that height and width members are correctly intialized.
TEST(test_image_init_basic){
  Image *img = new Image; // create an Image in dynamic memory
  Image_init(img, 3, 2);
  
  // Checks image invariants.  Checks width and height of image. 
  ASSERT_TRUE(0 < Image_width(img) && Image_width(img) <= MAX_MATRIX_WIDTH);  
  ASSERT_TRUE(0 < Image_height(img) && Image_height(img) <= MAX_MATRIX_HEIGHT);  
  ASSERT_EQUAL(Image_width(img), 3);
  ASSERT_EQUAL(Image_height(img), 2);
  
  // Checks width and height of matrices.
  ASSERT_EQUAL(Matrix_width(&img->red_channel), 3);
  ASSERT_EQUAL(Matrix_height(&img->red_channel), 2);
  ASSERT_EQUAL(Matrix_width(&img->green_channel), 3);
  ASSERT_EQUAL(Matrix_height(&img->green_channel), 2);
  ASSERT_EQUAL(Matrix_width(&img->blue_channel), 3);
  ASSERT_EQUAL(Matrix_height(&img->blue_channel), 2);

  delete img; // delete the Image
}

TEST(test_image_init_ppm_basic){
  Image *img = new Image; // create an Image in dynamic memory
  
  string input = "P3\n2 2\n255\n";
  input += "255 0 0 0 255 0 \n";
  input += "0 0 255 255 255 255 \n";
  istringstream is(input);
  Image_init(img, is);

  // Checks image invariants.  Checks width and height of image. 
  ASSERT_TRUE(0 < Image_width(img) && Image_width(img) <= MAX_MATRIX_WIDTH);  
  ASSERT_TRUE(0 < Image_height(img) && Image_height(img) <= MAX_MATRIX_HEIGHT);  
  ASSERT_EQUAL(Image_width(img), 2);
  ASSERT_EQUAL(Image_height(img), 2);

  Pixel red = {255, 0, 0};
  ASSERT_TRUE(Pixel_equal(Image_get_pixel(img, 0, 0), red));

  delete img; // delete the Image
}

// Tests that a pixel is correctly retrieved from a 1x1 image.
TEST(test_image_get_pixel_basic){
  Image *img = new Image; // create an Image in dynamic memory
  Image_init(img, 1, 1);
  Pixel fill_color;
  fill_color.r = 100;
  fill_color.g = 99;
  fill_color.b = 98;
  Image_fill(img, fill_color);
  
  const int row = 0;
  const int column = 0;
  ASSERT_TRUE(0 <= row && row < Image_height(img));
  ASSERT_TRUE(0 <= column && column < Image_width(img));
  ASSERT_TRUE(Pixel_equal(Image_get_pixel(img, row, column), fill_color));

  delete img; // delete the Image  
}
  
TEST(test_image_get_pixel_with_set_pixel){
  Image *img = new Image; // create an Image in dynamic memory
  
  Image_init(img, 2, 2);
  Pixel fill_color;
  fill_color.r = 100;
  fill_color.g = 99;
  fill_color.b = 98;
  Image_fill(img, fill_color);
  
  // Sets a unique pixel.
  Pixel unique_color;
  unique_color.r = 50;
  unique_color.g = 49;
  unique_color.b = 48;
  const int row = 0;
  const int column = 0;
  ASSERT_TRUE(0 <= row && row < Image_height(img));
  ASSERT_TRUE(0 <= column && column < Image_width(img));
  Image_set_pixel(img, row, column, unique_color);

  ASSERT_TRUE(Pixel_equal(Image_get_pixel(img, row, column), unique_color));

  delete img; // delete the Image
}

TEST(test_image_set_pixel_basic){
  Image *img = new Image; // create an Image in dynamic memory
  Image_init(img, 2, 2); 
  Pixel fill_color; 
  fill_color.r = 100; 
  fill_color.g = 99;
  fill_color.b = 98; 
  Image_fill(img, fill_color);
  
  // Sets a unique pixel at row 0 and column 0.
  Pixel unique_color;
  unique_color.r = 50;
  unique_color.g = 49;
  unique_color.b = 48;
  Image_set_pixel(img, 0, 0, unique_color);

  for (int r = 0; r < Image_height(img); ++r){
    for (int c = 0; c < Image_width(img); ++c){
      Pixel color = Image_get_pixel(img, r, c);
      if (r == 0 && c == 0){
        ASSERT_TRUE(Pixel_equal(color, unique_color));
      }else{
        ASSERT_TRUE(Pixel_equal(color, fill_color));
      }
    }
  }

  delete img; // delete the Image  
}

TEST(test_image_fiil_basic){
  Image *img = new Image; // create an Image in dynamic memory
  Image_init(img, 2, 2); 
  Pixel fill_color; 
  fill_color.r = 100; 
  fill_color.g = 99;
  fill_color.b = 98; 
  Image_fill(img, fill_color);
  
  for (int r = 0; r < Image_height(img); ++r){
    for (int c = 0; c < Image_width(img); ++c){
      Pixel color = Image_get_pixel(img, r, c);
      ASSERT_TRUE(Pixel_equal(color, fill_color)); 
    }
  }

  delete img; // delete the Image    
}

 
// NOTE: The unit test framework tutorial in Lab 2 originally
// had a semicolon after TEST_MAIN(). Although including and
// excluding the semicolon are both correct according to the c++
// standard, the version of g++ we are using reports a warning
// when there is a semicolon. Therefore do NOT add a semicolon
// after TEST_MAIN()
TEST_MAIN() // Do NOT put a semicolon here
