#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "tools/logout.h"
#include "measurement.h"

#define MAX_BUFFER_WIDTH 4096
#define MAX_BUFFER_HEIGHT 4096

/**
 * @return the number of values in an inclusive input window.
 */
inline static unsigned int getNumWindowValues(int x0, int y0, int x1, int y1)
{
  return ((abs(x1 - x0) + 1) * (abs(y1 - y0) + 1));
}

/**
 * @return pointer to allocated memory for MD structure
 */
inline static MD *reserveMDMemory(int bufferSize)
{
  MD *const newMd = malloc(sizeof(struct MD));
  newMd->buffer = (unsigned int *)malloc(bufferSize);
  return newMd;
}

/**
 * @return size of preprocessed measurement buffer
 */
inline static unsigned int getMDBufferSize(int width, int height)
{
  return (width) * (height) * sizeof(unsigned int);
}

/**
 * @return a preprocessed MD buffer
 *
 * @details For each field in the 8-bit input buffer, an unsigned int sum is stored in the output buffer. The algorithm
 * to calculate the sum uses the value of the current input field, and the previously calculated sums. It:
 * 1) Takes the value from the current field of the input buffer (at index i)
 * 2) Adds the sum stored in the field left of the current field if available (at index i - 1)
 * 3) Adds the sum stored in the field above the current field if available (at index i - width)
 * 4) Subtracts the sum stored in the field above-left the current field if available (at index i - width - 1)
 *
 * Below you see this depicted for an array for which we process field (3, 2). a means add, s means subtract and
 * c means current. x are just adjacent values we don't use for this iteration:
 *
 *   0 1 2 3
 * 0 x x x x
 * 1 x x s a
 * 2 x x a c
 *
 * a(2,2) contains the sum of the window between (0,0) and (2,2), a(3,1) contains the sum of the window between (0,0)
 * and (3,1). Since both a windows overlap, s(2,1) must be subtracted once in the end - else it would count twice.
 */
inline static unsigned int *preprocessMDInputBuffer(unsigned int *output, const unsigned char *input, int width, int height)
{
  LOUT("preprocessMDInputBuffer: with width=%d and height=%d\n", width, height);
  int y = 0;
  int x = 0;
  int i = 0;
  for (; y < height; ++y)
  {
    for (x = 0; x < width; ++x, ++i)
    {
      LOUT("\npreprocessMDInputBuffer: add input from (%d, %d) = %d ; index = %d \n", x, y, input[i], i);
      output[i] = input[i];

      if (x > 0)
      {
        LOUT("preprocessMDInputBuffer: add output from (%d, %d) = %d\n", x - 1, y, output[i - 1]);
        output[i] += output[i - 1];
      }

      if (y > 0)
      {
        LOUT("preprocessMDInputBuffer: add output from (%d, %d) = %d\n", x, y - 1, output[i - width]);
        output[i] += output[i - width];
      }

      if (x > 0 && y > 0)
      {
        LOUT("preprocessMDInputBuffer: remove output from (%d, %d) = %d\n", x - 1, y - 1, output[i - width - 1]);
        output[i] -= output[i - width - 1];
      }

      LOUT("preprocessMDInputBuffer: output(%d, %d) = %d\n", x, y, output[i]);
    }
  }
  return NULL;
}

inline static void swapInt(int *left, int *right)
{
  int oldLeft = *left;
  *left = *right;
  *right = oldLeft;
}

inline static int cropInt(int *value, int min, int max)
{
  *value = *value < min ? min : (*value > max ? max : *value);
}

inline static bool isOutOfBounds(int x0, int y0, int x1, int y1, int width, int height)
{
  return (x0 < 0 && x1 < 0 || y0 < 0 && y1 < 0 || x0 >= width && x1 >= width || y0 >= height && y1 >= height);
}

MD *MD_create(const unsigned char *s, int width, int height)
{
  if (width > MAX_BUFFER_WIDTH || height > MAX_BUFFER_HEIGHT)
  {
    return NULL;
  }
  const unsigned int bufferSize = getMDBufferSize(width, height);
  MD *newMd = reserveMDMemory(bufferSize);
  preprocessMDInputBuffer(newMd->buffer, s, width, height);
  newMd->width = width;
  newMd->height = height;
  return newMd;
}

void MD_delete(MD *md)
{
  if (md != NULL)
  {
    free(md->buffer);
    free(md);
  }
}

MD *MD_copy(const MD *md)
{
  const unsigned int bufferSize = getMDBufferSize(md->width, md->height);
  MD *newMd = reserveMDMemory(bufferSize);
  memcpy(newMd->buffer, md->buffer, bufferSize);
  newMd->width = md->width;
  newMd->height = md->height;
  return newMd;
}

unsigned int MD_getSum(const MD *md, int x0, int y0, int x1, int y1)
{
  LOUT("\nMD_getSum: get Sum from (%d, %d, %d, %d)\n", x0, y0, x1, y1);
  if (isOutOfBounds(x0, y0, x1, y1, md->width, md->height))
  {
    return 0;
  }

  if (x0 > x1)
  {
    swapInt(&x0, &x1);
  }
  if (y0 > y1)
  {
    swapInt(&y0, &y1);
  }

  // crop window at bounds
  cropInt(&x0, 0, md->width - 1);
  cropInt(&x1, 0, md->width - 1);
  cropInt(&y0, 0, md->height - 1);
  cropInt(&y1, 0, md->height - 1);

  // lower right sum
  unsigned int sum = md->buffer[x1 + y1 * md->width];

  LOUT("\nMD_getSum: Add value at (%d, %d) = %d\n", x1, y1, sum);

  // subtract rectangle left of window if exists
  if (x0 > 0)
  {
    sum -= md->buffer[x0 - 1 + y1 * md->width];

    LOUT("\nMD_getSum: Subtract value at (%d, %d) results in %d\n", x0 - 1, y1, sum);
  }

  // subtract rectangle top of window if exists
  if (y0 > 0)
  {
    sum -= md->buffer[x1 + (y0 - 1) * md->width];

    LOUT("\nMD_getSum: Subtract value at (%d, %d) results in %d\n", x1, (y0 - 1), sum);
  }

  // add rectangle top left of window if exists - because if it exists we subtracted it
  // twice by subtracting top and left window
  if (y0 > 0 && x0 > 0)
  {
    sum += md->buffer[x0 - 1 + (y0 - 1) * md->width];

    LOUT("\nMD_getSum: Add value at (%d, %d) results in %d\n", x0 - 1, (y0 - 1), sum);
  }

  return sum;
}

double MD_getAverage(const MD *md, int x0, int y0, int x1, int y1)
{
  const unsigned int sum = MD_getSum(md, x0, y0, x1, y1);
  return sum / getNumWindowValues(x0, y0, x1, y1);
}