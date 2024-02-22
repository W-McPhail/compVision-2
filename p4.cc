//
// <YOUR NAME>
// Computational Vision Homework 2
// Program 4 (Extra Credit)
//

#include "image.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;
using namespace ComputerVisionProjects;

// @brief Compute object properties
// @param input_filename the name of the input labeled image
// @param input_descriptions_filename the name of the input descriptions file (txt)
// @param output_image_filename the name of the output image
void FindLabeledObject(const string &input_filename, const string &input_descriptions_filename, const string &output_filename) {
  cout << "Compute properties" << endl;
  cout << "Input labeled image filename: " << input_filename << endl;
  cout << "input descriptions filename: " << input_descriptions_filename << endl;
  cout << "Output image filename: " << output_filename << endl;
  // .. Code that calls other functions
}

int main(int argc, char **argv){
  
  if (argc!=4) {
    printf("Usage: %s {input labeled image} {input descriptions file} {output image}\n", argv[0]);
    return 0;
  }
  
  const string input_filename(argv[1]);
  const string input_descriptions_filename(argv[2]);
  const string output_filename(argv[3]);

  FindLabeledObject(input_filename, input_descriptions_filename, output_filename);
  
  return 0;
}
