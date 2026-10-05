# RDTTool

RDTTool can extract all the files from the .rdt archives for the PC version of G-Police.

## Usage

To use RDTTool copy the rdtttool binary into the main G-Police game directory can contains all the game files, Then run rdttool with the .rdr fie name:

~~~
rdttool res.rdr
~~~

It will then procede to extract all the files from the .rdt files listed in the .rdr file.

### Command Options

rdttool filename

rdttool [-hv]

Options:

-v  Display the version number of rdttool.

-h  Display this help.

Example:

Extract the files from all three .rdt files:

~~~
rdttool res.rdr
~~~

Output the version of rdttool:

~~~
rdttool -v
~~~

## Building:

### Linux:

To compile, clone the GPModTools repo:

~~~
git clone https://github.com/apemax/gpmodtools.git
~~~

Then cd into the `GPModTool/RDTTool` directory and run make:

~~~
cd GPModTools/rdttool
make
~~~