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

Each function operates only in a certain ROI. There's a need to pass ROI's coordinate locations 
as parameter to these functions.

1. Add intensity: add
Increase the intensity for a gray-level image.

2. Binarization: binarize
Binarize the pixels with the threshold.

3. Scaling: scale
Reduce or expand the heigh and width with two scale factors.
Scaling factor = 2: double height and width of the input image.
Scaling factor = 0.5: half height and width of the input image.

4. Brighten the AOI: aoi
Selecting center (XC, YC) within ROI, then brighten the area by V*|(i-XC)/20| if |i-XC|<21

5. Color brightness: brighten
Process each color channel: R1=R + DR, G1=G + DG, B1=B + DB.

6. Color visualization: visualize
Color red if abs(V – I (i,j)) < T, otherwise color green


*** PARAMETERS FILE ***

[input image] [output image] [#roi] [xl] [yl] [Sx] [Sy] [function name] [p1...pX] ...

The following steps can be followed to construct the parameters:
1. the input file name;
2. the output file name;
3. number of ROIs
4. xl - inital row;
5. yl - intial column;
6. Sx - total number of pixels in the x axis
7. Sy - total number of pixels in the y axis
4. the name of the filter. Use "add", "binarize", "scale", "aoi", "brighten", "visualize" for your filters;
4. choose one of the following option:
	a. if you use "add", "binarize", "scale", or "aoi", you only need one more parameter which is the value 
		for adding intensity, threshold value for binarize filter, the scaling factor for scale filter, or
	   	the value for adding intensity within AOI.
	b. if you use "brighten", there should be 3 more parameters: DR, DG, DB. They are the values you want to
		add to each channel R,G,B 
	c. if you use "visualize", there should be 2 more parameters: V and T, which represents intensity 
		and threshold values.

*** Run the program: 
$ cd project
$ make
$ cd bin

