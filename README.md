# lib

## Usage
no installation available yet, simply paste your code into the demo.cpp, use CMakeLists.txt

use ``` -G "Unix Makefiles"``` on Windows

## Testing

cd windows-tests or linux-tests

```./test.sh```

## Usage

edit the CMakeLists.txt adding your main file and include directories

## Presentation
[link](https://docs.google.com/presentation/d/1WqENRvvxLhshr_XD4suWVhSl_fW7-LFJCP1Yb7i2hhM/edit?usp=sharing)

## Issues & Features

- nonblocking pipes

```pipc::pipe_execute``` - pipes BLOCK FOREVER if you exceed pipe size - need to re-write that bit

- ```pipc::cascade_execute```

no piping for windows, only this feature - captures the output and appends it to the next command

- problem with my error system

All error codes are negative powers of two and no two are allike. I wanted all returned errors to have prefixes corresponding to where they came from (-2^30 - process errors, -2^29 - fifo etc.) But this way we quickly run out of errors (not yet, but will happen). This is the first thing that has to be fixed

- background execution

there is a <pipc/process/background.hpp>, which is supposed to launch a process in the background - doesn't work, yet, creates two processes for some reason

