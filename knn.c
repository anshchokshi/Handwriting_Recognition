#include <stdio.h>
#include <math.h>    // Need this for sqrt()
#include <stdlib.h>
#include <string.h>

#include "knn.h"

/* Print the image to standard output in the pgmformat.  
 * (Use diff -w to compare the printed output to the original image)
 */
void print_image(unsigned char *image) {
    
    printf("P2\n %d %d\n 255\n", WIDTH, HEIGHT);
    for (int i = 0; i < NUM_PIXELS; i++) {
        printf("%d ", image[i]);
        if ((i + 1) % WIDTH == 0) {
            printf("\n");
        }
    }
}

/* Return the label from an image filename.
 * The image filenames all have the same format:
 *    <image number>-<label>.pgm
 * The label in the file name is the digit that the image represents
 */
unsigned char get_label(char *filename) {
    // Find the dash in the filename
    
    char *dash_char = strstr(filename, "-");
   
    // Convert the number after the dash into a integer

    return (char) atoi(dash_char + 1);
}

/* ******************************************************************
 * Complete the remaining functions below
 * ******************************************************************/


/* Read a pgm image from filename, storing its pixels
 * in the array img.
 * It is recommended to use fscanf to read the values from the file. First, 
 * consume the header information.  You should declare two temporary variables
 * to hold the width and height fields. This allows you to use the format string
 * "P2 %d %d 255 "
 * 
 * To read in each pixel value, use the format string "%hhu " which will store
 * the number it reads in an an unsigned character.
 * (Note that the img array is a 1D array of length WIDTH*HEIGHT.)
 */
void load_image(char *filename, unsigned char *img) {
    // Open corresponding image file, read in header (which we will discard)
    //printf("hi");
    FILE *f2 = fopen(filename, "r");
    if (f2 == NULL) {
        perror("fopen");
        exit(1);
    }
    int row;
    int width, height;
    unsigned char ch_int;
    
    while(getc(f2) != '\n');           // Ignore the first line in the input file

    
    fscanf(f2,"%d", &width);
    fscanf(f2,"%d", &height);
    int num_pixels = height * width;
    
    int temp;
    fscanf(f2, "%d", &temp); // ignoring the line with 255
    
    
    for (row=0; row < num_pixels; row++){ // iterating over all the pixels in the pgm image file.

        
            fscanf(f2,"%hhu", &ch_int);
            
            img[row]= ch_int;
        // saving the pixels in the img 1D array

    }
    
    fclose(f2);
}


/**
 * Load a full dataset into a 2D array called dataset.
 *
 * The image files to load are given in filename where
 * each image file name is on a separate line.
 * 
 * For each image i:
 *  - read the pixels into row i (using load_image)
 *  - set the image label in labels[i] (using get_label)
 * 
 * Return number of images read.
 */
int load_dataset(char *filename, 
                 unsigned char dataset[MAX_SIZE][NUM_PIXELS],
                 unsigned char *labels) {
    // We expect the file to hold up to MAX_SIZE number of file names
    FILE *f1 = fopen(filename, "r");
    if (f1 == NULL) {
        perror("fopen");
        exit(1);
    }

    int count = 0;
    
    char files[MAX_SIZE];
    while (fscanf(f1, "%s", files) != EOF) {  // iterating till the end of file
       
        load_image(files, dataset[count]);
        labels[count] = get_label(files);
        
        count++;
    }

    fclose(f1);
   
    return count;
}

/** 
 * Return the euclidean distance between the image pixels in the image
 * a and b.  (See handout for the euclidean distance function)
 */
double distance(unsigned char *a, unsigned char *b) {

    double sum = 0;
    for (int i = 0; i < NUM_PIXELS; i++) {
        double aval = (double)a[i];
        
        double bval = (double)b[i];
        double y = aval - bval;
        double x = pow((y), 2);
        
        sum = sum + x;
        
    }
    double dist = sqrt(sum);
    
    return dist;
    
}


/**
 * Return the most frequent label of the K most similar images to "input"
 * in the dataset
 * Parameters:
 *  - input - an array of NUM_PIXELS unsigned chars containing the image to test
 *  - K - an int that determines how many training images are in the 
 *        K-most-similar set
 *  - dataset - a 2D array containing the set of training images.
 *  - labels - the array of labels that correspond to the training dataset
 *  - training_size - the number of images that are actually stored in dataset
 * 
 * Steps
 *   (1) Find the K images in dataset that have the smallest distance to input
 *         When evaluating an image to decide whether it belongs in the set of 
 *         K closest images, it will only replace an image in the set if its
 *         distance to the test image is strictly less than all of the images in 
 *         the current K closest images.
 *   (2) Count the frequencies of the labels in the K images
 *   (3) Return the most frequent label of these K images
 *         In the case of a tie, return the smallest value digit.
 */

int helper_max_index(double a[], int n)
{
    /**
             Helper function that returns the index of the max value in the array.
             We use this in order to compare the max value with the current distance.
     */
    
    
    int k = 0;
    double max = a[k];
    
    for (int i = 0; i < n; ++i)
    {
        if ((double)a[i] > max)
        {
            max = (double)a[i];
            k = i;
        }
    }
    return k;
}

int helper_mostFrequent(int a[], int n)
{
   /**
    Helper function that first sorts the array of labels and then returns the most frequent label,
    we use descending order sorting because in the edge case where the frequencies of two
    labels are same we return the smaller one.
    took some help from the following code snippet in the second part of this function - https://stackoverflow.com/questions/8545590/find-the-most-popular-element-in-int-array.
    
    */
    for (int i = 0; i < n; ++i)
    {
        for (int j = i + 1; j < n; ++j)
        {
            if (a[i] < a[j])
            {
                int x = a[i];
                a[i] = a[j];
                
                a[j] = x;
                
            }
        }
    }
    
    int count = 1, tempCount;
      int frequent = a[0];
      int temp = 0;
      for (int i = 1; i < n; i++)
      {
        temp = a[i];
        tempCount = 0;
        for (int j = 0; j < n; j++)
        {
          if (temp == a[j]){
               tempCount++;
          }
           
        }
        
        if (tempCount >= count)
        {
          frequent = temp;
          count = tempCount;
          tempCount = 0;
        }
        
        
      }
      return frequent;
    
}



int knn_predict(unsigned char *input, int K,
                unsigned char dataset[MAX_SIZE][NUM_PIXELS],
                unsigned char *labels,
                int training_size) {
   
    double k_closest[K];
    
    int k_labels[K];
    
    for (int j = 0; j < K; j++) {  /** iteration used to populate the array with first k distance from the dataset, now the array size is K after the loop ends*/
        
        k_closest[j] = distance(dataset[j], input);
        
        k_labels[j] = labels[j];
        
    }
    
    for (int i = K; i < training_size; i++){ /** iteration for all the remaining images in the dataset, compare the each image distance with input and
                                                 replacing it if its strictly less than the max of the k_closest*/
       
        double count = 0;
        
        count = distance(dataset[i], input);
        
        int max = helper_max_index(k_closest,K);
       
        if (k_closest[max] > count){
                k_closest[max] = count;
            
                k_labels[max] = labels[i];
        }
        
    }
    /** the last step is finding the most frequent label in k_labels*/
    int final_label = helper_mostFrequent(k_labels, K);
    
    return final_label;
    
}


