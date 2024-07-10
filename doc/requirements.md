# Overview

The objective is to create a C Library allowing to perform 2D measurements on a measurement buffer. It shall contain the following functions:
````
MD* MD_create (const unsigned char* s, int width, int height);

void MD_delete (MD*);

MD* MD_copy (const MD*); /* copy constructor */

unsigned int MD_getSum (const MD*, int x0, int y0, int x1, int y1);

double MD_getAverage (const MD*, int x0, int y0, int x1, int y1);
````

## Functional Requirements

- The buffer _s_ will:

  - never contain more than 4096x4096 values
  - be organized linearly with (0,0) at beginning, and _width_ defining the offest for the beginning of the next line:
  ````
    (0,0)
    (width, 0)
    (2*width, 0)
    ...
  ````
- The buffer window defined via _x0, y0, x1, y1_:

  - shall be inclusive
  - shall allow for cases where x0 >= x1 and y0 >= y1, x0 < x1 and y0 < y1, and combinations of the two.
  - can extend beyond the buffers bounds. Out-of-bounds values shall be treated as 0.

- _MD_create_ is used to setup the initial 8-bit two-dimensional buffer. This function:
  
  - is not performance citical.
  - creates a copy of the provided input buffer _s_.

- _MD_delete_ shall release any resources allocated with _MD_create_.
- _MD_copy_ shall serve as copy constructor, creating a deep copy of the measurement data.
- _MD_getSum_ shall get the sum for the values contained in the provided buffer window.
- _MD_getAverage_ shall get the average value for the values contained in the provided buffer window.
- _MD_getSum_ and _MD_getAverage_ are performance critical. The goal is to achieve O(1) performance.

## Quality Requirements

- Unit Testing provided, including edge-cases.
- Robust operation without crashes or memory leaks.
- Comments for non-obvious code.
- Code is optimized to provide O(1) performance.
- Code is well readable.
- Only C Standard Library used.
