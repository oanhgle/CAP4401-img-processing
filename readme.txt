This software is architectured as follows. This software can work on grad server.

iptools -This folder hosts the files that are compiled into a static library. 
	image - This folder hosts the files that define an image.
	utility- this folder hosts the files that students store their implemented algorithms.
	
lib- This folder hosts the static libraries associated with this software.

project- This folder hosts the files that will be compiled into executables.
	bin- This folder hosts the binary executables created in the project directory.



*** INSTALATION ***

On Linux

Enter the project directory in terminal and run make

As a result you should get iptool in project/bin directory.

*** FUNCTIONS ***

1. Add intensity: add
Increase the intensity for a gray-level image.

2. Binarization: binarize
Binarize the pixels with the threshold.

3. Scaling: scale
Reduce or expand the heigh and width with two scale factors.
Scaling factor = 2: double height and width of the input image.
Scaling factor = 0.5: half height and width of the input image.

4. Increase: increase brightness/intensity of the pixel with the 
intensity larger than the threshold in a certain region. 
Given a square image area defined by upper-left corner (X1,Y1) 
and D - the size of the square area, increase image intensity by 
intensity value V only in this specified square window. The rest of 
the image remains unchanged. 

*** PARAMETERS FILE ***

* for "add", "binarize", or "scale":
	[input image] [output image] [filter name] [value]
* for "increase":
	[input image] [output image] [filter name] [intensity value] [threshold value] [x] [y] [size]

The following steps can be followed to construct the parameters:
1. the input file name;
2. the output file name;
3. the name of the filter. Use "add", "binarize", "scale", "increase" for your filters;
4. choose one of the following option:
	a. if you use "add", "binarize", or "scale", you only need one more parameter which is the value 
	   for adding intensity, threshold value for binarize filter, or the scaling factor for scale filter.
	b. if you use "increase", there should be 5 more parameters: 
		- the intensity value;
		- threshold value;
		- upper left x-coordinate;
		- upper left y-coordinate;
		- the size of the square area

*** Run the program: 
$ cd project
$ make
$ cd bin
$ ./iptool parameters.txt
