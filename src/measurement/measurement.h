#ifndef MEASUREMENT_H
#define MEASUREMENT_H

#ifdef __cplusplus
extern "C"
{
#endif
  /**
   * Measurement data structure
   */
  typedef struct MD
  {
    /** width of 2D data buffer - maximum value 4096 */
    unsigned int width;

    /** height of 2D data buffer - maximum value 4096 */
    unsigned int height;

    /** Preprocessed 2D data buffer stored in linear fashion in memory
     *
     * The maximum value stored in a single entry, if the input buffer contains
     * the maximum value 0xff (255) in every 8-bit entry, would be:
     * 4096 * 4096 * 255 < UINT_MAX
     *
     */
    unsigned int *buffer;
  } MD;

  /** Allocates resources for measurement data structure.
   *
   * @param s - pointer to 8-bit 2D measurement buffer
   * @param width - width of measurement buffer (x dimension). Must be <= 4096
   * @param height - height of measurement buffer (y dimension). Must be <= 4096
   *
   * @note Data will be pre-processed.
   *
   * @return pointer to measurement data structure or NULL for invalid input
   */
  MD *MD_create(const unsigned char *s, int width, int height);

  /** Frees all resources associated with measurement data structure
   *
   * @param MD - pointer to measurement data structure
   *
   * @attention Memory is freed after this call - don't use pointer anymore
   */
  void MD_delete(MD *);

  /** Copies all resources associated with measurement data structure
   *
   * @param MD - pointer to measurement data structure
   *
   * @return pointer to deep copy of measurement data structure
   */
  MD *MD_copy(const MD *); /* copy constructor */

  /**
   * @param MD - pointer to measurement data structure
   * @param x0 - measurement window first x coordinate (inclusive)
   * @param y0 - measurement window first y coordinate (inclusive)
   * @param x1 - measurement window second x coordinate (inclusive)
   * @param y1 - measurement window second y coordinate (inclusive)
   *
   * @return the sum of the values included in the measurement window
   */
  unsigned int MD_getSum(const MD *, int x0, int y0, int x1, int y1);

  /**
   * @param MD - pointer to measurement data structure
   * @param x0 - measurement window first x coordinate (inclusive)
   * @param y0 - measurement window first y coordinate (inclusive)
   * @param x1 - measurement window second x coordinate (inclusive)
   * @param y1 - measurement window second y coordinate (inclusive)
   *
   * @return the average of the values included in the measurement window
   */
  double MD_getAverage(const MD *, int x0, int y0, int x1, int y1);

#ifdef __cplusplus
}
#endif

#endif