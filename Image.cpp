#include <cassert>
#include <string>
#include "Image.h"

using namespace std;

// REQUIRES: img points to an Image
//           0 < width && width <= MAX_MATRIX_WIDTH
//           0 < height && height <= MAX_MATRIX_HEIGHT
// MODIFIES: *img
// EFFECTS:  Initializes the Image with the given width and height.
// NOTE:     Do NOT use new or delete here.
void Image_init(Image* img, int width, int height) {
  assert(0 < width && width <= MAX_MATRIX_WIDTH);
  assert(0 < height && height <= MAX_MATRIX_HEIGHT);
  img->width = width;
  img->height = height;
  Matrix_init(&img->red_channel, width, height);
  Matrix_init(&img->green_channel, width, height);
  Matrix_init(&img->blue_channel, width, height);
}

// REQUIRES: img points to an Image
//           is contains an image in PPM format without comments
//           (any kind of whitespace is ok)
// MODIFIES: *img
// EFFECTS:  Initializes the Image by reading in an image in PPM format
//           from the given input stream.
// NOTE:     See the project spec for a discussion of PPM format.
// NOTE:     Do NOT use new or delete here.
void Image_init(Image* img, std::istream& is) {
  // Checks that the input is a plain ppm file.
  string is_valid_ppm;
  const string plain_ppm_file = "P3";
  is >> is_valid_ppm;
  assert(is_valid_ppm == plain_ppm_file); 
  
  // Intializes Image width and height.
  string width_str;
  string height_str;
  is >> width_str;
  is >> height_str;
  Image_init(img, stoi(width_str), stoi(height_str));

  // Checks max pixel intensity is equal to MAX_INTENSITY.
  string max_value_str;
  is >> max_value_str;
  assert(stoi(max_value_str) == MAX_INTENSITY);

  // Intializes a Pixel and then sets it as the color for a given row and column.
  Pixel color;
  for (int row = 0; row < Image_height(img); ++row){
    for (int column = 0; column < Image_width(img); ++column){
      string r;
      string g;
      string b;
      is >> r;
      is >> g;
      is >> b;
      color.r = stoi(r);
      color.g = stoi(g);
      color.b = stoi(b);
      Image_set_pixel(img, row, column, color); 
    }
  }
}

// REQUIRES: img points to a valid Image
// EFFECTS:  Writes the image to the given output stream in PPM format.
//           You must use the kind of whitespace specified here.
//           First, prints out the header for the image like this:
//             P3 [newline]
//             WIDTH [space] HEIGHT [newline]
//             255 [newline]
//           Next, prints out the rows of the image, each followed by a
//           newline. Each pixel in a row is printed as three ints
//           for its red, green, and blue components, in that order. Each
//           int is followed by a space. This means that there will be an
//           "extra" space at the end of each line. See the project spec
//           for an example.
void Image_print(const Image* img, std::ostream& os) {
  const int height = Image_height(img);
  const int width = Image_width(img);
  os << "P3" << endl << width << " " << height << endl << "255" << endl;
  for (int r = 0; r < height; ++r){
    for (int c = 0; c < width; ++c){
      Pixel color = Image_get_pixel(img, r, c);
      os << color.r << " " << color.g << " " << color.b << " ";
    }
    os << endl;
  }  
}

// REQUIRES: img points to a valid Image
// EFFECTS:  Returns the width of the Image.
int Image_width(const Image* img) {
  return img->width;
}

// REQUIRES: img points to a valid Image
// EFFECTS:  Returns the height of the Image.
int Image_height(const Image* img) {
  return img->height;
}

// REQUIRES: img points to a valid Image
//           0 <= row && row < Image_height(img)
//           0 <= column && column < Image_width(img)
// EFFECTS:  Returns the pixel in the Image at the given row and column.
Pixel Image_get_pixel(const Image* img, int row, int column) {
  assert(0 <= row && row < Image_height(img));
  assert(0 <= column && column < Image_width(img));
  Pixel color;
  color.r = *Matrix_at(&img->red_channel, row, column);
  color.g = *Matrix_at(&img->green_channel, row, column);
  color.b = *Matrix_at(&img->blue_channel, row, column);
  return color;    
}

// REQUIRES: img points to a valid Image
//           0 <= row && row < Image_height(img)
//           0 <= column && column < Image_width(img)
// MODIFIES: *img
// EFFECTS:  Sets the pixel in the Image at the given row and column
//           to the given color.
void Image_set_pixel(Image* img, int row, int column, Pixel color) {
  assert(0 <= row && row < Image_height(img));
  assert(0 <= column && column < Image_width(img));
  *Matrix_at(&img->red_channel, row, column) = color.r;
  *Matrix_at(&img->green_channel, row, column) = color.g;
  *Matrix_at(&img->blue_channel, row, column) = color.b;
}

// REQUIRES: img points to a valid Image
// MODIFIES: *img
// EFFECTS:  Sets each pixel in the image to the given color.
void Image_fill(Image* img, Pixel color) {
  for (int r = 0; r < Image_height(img); ++r){
    for (int c = 0; c < Image_width(img); ++c){
      *Matrix_at(&img->red_channel, r, c) = color.r;
      *Matrix_at(&img->green_channel, r, c) = color.g;
      *Matrix_at(&img->blue_channel, r, c) = color.b;  
    }
  }
}
