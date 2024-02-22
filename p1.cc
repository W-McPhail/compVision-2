//
// <William McPhail>
// Computational Vision Homework 2
// Program 1
//

#include "image.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;
using namespace ComputerVisionProjects;



 // @brief Implementation of Task Program 1 to convert to binary image given a threshold.
 //  
 // @param input_filename the name of the input image
 // @param threshold the threshold value
 // @param output_filename the name of the output image
void ConvertToBinary(const string &input_filename, int threshold, const string &output_filename) {
  cout << "Converting to Binary Image" << endl;
  cout << "Input filename: " << input_filename << endl;
  cout << "Threshold: " << threshold << endl;
  cout << "Output filename: " << output_filename << endl;

  // Read the input image.
  Image an_image;
  if (!ReadImage(input_filename, &an_image)) {
    cout << "Can't open file " << input_filename << endl;
    return;
  }

  // Convert to binary image.
  for (size_t i = 0; i < an_image.num_rows(); ++i) {
    for (size_t j = 0; j < an_image.num_columns(); ++j) {
      if (an_image.GetPixel(i, j) < threshold) {
        an_image.SetPixel(i, j, 0);
      } else {
        an_image.SetPixel(i, j, 255);
      }
    }
  }

  // Write the output image.
  if (!WriteImage(output_filename, an_image)) {
    cout << "Can't write to file " << output_filename << endl;
  }

  cout << "Done" << endl;

}

int main(int argc, char **argv){  
  if (argc!=4) {
    printf("Usage: %s input_file threshold output_file\n", argv[0]);
    return 0;
  }
  
  const string input_filename(argv[1]);
  const int threshold = atoi(argv[2]);
  const string output_filename(argv[3]);

  ConvertToBinary(input_filename, threshold, output_filename);

  return 0;
}
