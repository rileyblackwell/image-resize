#include "Matrix.h"
#include "Image.h"
#include "processing.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;


int main(int argc, char *argv[]){
    Image *img = new Image; // create an Image in dynaimc memory

    if (!(argc == 4 || argc == 5)){
        cout << "Usage: resize.exe IN_FILENAME OUT_FILENAME WIDTH [HEIGHT]\n"
        << "WIDTH and HEIGHT must be less than or equal to original" << endl;
        return 1;
    }
    string input_filename = argv[1];
    ifstream fin;
    fin.open(input_filename);
    if (!fin.is_open()) {
        cout << "Error opening file: " << input_filename << endl;
        return 1;
    }
    Image_init(img, fin);
    fin.close();

    string new_width_str = argv[3];
    int new_width = stoi(new_width_str);
    if (new_width > Image_width(img)){
        cout << "Usage: resize.exe IN_FILENAME OUT_FILENAME WIDTH [HEIGHT]\n"
        << "WIDTH and HEIGHT must be less than or equal to original" << endl;
        return 1;    
    }
    
    if (argc == 4){
        seam_carve_width(img, new_width); 
    }else if (argc == 5){
        string new_height_str = argv[4];
        int new_height = stoi(new_height_str);
        if (new_height > Image_height(img)){
        cout << "Usage: resize.exe IN_FILENAME OUT_FILENAME WIDTH [HEIGHT]\n"
        << "WIDTH and HEIGHT must be less than or equal to original" << endl;
        return 1;    
        }
        seam_carve(img, new_width, new_height);
    }
    
    string output_filename = argv[2];
    ofstream fout;
    fout.open(output_filename);
    if (!fout.is_open()) {
        cout << "Error opening file: " << output_filename << endl;
        return 1;
    }
    Image_print(img, fout);
    fout.close();
    
    delete img; // delete the image
    
    return 0;
}