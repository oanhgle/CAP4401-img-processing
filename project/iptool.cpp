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

using namespace std;

#define MAXLEN 256

int main (int argc, char** argv)
{
	image src, tgt;
	FILE *fp;
	char str[MAXLEN];
	char outfile[MAXLEN];
	char *pch;
	if ((fp = fopen(argv[1],"r")) == NULL) {
		fprintf(stderr, "Can't open file: %s\n", argv[1]);
		exit(1);
	}

	while(fgets(str,MAXLEN,fp) != NULL) {
		pch = strtok(str, " ");
		src.read(pch);

		pch = strtok(NULL, " ");
		strcpy(outfile, pch);

		pch = strtok(NULL, " ");
        if (strncasecmp(pch,"add",MAXLEN)==0) {
			/* Add Intensity */
			pch = strtok (NULL, " ");
			utility::addGrey(src,tgt,atoi(pch));
        }

        else if (strncasecmp(pch,"binarize",MAXLEN)==0) {
			/* Thresholding */
			pch = strtok(NULL, " ");
			utility::binarize(src,tgt,atoi(pch));
		}

		else if (strncasecmp(pch,"scale",MAXLEN)==0) {
			/* Image scaling */
			pch = strtok(NULL, " ");
			utility::scale(src,tgt,atof(pch));
		}
		
		else if (strncasecmp(pch,"increase",MAXLEN)==0) {
			/* Increase brightness/intensity of a certain area */
			vector<int> params;
			pch = strtok (NULL, " ");
			while (pch)
			{
				params.push_back(atoi(pch));
				pch = strtok (NULL, " ");
			}
			if(params.size() == 5) 
				utility::increase_brightness(src,tgt,params[0],params[1],params[2],params[3],params[4]);
			else 
			{
				printf("See readme.txt for how to construct the input parameters\n");
				continue;
			}
		}

		else {
			printf("No function: %s\n", pch);
			continue;
		}

		tgt.save(outfile);
	}
	fclose(fp);
	return 0;
}

