# Bevara filters

This is repository allows building existing accessors or create new ones. 

## Build

make a directory to start building the project (for instance "build")

Type: 
../configure

Then :
make (-j8)

The accessors should be available as a set of wasm files that can be distributed using the _with_ attributes of any universal tags.

## Creating new filters

Simply call the add_filter function in the CMakeList.txt at the root of this repository. Refers to the [Cmake Documentation](https://cmake.org/documentation/) to get more information about CMake compilation environment.

Arguments of the add_filter are :
add_filter(
    name_of_the_accessor
    source_files
    entry_functions
    compilation_flags
    include_directories
    link_flags
    version_number
)

Please refers to the [Bevara documentation](https://bevara.com/documentation/develop/) to get support.