# Bevara filters

This is repository allows building existing accessors or creating new ones. By using the Bevara Access Develop IDE and/or creating or modifying any filters, you agree to be bound by the Developer's EULA as found at: https://bevara.com/terms_of_service/. You further agree to be bound by the underlying filter licenses as described in LICENSE.

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

Please refer to the [Bevara documentation](https://bevara.com/documentation/develop/) for support.
