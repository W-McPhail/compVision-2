//
// <William McPhail>
// Computational Vision Homework 2
// Program 3
// Date of Creation: 2/22/2024
//

#include "image.h"

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
// @param output_descriptions_filename the name of the output descriptions file (txt)
// @param output_image_filename the name of the output image
void ComputeProperties(const string &input_filename, const string &output_descriptions_filename, const string &output_image_filename)
{
  cout << "Compute properties" << endl;
  cout << "Input labeled image filename: " << input_filename << endl;
  cout << "Output descriptions filename: " << output_descriptions_filename << endl;
  cout << "Output image filename: " << output_image_filename << endl;
  // .. Code that calls other functions

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

    // cout << "Center row position: " << center_row_position / object_area << endl;
    // cout << "Center column position: " << center_column_position / object_area << endl;
    // cout << "Object area: " << object_area << endl;

    //a_primes.push_back(a_prime);
    a_primes[x] = a_prime;
    b_primes[x] = (b_prime);
    c_primes[x] = (c_prime);

    // cout << "a prime: " << a_prime << endl;
    // cout << "b prime: " << b_prime << endl;
    // cout << "c prime: " << c_prime << endl;
  }

  //Compute minimum moment of inertia, roundedness, and orientation
  for (auto x : unique_label_set)
  {
    // cout << a_primes[x] << " " << b_primes[x] << " " << c_primes[x] << endl;
    // cout << center_row_positions[x] << " " << center_column_positions[x] << " " << object_areas[x] << endl;

    double a = a_primes[x] - center_row_positions[x] * center_row_positions[x] * object_areas[x];
    double b = b_primes[x] - 2 * center_row_positions[x] * center_column_positions[x] * object_areas[x];
    double c = c_primes[x] - center_column_positions[x] * center_column_positions[x] * object_areas[x];

    // cout << a << " " << b << " " << c << endl;
    const double theta1 = atan2(b, a - c) / 2.0;
    const double e_min = a * sin(theta1) * sin(theta1) - b * sin(theta1) * cos(theta1) + c * cos(theta1) * cos(theta1);
    const double theta2 = theta1 + M_PI / 2.0;
    const double e_max = a * sin(theta2) * sin(theta2) - b * sin(theta2) * cos(theta2) + c * cos(theta2) * cos(theta2);
    const double roundness = e_min / e_max;

    minimum_moment_of_inertia[x] = (e_min);
    roundedness[x] = (roundness);
    orientation[x] = (theta1);

    // cout << "Minimum moment of inertia: " << e_min << endl;
    // cout << "Roundedness: " << roundness << endl;
    // cout << "Orientation: " << theta1 << endl;

    // cout << "a" << a << endl;
    // cout << "b" << b << endl;
    // cout << "c" << c << endl;
  }

  //draw centroid & orientation line
  for (auto x : unique_label_set)
  {
    DrawLine(center_row_positions[x], center_column_positions[x], center_row_positions[x] + 30 * cos(orientation[x]), center_column_positions[x] + 30 * sin(orientation[x]), 255, &an_image);
  }


  // eniac testing : scp - r dir1 william.mcphail98@meniac.cus.hunter.cunt.edu:~/
  string output_descriptions = output_descriptions_filename;

  ofstream outFile(output_descriptions);
  for (auto x : unique_label_set) {
    outFile << std::fixed << x << " " << center_row_positions[x] << " " << center_column_positions[x] << " " << minimum_moment_of_inertia[x]
     << " " << object_areas[x] << " " << roundedness[x] << " " << (orientation[x] * 180) / M_PI << endl;
  }

  // Write the output image.
  if (!WriteImage(output_image_filename, an_image))
  {
    cout << "Can't write to file " << output_image_filename << endl;
  }

}

int main(int argc, char **argv)
{

  if (argc != 4)
  {
    printf("Usage: %s input_labeled_image output_descriptions(txt) output_image\n", argv[0]);
    return 0;
  }

  const string input_filename(argv[1]);
  const string output_descriptions_filename(argv[2]);
  const string output_image_filename(argv[3]);

  ComputeProperties(input_filename, output_descriptions_filename, output_image_filename);

  return 0;
}
