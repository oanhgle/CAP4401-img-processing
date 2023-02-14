#include "utility.h"
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
	if(!img.isInbounds(xl, yl) || !img.isInbounds(xr, yr))
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