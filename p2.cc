//
// <William McPhail>
// Computational Vision Homework 2
// Program 2 : Sequential Labeling of Binary Image & Changing Grayscale
// Date of Creation: 2/21/2024
//

#include "image.h"
#include "DisjSets.h"
#include <unordered_map>
#include <set>
#include <algorithm>

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;
using namespace ComputerVisionProjects;

// @brief Implementation of sequential labeling algorithm
// @param input_filename the name of the input image
// @param output_filename the name of the output image
void PerformSequentialLabeling(const string &input_filename, const string &output_filename)
{
  cout << "Sequential labeling" << endl;
  cout << "Input filename: " << input_filename << endl;
  cout << "Output filename: " << output_filename << endl;
  // .. Code that calls other functions

  // Read the input image.
  Image an_image;
  if (!ReadImage(input_filename, &an_image))
  {
    cout << "Can't open file " << input_filename << endl;
    return;
  }

  // Perform Sequential Labeling
  DisjSets ds(an_image.num_rows() * an_image.num_columns());

  int label = 1;
  vector<int> labels(an_image.num_rows() * an_image.num_columns(), 0);

  // Loop through the image and assign labels
  for (size_t i = 0; i < an_image.num_rows(); ++i)
  {
    for (size_t j = 0; j < an_image.num_columns(); ++j)
    {
      if (an_image.GetPixel(i, j) == 255)
      {
        int current_set = labels[i * an_image.num_columns() + j];

        // if i == 0 & j == 0
        if (i == 0 && j == 0 && an_image.GetPixel(i, j) == 255)
        {
          labels[i * an_image.num_columns() + j] = label++;
          continue;
        }
        else
        {
          // row 0, left only
          if (i == 0 && an_image.GetPixel(i, j - 1) == 255)
          {
            labels[i * an_image.num_columns() + j] = labels[i * an_image.num_columns() + j - 1];
            continue;
          }
          // col 0, upper only
          if (i == 0 && an_image.GetPixel(i, j - 1) == 0)
          {
            labels[i * an_image.num_columns() + j] = label++;
            continue;
          }
          // row 0, left only
          else if (j == 0 && an_image.GetPixel(i - 1, j) == 255)
          {
            labels[i * an_image.num_columns() + j] = labels[(i - 1) * an_image.num_columns() + j];
            continue;
          }
          // left w/o label 
          else if (j == 0 && an_image.GetPixel(i - 1, j) == 0)
          {
            labels[i * an_image.num_columns() + j] = label++;
            continue;
          }
          // regular diagonal
          else if (an_image.GetPixel(i - 1, j - 1) == 255)
          {
            labels[i * an_image.num_columns() + j] = labels[(i - 1) * an_image.num_columns() + j - 1];
            continue;
          }
          // top & left
          else if (an_image.GetPixel(i - 1, j) == 255 && an_image.GetPixel(i, j - 1) == 255)
          {
            int f1 = ds.find(labels[(i - 1) * an_image.num_columns() + j]);
            int f2 = ds.find(labels[i * an_image.num_columns() + j - 1]);
            if (f1 == f2)
            {
              labels[i * an_image.num_columns() + j] = std::min(f1, f2);
            }
            else
            {
              labels[i * an_image.num_columns() + j] = std::min(f1, f2);
              ds.unionSets(f1, f2);
            }
            continue;
          }
          // regular upper
          else if (an_image.GetPixel(i, j - 1) == 255)
          {
            labels[i * an_image.num_columns() + j] = labels[i * an_image.num_columns() + j - 1];
            continue;
          }
          // regular left
          else if (an_image.GetPixel(i - 1, j) == 255)
          {
            labels[i * an_image.num_columns() + j] = labels[(i - 1) * an_image.num_columns() + j];
            continue;
          }
          // both 0, assign label
          else if (an_image.GetPixel(i - 1, j) == 0 && an_image.GetPixel(i, j - 1) == 0)
          {
            labels[i * an_image.num_columns() + j] = label++;
          }
        }
      }
    }
  }

  // Assign unique grayscale values to each connected component
  set<int> label_set;

  for (size_t i = 0; i < an_image.num_rows(); ++i)
  {
    for (size_t j = 0; j < an_image.num_columns(); ++j)
    {
      if (an_image.GetPixel(i, j) == 255)
      {
        int current_set = ds.find(labels[i * an_image.num_columns() + j]);
        if (current_set != 0){
            label_set.insert(current_set);
        }

        int index = 1;
        for (auto x: label_set)
        {
          if (x == current_set)
          {
            break;
          }
          index++;
        }
        an_image.SetPixel(i, j, (index * 30));
      }
    }
  }

  // Write the output image.
  if (!WriteImage(output_filename, an_image))
  {
    cout << "Can't write to file " << output_filename << endl;
  }

  cout << "Done" << endl;
}

int main(int argc, char **argv)
{

  if (argc != 3)
  {
    printf("Usage: %s input_file output_file\n", argv[0]);
    return 0;
  }

  const string input_filename(argv[1]);
  const string output_filename(argv[2]);

  PerformSequentialLabeling(input_filename, output_filename);

  return 0;
}
