//
// William McPhail
// Computational Vision Homework 2
// Program 4 (Extra Credit)
//

#include "image.h"

#include <sstream>

#include <set>
#include <vector>
#include <map>
#include <cmath>

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
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

  //Compute Properties for Input File

  // Read the input image.
  Image an_image;
  if (!ReadImage(input_filename, &an_image))
  {
    cout << "Can't open file " << input_filename << endl;
    return;
  }

  //Declare variables
  vector<string> object_labels;
  map<int, double> center_row_positions;
  map<int, double> center_column_positions;
  map<int, double> object_areas;
  map<int, double> a_primes;
  map<int, double> b_primes;
  map<int, double> c_primes;
  map<int, double> minimum_moment_of_inertia;
  map<int, double> roundedness;
  map<int, double> orientation;
  set<int> unique_label_set;

  //Get unique labels
  for (size_t i = 0; i < an_image.num_rows(); ++i)
  {
    for (size_t j = 0; j < an_image.num_columns(); ++j)
    {
      if (an_image.GetPixel(i, j) != 0)
      {
        unique_label_set.insert(an_image.GetPixel(i, j));
      }
    }
  }

  //Compute centroids, object areas, and a', b', c'
  for (auto x : unique_label_set)
  {
    double center_row_position = 0;
    double center_column_position = 0;
    double object_area = 0;

    double a_prime = 0;
    double b_prime = 0;
    double c_prime = 0;
    for (int i = 0; i < an_image.num_rows(); ++i)
    {
      for (int j = 0; j < an_image.num_columns(); ++j)
      {
        if (an_image.GetPixel(i, j) == x)
        {
          center_row_position += i;
          center_column_position += j;
          object_area += 1;

          a_prime += i * i;
          b_prime += i * j;
          c_prime += j * j;
        }
      }
    }
    b_prime = 2 * b_prime;

    center_row_positions[x] = (center_row_position / object_area);
    center_column_positions[x] = (center_column_position / object_area);
    object_areas[x] = (object_area);

    a_primes[x] = a_prime;
    b_primes[x] = (b_prime);
    c_primes[x] = (c_prime);
  }

  //Compute minimum moment of inertia, roundedness, and orientation
  for (auto x : unique_label_set)
  {
    double a = a_primes[x] - center_row_positions[x] * center_row_positions[x] * object_areas[x];
    double b = b_primes[x] - 2 * center_row_positions[x] * center_column_positions[x] * object_areas[x];
    double c = c_primes[x] - center_column_positions[x] * center_column_positions[x] * object_areas[x];

    const double theta1 = atan2(b, a - c) / 2.0;
    const double e_min = a * sin(theta1) * sin(theta1) - b * sin(theta1) * cos(theta1) + c * cos(theta1) * cos(theta1);
    const double theta2 = theta1 + M_PI / 2.0;
    const double e_max = a * sin(theta2) * sin(theta2) - b * sin(theta2) * cos(theta2) + c * cos(theta2) * cos(theta2);
    const double roundness = e_min / e_max;

    minimum_moment_of_inertia[x] = (e_min);
    roundedness[x] = (roundness);
    orientation[x] = (theta1);
  }

  //Compare properties to input descriptions file
  ifstream inFile(input_descriptions_filename);
  if (!inFile)
  {
    cout << "Can't open file " << input_descriptions_filename << endl;
    return;
  }

  string line;
  while (getline(inFile, line))
  {
    istringstream iss(line);
    int label;
    double row, column, moment, area, round, orient;
    if (!(iss >> label >> row >> column >> moment >> area >> round >> orient))
    {
      break;
    }

    for (auto x : unique_label_set){

    // cout current values of loop : "comparing to" : current value of label
    // cout << "Moment: " << minimum_moment_of_inertia[x] << " : " << moment << endl;
    // cout << "Area: " << object_areas[x] << " : " << area << endl;
    // cout << "Round: " << roundedness[x] << " : " << round << endl;
    // cout << "Orient: " << orientation[x] << " : " << orient << endl;

      //using 20% area and 8% roundness as threshold
      if (abs(object_areas[x] - area) / area < 0.20 && abs(roundedness[x] - round) / round < 0.08)
      {
        // cout << "here" << endl;
        DrawLine(center_row_positions[x], center_column_positions[x], center_row_positions[x] + 30 * cos(orientation[x]), center_column_positions[x] + 30 * sin(orientation[x]), 255, &an_image);
      }
    }
  }

  // Write the output image.
  if (!WriteImage(output_filename, an_image))
  {
    cout << "Can't write to file " << output_filename << endl;
  }

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
