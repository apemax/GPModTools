# TEXtoBMP

TEXtoBMP can convert all the .tex/.pal/.bmp texture files from the PC version of G-Police to .bmp files.

### TODO

* Some .tex/.pal/.bmp files aren't converting correctly. Needs further investigation.

## Usage

To use TEXtoBMP copy the textobmp binary into the same directory as the extracted .tex/.pal/.bmp files you want to convert, Then run textobmp with the .tex file name:

~~~
textobmp filename.tex
~~~

It will then procede to convert the .tex/.pal/.bmp files to a .bmp file that can be viewed in any image viewer.

### Command Options

textobmp filename

textobmp [-hv]

Options:

-v  Display the version number of textobmp.

-h  Display this help.

Example:

Convert .tex/.pal/.bmp files to .bmp file:

~~~
textobmp filename.tex
~~~

Output the version of textobmp:

~~~
textobmp -v
~~~

## Building:

### Linux:

To compile, clone the GPModTools repo:

~~~
git clone https://github.com/apemax/gpmodtools.git
~~~

Then cd into the `GPModTool/textobmp` directory and run make:

~~~
cd GPModTools/textobmp
make
~~~