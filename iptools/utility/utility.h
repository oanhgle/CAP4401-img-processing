#ifndef UTILITY_H
#define UTILITY_H

#include "../image/image.h"
#include <sstream>
#include <math.h>

class utility
{
	public:
		utility();
		virtual ~utility();
		static std::string intToString(int number);
		static int checkValue(int value);
		static void addGrey(image &tgt, int xl, int yl, int xr, int yr, int value);
		static void binarize(image &tgt, int xl, int yl, int xr, int yr, int threshold);
		static void scale(image &tgt, int xl, int yl, int xr, int yr, float ratio);
		static bool roi_isInbounds(image& img, int xl, int yl, int xr, int yr);
		static void aoi_bright(image &tgt, int xl, int yl, int xr, int yr, int value);
		static void color_bright(image &tgt, int xl, int yl, int xr, int yr, int dr, int dg, int db);
		static void color_vis(image &tgt, int xl, int yl, int xr, int yr, int value, int threshold);
};

#endif

