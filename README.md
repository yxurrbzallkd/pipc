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

```pipc::pipe_execute``` - pipes BLOCK FOREVER if you exceed pipe size - need to re-write that bit

```pipc::conveyor_execute``` - no piping for windows, only this feature - captures the output and appends it to the next command
