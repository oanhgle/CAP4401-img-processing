/************************************************************
 *															*
 * This sample project include three functions:				*
 * 1. Add intensity for gray-level image.					*
 *    Input: source image, output image name, value			*
 *															*
 * 2. Image thresholding: pixels will become black if the	*
 *    intensity is below the threshold, and white if above	*
 *    or equal the threhold.								*
 *    Input: source image, output image name, threshold		*
 *															*
 * 3. Image scaling: reduction/expansion of 2 for 			*
 *    the width and length. This project uses averaging 	*
 *    technique for reduction and pixel replication			*
 *    technique for expansion.								*
 *    Input: source image, output image name, scale factor	*
 *															*
 ************************************************************/

#include "../iptools/core.h"
#include <strings.h>
#include <string.h>
#include <algorithm>

using namespace std;

#define MAXLEN 256

struct Parameter {
  int xl;
  int yl;
  int xr;
  int yr;
  char* function;
  vector<float> px;
};

int main (int argc, char** argv)
{
	image src, tgt;
	FILE *fp;
	char str[MAXLEN];
	char infile[MAXLEN];
	char outfile[MAXLEN];
	char *pch;
	if ((fp = fopen(argv[1],"r")) == NULL) {
		fprintf(stderr, "Can't open file: %s\n", argv[1]);
		exit(1);
	}

	while(fgets(str,MAXLEN,fp) != NULL) {
		pch = strtok(str, " ");
		src.read(pch);
		strcpy(infile, pch);

		pch = strtok(NULL, " ");
		strcpy(outfile, pch);

		pch = strtok(NULL, " ");

		int num_roi = atoi(pch);

		// allow for up to 3 roi per image.
		if(num_roi > 3 || num_roi < 1) continue; 

		// read parameters
		vector<Parameter> pr;
		vector<bool> overlap;
		vector<char *> tmp;
		pch = strtok (NULL, " ");

		int size = -1;
		while (pch)
		{
			tmp.push_back(pch);
			if(tmp.size() == 5)
			{
				tmp[4][strcspn(tmp[4],"\n")] = '\0';
				if (strncasecmp(tmp[4],"combohisto",MAXLEN)==0 || strncasecmp(tmp[4],"colorize",MAXLEN)==0) size = 9;
				else if (strncasecmp(tmp[4],"bright",MAXLEN)==0 || strncasecmp(tmp[4],"perchastretch",MAXLEN)==0) size = 8;
				else if (strncasecmp(tmp[4],"visualize",MAXLEN)==0 || strncasecmp(tmp[4],"rgbstretch",MAXLEN)==0) size = 7;
				else if (strncasecmp(tmp[4],"histo",MAXLEN)==0 || strncasecmp(tmp[4],"multithreshold",MAXLEN)==0) size = 5;
				else size = 6;
			}
			if(tmp.size() == size)
			{
				vector<float> intsub;
				vector<char*> sub = {tmp.begin() + 5, tmp.end()}; 
				transform(sub.begin(), sub.end(),  std::back_inserter(intsub), [&](char* x) { 
					if(strncasecmp(x,"R",MAXLEN)==0) return 0.0;
					else if (strncasecmp(x,"G",MAXLEN)==0) return 1.0;
					else if (strncasecmp(x,"B",MAXLEN)==0) return 2.0;
					return atof(x);
				});
				
				Parameter new_param = {
					atoi(tmp[0]), 
					atoi(tmp[1]), 
					atoi(tmp[0]) + atoi(tmp[3]), 
					atoi(tmp[1]) + atoi(tmp[2]),
					tmp[4],
					intsub
				};
				
				pr.push_back(new_param);

				// overlapping?
				overlap.push_back(false);
				int i, j;

				i = j = pr.size() - 1;

				while(--j >= 0)
				{
					vector<int> a = {pr[i].xl, pr[i].yl, pr[i].xr, pr[i].yr};
					vector<int> b = {pr[j].xl, pr[j].yl, pr[j].xr, pr[j].yr};
					auto isOverlap = [] (vector<int> a, vector<int> b){
						return (min(a[2], b[2]) > max(a[0], b[0]) && min(a[3], b[3]) > max(a[1], b[1])); 
					};
					if(isOverlap(a, b))
					{
						overlap[j] = true;
						overlap[i] = true;
						printf("ROI ((%d,%d), (%d, %d)) and ((%d,%d), (%d, %d)) are overlapping. Rejected them!\n", pr[j].xl, pr[j].yl, pr[j].xr, pr[j].yr, pr[i].xl, pr[i].yl, pr[i].xr, pr[i].yr);
					}
				}
				tmp.clear();
				size = -1;
			}
			pch = strtok (NULL, " ");
		}
		
		if(overlap.size() != num_roi) printf("Parameters need to match the input #roi\n");
		
		for(int i = num_roi-1; i >= 0; i--)
		{
			if(overlap[i])
				pr.erase(pr.begin()+i);
		}

		if(pr.empty()) continue;

		// perform the processing if no overlapping
		tgt = src; 
		for(int i = 0; i < pr.size(); i++)
		{
			string out = outfile;
			string in = infile;
			std::string outname, inname;
			outname = out.substr(0, out.size()-4);
			inname = in.substr(0, in.size()-4);
			// check boundary
			if(!utility::roi_isInbounds(tgt, pr[i].xl, pr[i].yl, pr[i].xr, pr[i].yr))
				continue;

			if (strncasecmp(pr[i].function,"add",MAXLEN)==0) {
				/* Add Intensity */
				utility::addGrey(tgt, pr[i].xl, pr[i].yl, pr[i].xr, pr[i].yr, (int)pr[i].px[0]);
        	}

			else if (strncasecmp(pr[i].function,"binarize",MAXLEN)==0) {
				/* Thresholding */
				utility::binarize(tgt, pr[i].xl, pr[i].yl, pr[i].xr, pr[i].yr, (int)pr[i].px[0]);
			}

			else if (strncasecmp(pr[i].function,"scale",MAXLEN)==0) {
				/* Image scaling */
				utility::scale(tgt, pr[i].xl, pr[i].yl, pr[i].xr, pr[i].yr, pr[i].px[0]);
			}

			else if (strncasecmp(pr[i].function,"aoi",MAXLEN)==0) {
				/* bright the area of interest (AOI) */
				utility::aoi_bright(tgt, pr[i].xl, pr[i].yl, pr[i].xr, pr[i].yr, (int)pr[i].px[0]);
			}

			else if (strncasecmp(pr[i].function,"bright",MAXLEN)==0) {
				/* Add color brightness */
				utility::color_bright(tgt, pr[i].xl, pr[i].yl, pr[i].xr, (int)pr[i].yr, (int)pr[i].px[0], (int)pr[i].px[1], (int)pr[i].px[2]);
			}

			else if (strncasecmp(pr[i].function,"visualize",MAXLEN)==0) {
				/* Add color visualization */
				utility::color_vis(tgt, pr[i].xl, pr[i].yl, pr[i].xr, pr[i].yr, (int)pr[i].px[0], (int)pr[i].px[1]);
			}

			else if (strncasecmp(pr[i].function,"histo",MAXLEN)==0) {
				/* Perform histogram stretch on single level */
				string before = inname + "-h-(roi" + std::to_string(i) + ").pgm";
				string after = outname + "-h-(roi" + std::to_string(i) + ").pgm";
				utility::histogram(tgt, pr[i].xl, pr[i].yl, pr[i].xr, pr[i].yr, RED, const_cast<char*>(before.c_str()));
				utility::histogram_stretch(tgt, pr[i].xl, pr[i].yl, pr[i].xr, pr[i].yr, RED, 0, 255);
				utility::histogram(tgt, pr[i].xl, pr[i].yl, pr[i].xr, pr[i].yr, RED, const_cast<char*>(after.c_str()));
			}

			else if (strncasecmp(pr[i].function,"combohisto",MAXLEN)==0) {
				/* Perform 2valued threshold + histogram stretch on single level */
				string before = inname + "-ch-(roi" + std::to_string(i) + ").pgm";
				string after = outname + "-ch-(roi" + std::to_string(i) + ").pgm";
				utility::histogram(tgt, pr[i].xl, pr[i].yl, pr[i].xr, pr[i].yr, RED, const_cast<char*>(before.c_str()));
				utility::thres_histo(tgt, pr[i].xl, pr[i].yl, pr[i].xr, pr[i].yr, (int)pr[i].px[0], (int)pr[i].px[1], (int)pr[i].px[2], (int)pr[i].px[3]);
				utility::histogram(tgt, pr[i].xl, pr[i].yl, pr[i].xr, pr[i].yr, RED, const_cast<char*>(after.c_str()));
			}

			else if (strncasecmp(pr[i].function,"perchastretch",MAXLEN)==0) {
				/* Perform histogram stretching to R,G,B components independently */
				string before = inname + "-percs-(roi" + std::to_string(i) + ").ppm";
				string after = outname + "-percs-(roi" + std::to_string(i) + ").ppm";
				utility::histogram(tgt, pr[i].xl, pr[i].yl, pr[i].xr, pr[i].yr, (channel)pr[i].px[0], const_cast<char*>(before.c_str()));
				utility::histogram_stretch(tgt, pr[i].xl, pr[i].yl, pr[i].xr, pr[i].yr, (channel)pr[i].px[0], (int)pr[i].px[1], (int)pr[i].px[2]);
				utility::histogram(tgt, pr[i].xl, pr[i].yl, pr[i].xr, pr[i].yr, (channel)pr[i].px[0], const_cast<char*>(after.c_str()));
			}

			else if(strncasecmp(pr[i].function,"rgbstretch",MAXLEN)==0) {
				/* Perform histogram stretching to R,G,B components simultaneously */
				string before_r = inname + "-r-(roi" + std::to_string(i) + ").ppm";
				string after_r = outname + "-r-(roi" + std::to_string(i) + ").ppm";
				string before_g = inname + "-g-(roi" + std::to_string(i) + ").ppm";
				string after_g = outname + "-g-roi" + std::to_string(i) + ").ppm";
				string before_b = inname + "-b-(roi" + std::to_string(i) + ").ppm";
				string after_b = outname + "-b-(roi" + std::to_string(i) + ").ppm";
				utility::histogram(tgt, pr[i].xl, pr[i].yl, pr[i].xr, pr[i].yr, RED, const_cast<char*>(before_r.c_str()));
				utility::histogram(tgt, pr[i].xl, pr[i].yl, pr[i].xr, pr[i].yr, GREEN, const_cast<char*>(before_g.c_str()));
				utility::histogram(tgt, pr[i].xl, pr[i].yl, pr[i].xr, pr[i].yr, BLUE, const_cast<char*>(before_b.c_str()));
				utility::rgb_histo_stretch(tgt, pr[i].xl, pr[i].yl, pr[i].xr, pr[i].yr, (int)pr[i].px[0], (int)pr[i].px[1]);
				utility::histogram(tgt, pr[i].xl, pr[i].yl, pr[i].xr, pr[i].yr, RED, const_cast<char*>(after_r.c_str()));
				utility::histogram(tgt, pr[i].xl, pr[i].yl, pr[i].xr, pr[i].yr, GREEN, const_cast<char*>(after_g.c_str()));
				utility::histogram(tgt, pr[i].xl, pr[i].yl, pr[i].xr, pr[i].yr, BLUE, const_cast<char*>(after_b.c_str()));
			}

			else if(strncasecmp(pr[i].function,"colorize",MAXLEN)==0) {
				/* Pseudo coloring */
				utility::colorize(tgt, pr[i].xl, pr[i].yl, pr[i].xr, pr[i].yr, (int)pr[i].px[0], (int)pr[i].px[1], (int)pr[i].px[2], (int)pr[i].px[3]);
			}

			else if(strncasecmp(pr[i].function,"multithreshold",MAXLEN)==0) {
				/* Perform 9-valued equal thresholding + coloring */
				utility::multithreshold(tgt, pr[i].xl, pr[i].yl, pr[i].xr, pr[i].yr);
			}

			else {
				printf("No function: %s\n", pch);
				continue;
			}
			tgt.save(outfile);
		}
	}
	fclose(fp);
	return 0;
}

