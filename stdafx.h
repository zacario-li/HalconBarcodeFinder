#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <fstream>

#include "opencv2\opencv.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2\imgproc.hpp"
#include <vector>

#ifndef __APPLE__
#  include "HalconCpp.h"
#  include "HDevThread.h"
#  if defined(__linux__) && !defined(NO_EXPORT_APP_MAIN)
#    include <X11/Xlib.h>
#  endif
#else
#  ifndef HC_LARGE_IMAGES
#    include <HALCONCpp/HalconCpp.h>
#    include <HALCONCpp/HDevThread.h>
#  else
#    include <HALCONCppxl/HalconCpp.h>
#    include <HALCONCppxl/HDevThread.h>
#  endif
#  include <stdio.h>
#  include <HALCON/HpThread.h>
#  include <CoreFoundation/CFRunLoop.h>
#endif

using namespace HalconCpp;
using namespace std;


//typedef
typedef struct _rect{
	string decoded_str;
	float row;
	float column;
	float width;
	float height;
}BarcodeRect;



void dev_open_window_fit_image(HObject ho_Image, HTuple hv_Row, HTuple hv_Column,HTuple hv_WidthLimit, HTuple hv_HeightLimit, HTuple *hv_WindowHandle);
// Chapter: Develop
// Short Description: Switch dev_update_pc, dev_update_var and dev_update_window to 'off'. 
void dev_update_off();
void action();
void forPM();
void disp_message(HTuple hv_WindowHandle, HTuple hv_String, HTuple hv_CoordSystem,
	HTuple hv_Row, HTuple hv_Column, HTuple hv_Color, HTuple hv_Box);
// Chapter: Graphics / Text
// Short Description: Set font independent of OS 
void set_display_font(HTuple hv_WindowHandle, HTuple hv_Size, HTuple hv_Font, HTuple hv_Bold,
	HTuple hv_Slant);
string GetCurrentDir();
//Hobject <-> IplImage
HObject IplImageToHImage(IplImage *pImage);
IplImage *HImageToIplImage(HObject &Hobj);
//OpenCV_Utils
bool roiAddImage(IplImage *pDst,bool flag);

/*=====================================================================================================
*export to QtUI*
function: checkBarCode()

input>>
pOriImg(input): Original IplImage pointer
pDstImg(output): function will allocate memory for the pointer, when finished using it, remember to use cvReleaseImage() to free the memory.
pCommonData(output): function will store the bar code string data in it, when finished using it, remeber to use delete[] to free the memory.

return>>
1 means OK
other specify the error code;
======================================================================================================*/
int checkBarCode(IplImage *pOriImg,IplImage **pDstImg,void **pCommonData);

/*=====================================================================================================
*draw barcode rect*
======================================================================================================*/
int drawBarcodeRect(IplImage *pI, vector<BarcodeRect> *t);

/*======================================================================================================
*find the barcode by index*
function: findBarcodeByIndex()

input>>
pOriImg(input): IplImage image file pointer
index(input):	barcode index. If pass -1, it will figure out all barcodes.
t:				barcode rect vector, store decoded string and x.y
=======================================================================================================*/
int findBarcodeByIndex(IplImage *pOriImg, int index, vector<BarcodeRect> *t);

/*======================================================================================================
*PhonePackageBarcodeCheck*

function: checkPhonePackageBarcode()

input>>
pOriImg0(input):first Original IplImage pointer
barcodeIndex0(input):the first barcode index which need to be checked
pOriImg1(input):second Original IplImage pointer
barcodeIndex1(input):the second barcode index which need to be checked
pDstImg(output):function will allocate memory for the pointer, when finished using it, remember to use cvReleaseImage() to free the memory.
pCommonData(output):function will store the bar code string data in it, when finished using it, remeber to use delete[] to free the memory.

return>>
1 means OK
other specify the error code;
=======================================================================================================*/
int checkPhonePackageBarcode(IplImage *pOriImg0,int barcodeIndex0,IplImage *pOriImg1,int barcodeIndex1, IplImage **pDstImg, void **pCommandData);
