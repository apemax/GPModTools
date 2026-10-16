# IMGtoBMP

IMGtoBMP can convert all the .img files from the PC version of G-Police to .bmp files.

### TODO

* Correct colours from RGB to BMP standard BGR.
* Figure out the resolution of the DescMask1.img so it can be converted correctly.

## Usage

To use IMGtoBMP copy the imgtobmp binary into the main G-Police game directory that contains all the game files, Then run imgtobmp with the .img file name:

~~~
imgtobmp filename.img
~~~

It will then procede to convert the .img file to a .bmp file that can be viewed in any image viewer.

### Command Options

imgtobmp filename

imgtobmp [-hv]

Options:

-v  Display the version number of imgtobmp.

-h  Display this help.

Example:

Convert .img file to .bmp file:

~~~
imgtobmp filename.img
~~~

Output the version of imgtobmp:

~~~
imgtobmp -v
~~~

## Building:

### Linux:

To compile, clone the GPModTools repo:

~~~
git clone https://github.com/apemax/gpmodtools.git
~~~

Then cd into the `GPModTool/imgtobmp` directory and run make:

~~~
cd GPModTools/imgtobmp
make
~~~