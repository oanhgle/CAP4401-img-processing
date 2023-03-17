#include "utility.h"
#include <map>
#include <cmath>
#include <algorithm>

#define MAXRGB 255 //white
#define MINRGB 0 //black

std::string utility::intToString(int number)
{
   std::stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

int utility::checkValue(int value)
{
	if (value > MAXRGB)
		return MAXRGB;
	if (value < MINRGB)
		return MINRGB;
	return value;
}

/*-----------------------------------------------------------------------**/
bool utility::roi_isInbounds(image& img, int xl, int yl, int xr, int yr)
{
	if(!img.isInbounds(xl, yl) || !img.isInbounds(xr-1, yr-1))
	{
		printf("ROI ((%d,%d), (%d, %d)) is not in the image.", xl, yl, xr, yr);
		return false;
	}
	return true;
}

/*-----------------------------------------------------------------------**/
void utility::addGrey(image &tgt, int xl, int yl, int xr, int yr, int value)
{
	for (int i=xl; i<xr; i++)
		for (int j=yl; j<yr; j++)
		{
			tgt.setPixel(i,j,checkValue(tgt.getPixel(i,j)+value)); 
		}
}

/*-----------------------------------------------------------------------**/
void utility::binarize(image &tgt, int xl, int yl, int xr, int yr, int threshold)
{ 
	for (int i=xl; i<xr; i++)
	{
		for (int j=yl; j<yr; j++)
		{
			if (tgt.getPixel(i,j) < threshold)
				tgt.setPixel(i,j,MINRGB);
			else
				tgt.setPixel(i,j,MAXRGB);
		}
	}
}

/*-----------------------------------------------------------------------**/
void utility::scale(image &tgt, int xl, int yl, int xr, int yr, float ratio)
{
	int rows = (int)((float)(abs(xr-xl)) * ratio);
	int cols  = (int)((float)(abs(yl-yr)) * ratio);
	image tmp;
	tmp.resize(rows, cols);

	int diff_r = xl - 0; 
	int diff_c = yl - 0;
	for (int i=0; i<rows; i++)
	{
		for (int j=0; j<cols; j++)
		{
			int i2 = (int)floor((float)(i/ratio)) + diff_r;
			int j2 = (int)floor((float)(j/ratio)) + diff_c;

			if (ratio > 0) {
				/* Expansion -> Directly copy the value */
				tmp.setPixel(i,j,RED,checkValue(tgt.getPixel(i2,j2,RED)));
				tmp.setPixel(i,j,GREEN,checkValue(tgt.getPixel(i2,j2,GREEN)));
				tmp.setPixel(i,j,BLUE,checkValue(tgt.getPixel(i2,j2,BLUE)));
			}

			else {
				/* Reduction -> Average the values of four pixels for each channel*/
				int valueR = tgt.getPixel(i2,j2,RED) + tgt.getPixel(i2,j2+1,RED) + tgt.getPixel(i2+1,j2,RED) + tgt.getPixel(i2+1,j2+1,RED);
				tmp.setPixel(i,j,RED,checkValue(valueR/4));
				int valueG = tgt.getPixel(i2,j2,GREEN) + tgt.getPixel(i2,j2+1,GREEN) + tgt.getPixel(i2+1,j2,GREEN) + tgt.getPixel(i2+1,j2+1,GREEN);
				tmp.setPixel(i,j,GREEN,checkValue(valueG/4));
				int valueB = tgt.getPixel(i2,j2,BLUE) + tgt.getPixel(i2,j2+1,BLUE) + tgt.getPixel(i2+1,j2,BLUE) + tgt.getPixel(i2+1,j2+1,BLUE);
				tmp.setPixel(i,j,BLUE,checkValue(valueB/4));
			}
		}
	}

	/* map tmp to ROI */
	for (int i=xl; i<xr; i++)
	{
		for (int j=yl; j<yr; j++)
		{
			if(tmp.isInbounds(i-diff_r, j-diff_c))
			{
				tgt.setPixel(i,j,RED,tmp.getPixel(i-diff_r, j-diff_c, RED));
				tgt.setPixel(i,j,GREEN,tmp.getPixel(i-diff_r, j-diff_c, GREEN));
				tgt.setPixel(i,j,BLUE,tmp.getPixel(i-diff_r, j-diff_c, BLUE));
			}
			else 
			{
				tgt.setPixel(i, j, RED, 0);
				tgt.setPixel(i, j, GREEN, 0);
				tgt.setPixel(i, j, BLUE, 0);
			}
		}
	}
}

/*-----------------------------------------------------------------------**/
void utility::aoi_bright(image &tgt, int xl, int yl, int xr, int yr, int value)
{
	int xc = xl + (xr-xl)/2;
	int yc = yl + (yr-yl)/2;
	for (int i=xl; i<xr; i++)
	{
		for (int j=yl; j<yr; j++)
		{
			if(abs(i - xc) < 21)
			{
				tgt.setPixel(i,j,RED,checkValue(tgt.getPixel(i,j,RED) + (value * abs((i-xc)/20.0))));
				tgt.setPixel(i,j,GREEN,checkValue(tgt.getPixel(i,j,GREEN) + (value * abs((i-xc)/20.0))));
				tgt.setPixel(i,j,BLUE,checkValue(tgt.getPixel(i,j,BLUE) + (value * abs((i-xc)/20.0))));
			}
			else 
			{	
				tgt.setPixel(i,j,RED,checkValue(tgt.getPixel(i,j,RED)));
				tgt.setPixel(i,j,GREEN,checkValue(tgt.getPixel(i,j,GREEN)));
				tgt.setPixel(i,j,BLUE,checkValue(tgt.getPixel(i,j,BLUE)));
			}
		}
	}
}

/*-----------------------------------------------------------------------**/
void utility::color_bright(image &tgt, int xl, int yl, int xr, int yr, int dr, int dg, int db)
{
	for (int i=xl; i<xr; i++)
	{
		for (int j=yl; j<yr; j++)
		{
			// R1=R + DR, G1=G + DG, B1=B + DB
			tgt.setPixel(i, j, RED, checkValue(tgt.getPixel(i, j, RED) + dr));
			tgt.setPixel(i, j, GREEN, checkValue(tgt.getPixel(i, j, GREEN) + dg));
			tgt.setPixel(i, j, BLUE, checkValue(tgt.getPixel(i, j, BLUE) + db));
		}
	}
}
/*-----------------------------------------------------------------------**/
void utility::color_vis(image &tgt, int xl, int yl, int xr, int yr, int value, int threshold)
{
	for (int i=xl; i<xr; i++)
	{
		for (int j=yl; j<yr; j++)
		{
			if(abs(value - tgt.getPixel(i,j)) < threshold)
			{
				tgt.setPixel(i, j, RED, checkValue(tgt.getPixel(i, j)));
				tgt.setPixel(i, j, GREEN, 0);
				tgt.setPixel(i, j, BLUE, 0);
			}
			else
			{
				tgt.setPixel(i, j, GREEN, checkValue(tgt.getPixel(i, j)));
				tgt.setPixel(i, j, RED, 0);
				tgt.setPixel(i, j, BLUE, 0);
			}
		}
	}
}

/*-----------------------------------------------------------------------**/
void utility::histogram(image img, int xl, int yl, int xr, int yr, channel channel, char* outfile)
{
	/* calculate H */
	vector<int> H(256,0);
	for (int i=xl; i<xr; i++)
	{
		for (int j=yl; j<yr; j++)
		{
        	H[img.getPixel(i,j,channel)]+=1;
		}
	}
	/*	plotting */
	image plot ;
	plot.resize(256, 256);
	int max_value = 0;
	// const int max_value = *std::max_element(H.begin(), H.end());
	for(int i = 0; i < H.size(); i++)
	{
		max_value = max(max_value, H[i]);
	}
	for(int i = 0; i < H.size(); i++)
	{
		int v = ((float)H[i]/max_value*256.0);
		int j = plot.getNumberOfColumns()-1;
		while(v>0 && j>0)
		{
			plot.setPixel(j,i,channel,255);
			v--; j--;
		}
	}
	plot.save(outfile);
}

/*-----------------------------------------------------------------------**/
void utility::histogram_stretch(image &tgt, int xl, int yl, int xr, int yr, channel component, int a, int b)
{
	/* find roi's imin and imax */
	int imin=255, imax=0;
	for (int i=xl; i<xr; i++)
	{
		for (int j=yl; j<yr; j++)
		{
			imin = min(imin, tgt.getPixel(i,j,component));
			imax = max(imax, tgt.getPixel(i,j,component));
		}
	}

	/* compute i1 and i2*/
	double i1 = 1.05 * imin; 
	double i2 = 0.95 * imax; 

	/* stretching */
	for (int i=xl; i<xr; i++)
	{
		for (int j=yl; j<yr; j++)
		{
			if (tgt.getPixel(i,j,component) < i1)
				tgt.setPixel(i, j, component, a);
			else if (tgt.getPixel(i,j,component) > i2) 
				tgt.setPixel(i, j, component, b);
			else
			{
				int p_prime = (tgt.getPixel(i, j, component) - i1) * ((b-a)/(i2-i1)) + a;
				tgt.setPixel(i, j, component, checkValue(p_prime));
			}
		}
	}
}

/*-----------------------------------------------------------------------**/
void utility::thres_histo(image &tgt, int xl, int yl, int xr, int yr, int a, int b, int threshold_1, int threshold_2)
{
	/* 
		dictionary to store the subsets 
		0-dark; 1-medium; 2-bright
	*/
	map<int, vector< pair<int,int> > > dictionary;

	/* apply two-valued threshold */
	for (int i=xl; i<xr; i++)
	{
		for (int j=yl; j<yr; j++)
		{
			if (tgt.getPixel(i,j) < threshold_1) 			// dark
				dictionary[0].push_back(make_pair(i,j));
			else if (tgt.getPixel(i,j) > threshold_2) 		// bright
				dictionary[2].push_back(make_pair(i,j));
			else dictionary[1].push_back(make_pair(i,j)); 	// medium
		}
	}

	/* apply histo stretch on each subset */
	for(auto subset : dictionary)
	{	 
		int imin = 255;
		int imax = 0;
		for(auto pixel : subset.second)
		{
			int i = pixel.first, j = pixel.second;
			imin = min(imin, tgt.getPixel(i,j));
			imax = max(imax, tgt.getPixel(i,j));
		}
		double i1 = 1.05 * imin; 
		double i2 = 0.95 * imax;
		for(auto pixel : subset.second)
		{	
			int i = pixel.first, j = pixel.second;
			if (tgt.getPixel(i,j) < i1) 
				tgt.setPixel(i, j, a);
			else if (tgt.getPixel(i,j) > i2) 
				tgt.setPixel(i, j, b);
			else
			{
				int p_prime = (tgt.getPixel(i, j) - i1) * ((b-a)/(i2-i1)) + a;
				tgt.setPixel(i, j, checkValue(p_prime));
			}
		}
	}
}

/*-----------------------------------------------------------------------**/
void utility::rgb_histo_stretch(image &tgt, int xl, int yl, int xr, int yr, int a, int b)
{
	/* apply histo stretch to all three channels simultaneously */
	histogram_stretch(tgt, xl, yl, xr, yr, RED, a, b);
	histogram_stretch(tgt, xl, yl, xr, yr, GREEN, a, b);
	histogram_stretch(tgt, xl, yl, xr, yr, BLUE, a, b);
}

/*-----------------------------------------------------------------------**/
void utility::colorize(image &tgt, int xl, int yl, int xr, int yr, int a, int b, int threshold_1, int threshold_2)
{
	/* two-valued threshold */
	for (int i=xl; i<xr; i++)
	{
		for (int j=yl; j<yr; j++)
		{
			if (tgt.getPixel(i,j) < threshold_1) 		// color to red only
			{	
				tgt.setPixel(i, j, RED, checkValue(tgt.getPixel(i, j)));
				tgt.setPixel(i, j, GREEN, 0);
				tgt.setPixel(i, j, BLUE, 0);
			}
			else if (tgt.getPixel(i,j) > threshold_2) 	// color to blue only
			{	
				tgt.setPixel(i, j, BLUE, checkValue(tgt.getPixel(i, j)));
				tgt.setPixel(i, j, GREEN, 0);
				tgt.setPixel(i, j, RED, 0);
			}
			else 										// color to green only
			{
				tgt.setPixel(i, j, GREEN, checkValue(tgt.getPixel(i, j)));
				tgt.setPixel(i, j, RED, 0);
				tgt.setPixel(i, j, BLUE, 0);
			}
		}
	}

	/* apply histogram stretch on each channel */
	histogram_stretch(tgt, xl, yl, xr, yr, RED, a, b);
	histogram_stretch(tgt, xl, yl, xr, yr, BLUE, a, b);
	histogram_stretch(tgt, xl, yl, xr, yr, GREEN, a, b);
}

/*-----------------------------------------------------------------------**/
void utility::multithreshold(image &tgt, int xl, int yl, int xr, int yr)
{
	/* compute interval size and thres values */
	int imin = 255, imax = 0;
	for (int i=xl; i<xr; i++)
	{
		for (int j=yl; j<yr; j++)
		{
			imin = min(imin, tgt.getPixel(i,j));
			imax = max(imax, tgt.getPixel(i,j));
		}
	}
	double interval_size = (imax-imin)/10.0;
	vector<int> thresholds(9, 0);
	for(int i = 0; i < 9; i++)
		thresholds[i] = (int)(imin + i * interval_size);
	
	/* apply 9-valued threshold */
	for (int i=xl; i<xr; i++)
	{
		for (int j=yl; j<yr; j++)
		{
			if (imin <= tgt.getPixel(i,j) && tgt.getPixel(i,j) < thresholds[0]) // [imin, t[0])
			{	
				tgt.setPixel(i, j, RED, 255);
				tgt.setPixel(i, j, GREEN, 0);
				tgt.setPixel(i, j, BLUE, 0);
			}
			else if (thresholds[0] <= tgt.getPixel(i,j) && tgt.getPixel(i,j) < thresholds[1]) // [t0, t1)
			{	
				tgt.setPixel(i, j, RED, 0);
				tgt.setPixel(i, j, GREEN, 255);
				tgt.setPixel(i, j, BLUE, 0);
			}
			else if (thresholds[1] <= tgt.getPixel(i,j) && tgt.getPixel(i,j) < thresholds[2]) // [t1, t2)
			{	
				tgt.setPixel(i, j, RED, 0);
				tgt.setPixel(i, j, GREEN, 0);
				tgt.setPixel(i, j, BLUE, 255);
			}
			else if (thresholds[2] <= tgt.getPixel(i,j) && tgt.getPixel(i,j) < thresholds[3]) // [t2, t3)
			{
				tgt.setPixel(i, j, RED, 0);
				tgt.setPixel(i, j, GREEN, 255);
				tgt.setPixel(i, j, BLUE, 255);
			}
			else if (thresholds[3] <= tgt.getPixel(i,j) && tgt.getPixel(i,j) < thresholds[4]) // [t3, t4)
			{	
				tgt.setPixel(i, j, RED, 255);
				tgt.setPixel(i, j, GREEN, 0);
				tgt.setPixel(i, j, BLUE, 255);
			}
			else if (thresholds[4] <= tgt.getPixel(i,j) && tgt.getPixel(i,j) < thresholds[5]) // [t4, t5)
			{	
				tgt.setPixel(i, j, RED, 255);
				tgt.setPixel(i, j, GREEN, 255);
				tgt.setPixel(i, j, BLUE, 0);
			}
			else if (thresholds[5] <= tgt.getPixel(i,j) && tgt.getPixel(i,j) < thresholds[6]) // [t5, t6)
			{
				tgt.setPixel(i, j, RED, 255);
				tgt.setPixel(i, j, GREEN, 192);
				tgt.setPixel(i, j, BLUE, 203);
			}
			else if (thresholds[6] <= tgt.getPixel(i,j) && tgt.getPixel(i,j) < thresholds[7]) // [t6, t7)
			{	
				tgt.setPixel(i, j, RED, 105);
				tgt.setPixel(i, j, GREEN, 75);
				tgt.setPixel(i, j, BLUE, 0);
			}
			else if (thresholds[7] <= tgt.getPixel(i,j) && tgt.getPixel(i,j) < thresholds[8]) // [t7, t8)
			{	
				tgt.setPixel(i, j, RED, 0);
				tgt.setPixel(i, j, GREEN, 128);
				tgt.setPixel(i, j, BLUE, 128);
			}
			else // [t8, imax]
			{
				tgt.setPixel(i, j, RED, 255);
				tgt.setPixel(i, j, GREEN, 109);
				tgt.setPixel(i, j, BLUE, 16);
			}
		}
	}
}
