// bar-code-result-finder_v0.1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

int _tmain(int argc, _TCHAR* argv[])
{
	//action();
	forPM();
	return 0;
}
void forPM(){

	string workingPath, fullpath, fname, icBad, icGood;
	workingPath = GetCurrentDir();

REENTER:
	cout << "请输入文件名(带后缀名)：";
	fname.clear();
	fullpath.clear();
	cin >> fname;
	fullpath += workingPath;
	fullpath += fname;
	cout << "full dir name is:" << fullpath << endl;
	//check file
	ifstream in(fullpath.c_str());
	if (!in){
		cout << "文件不存在" << endl;
		goto REENTER;
	}

	IplImage *pdst,*ts;
	IplImage *tt = cvLoadImage(fullpath.c_str());
	fullpath.clear();
	fname.clear();
	cout << "input second file name:";
	cin >> fname;
	fullpath += workingPath;
	fullpath += fname;
	ts = cvLoadImage(fullpath.c_str());

	checkPhonePackageBarcode(tt, -1, ts, -1, &pdst, NULL);
	cvNamedWindow("BarR", CV_WINDOW_NORMAL);
	cvShowImage("BarR", pdst);
	cvWaitKey(0);
	cvReleaseImage(&pdst);

	//IplImage *pDst = NULL;
	//char *pDs;
	//try{
	//	checkBarCode(tt, &pDst, (void **)&pDs);
	//}
	//catch (...){
	//	enum{nFrames = 30};
	//	void *pFrames[nFrames];
	//	int mFrameCount = CaptureStackBackTrace(1, nFrames, pFrames, NULL);
	//	for (int i = 0; i < nFrames; i++){
	//		cout << "trace: " << pFrames[i] << endl;
	//	}
	//	system("pause");
	//}
	//cvShowImage("Dst", pDst);
	//cvWaitKey(0);

	//delete[] pDs;
	cvReleaseImage(&tt);
	//cvReleaseImage(&pDst);
}
void dev_open_window_fit_image(HObject ho_Image, HTuple hv_Row, HTuple hv_Column,
	HTuple hv_WidthLimit, HTuple hv_HeightLimit, HTuple *hv_WindowHandle)
{

	// Local iconic variables

	// Local control variables
	HTuple  hv_MinWidth, hv_MaxWidth, hv_MinHeight;
	HTuple  hv_MaxHeight, hv_ResizeFactor, hv_ImageWidth, hv_ImageHeight;
	HTuple  hv_TempWidth, hv_TempHeight, hv_WindowWidth, hv_WindowHeight;

	//This procedure opens a new graphics window and adjusts the size
	//such that it fits into the limits specified by WidthLimit
	//and HeightLimit, but also maintains the correct image aspect ratio.
	//
	//If it is impossible to match the minimum and maximum extent requirements
	//at the same time (f.e. if the image is very long but narrow),
	//the maximum value gets a higher priority,
	//
	//Parse input tuple WidthLimit
	if (0 != (HTuple((hv_WidthLimit.TupleLength()) == 0).TupleOr(hv_WidthLimit<0)))
	{
		hv_MinWidth = 500;
		hv_MaxWidth = 800;
	}
	else if (0 != ((hv_WidthLimit.TupleLength()) == 1))
	{
		hv_MinWidth = 0;
		hv_MaxWidth = hv_WidthLimit;
	}
	else
	{
		hv_MinWidth = ((const HTuple&)hv_WidthLimit)[0];
		hv_MaxWidth = ((const HTuple&)hv_WidthLimit)[1];
	}
	//Parse input tuple HeightLimit
	if (0 != (HTuple((hv_HeightLimit.TupleLength()) == 0).TupleOr(hv_HeightLimit<0)))
	{
		hv_MinHeight = 400;
		hv_MaxHeight = 600;
	}
	else if (0 != ((hv_HeightLimit.TupleLength()) == 1))
	{
		hv_MinHeight = 0;
		hv_MaxHeight = hv_HeightLimit;
	}
	else
	{
		hv_MinHeight = ((const HTuple&)hv_HeightLimit)[0];
		hv_MaxHeight = ((const HTuple&)hv_HeightLimit)[1];
	}
	//
	//Test, if window size has to be changed.
	hv_ResizeFactor = 1;
	GetImageSize(ho_Image, &hv_ImageWidth, &hv_ImageHeight);
	//First, expand window to the minimum extents (if necessary).
	if (0 != (HTuple(hv_MinWidth>hv_ImageWidth).TupleOr(hv_MinHeight>hv_ImageHeight)))
	{
		hv_ResizeFactor = (((hv_MinWidth.TupleReal()) / hv_ImageWidth).TupleConcat((hv_MinHeight.TupleReal()) / hv_ImageHeight)).TupleMax();
	}
	hv_TempWidth = hv_ImageWidth*hv_ResizeFactor;
	hv_TempHeight = hv_ImageHeight*hv_ResizeFactor;
	//Then, shrink window to maximum extents (if necessary).
	if (0 != (HTuple(hv_MaxWidth<hv_TempWidth).TupleOr(hv_MaxHeight<hv_TempHeight)))
	{
		hv_ResizeFactor = hv_ResizeFactor*((((hv_MaxWidth.TupleReal()) / hv_TempWidth).TupleConcat((hv_MaxHeight.TupleReal()) / hv_TempHeight)).TupleMin());
	}
	hv_WindowWidth = hv_ImageWidth*hv_ResizeFactor;
	hv_WindowHeight = hv_ImageHeight*hv_ResizeFactor;
	//Resize window
	SetWindowAttr("background_color", "black");
	OpenWindow(hv_Row, hv_Column, hv_WindowWidth, hv_WindowHeight, 0, "", "", &(*hv_WindowHandle));
	HDevWindowStack::Push((*hv_WindowHandle));
	if (HDevWindowStack::IsOpen())
		SetPart(HDevWindowStack::GetActive(), 0, 0, hv_ImageHeight - 1, hv_ImageWidth - 1);
	return;
}

void dev_update_off()
{

	//This procedure sets different update settings to 'off'.
	//This is useful to get the best performance and reduce overhead.
	//
	// dev_update_pc(...); only in hdevelop
	// dev_update_var(...); only in hdevelop
	// dev_update_window(...); only in hdevelop
	return;
}

void disp_message(HTuple hv_WindowHandle, HTuple hv_String, HTuple hv_CoordSystem,
	HTuple hv_Row, HTuple hv_Column, HTuple hv_Color, HTuple hv_Box)
{

	// Local iconic variables

	// Local control variables
	HTuple  hv_Red, hv_Green, hv_Blue, hv_Row1Part;
	HTuple  hv_Column1Part, hv_Row2Part, hv_Column2Part, hv_RowWin;
	HTuple  hv_ColumnWin, hv_WidthWin, hv_HeightWin, hv_MaxAscent;
	HTuple  hv_MaxDescent, hv_MaxWidth, hv_MaxHeight, hv_R1;
	HTuple  hv_C1, hv_FactorRow, hv_FactorColumn, hv_UseShadow;
	HTuple  hv_ShadowColor, hv_Exception, hv_Width, hv_Index;
	HTuple  hv_Ascent, hv_Descent, hv_W, hv_H, hv_FrameHeight;
	HTuple  hv_FrameWidth, hv_R2, hv_C2, hv_DrawMode, hv_CurrentColor;

	//This procedure displays text in a graphics window.
	//
	//Input parameters:
	//WindowHandle: The WindowHandle of the graphics window, where
	//   the message should be displayed
	//String: A tuple of strings containing the text message to be displayed
	//CoordSystem: If set to 'window', the text position is given
	//   with respect to the window coordinate system.
	//   If set to 'image', image coordinates are used.
	//   (This may be useful in zoomed images.)
	//Row: The row coordinate of the desired text position
	//   If set to -1, a default value of 12 is used.
	//Column: The column coordinate of the desired text position
	//   If set to -1, a default value of 12 is used.
	//Color: defines the color of the text as string.
	//   If set to [], '' or 'auto' the currently set color is used.
	//   If a tuple of strings is passed, the colors are used cyclically
	//   for each new textline.
	//Box: If Box[0] is set to 'true', the text is written within an orange box.
	//     If set to' false', no box is displayed.
	//     If set to a color string (e.g. 'white', '#FF00CC', etc.),
	//       the text is written in a box of that color.
	//     An optional second value for Box (Box[1]) controls if a shadow is displayed:
	//       'true' -> display a shadow in a default color
	//       'false' -> display no shadow (same as if no second value is given)
	//       otherwise -> use given string as color string for the shadow color
	//
	//Prepare window
	GetRgb(hv_WindowHandle, &hv_Red, &hv_Green, &hv_Blue);
	GetPart(hv_WindowHandle, &hv_Row1Part, &hv_Column1Part, &hv_Row2Part, &hv_Column2Part);
	GetWindowExtents(hv_WindowHandle, &hv_RowWin, &hv_ColumnWin, &hv_WidthWin, &hv_HeightWin);
	SetPart(hv_WindowHandle, 0, 0, hv_HeightWin - 1, hv_WidthWin - 1);
	//
	//default settings
	if (0 != (hv_Row == -1))
	{
		hv_Row = 12;
	}
	if (0 != (hv_Column == -1))
	{
		hv_Column = 12;
	}
	if (0 != (hv_Color == HTuple()))
	{
		hv_Color = "";
	}
	//
	hv_String = (("" + hv_String) + "").TupleSplit("\n");
	//
	//Estimate extentions of text depending on font size.
	GetFontExtents(hv_WindowHandle, &hv_MaxAscent, &hv_MaxDescent, &hv_MaxWidth, &hv_MaxHeight);
	if (0 != (hv_CoordSystem == HTuple("window")))
	{
		hv_R1 = hv_Row;
		hv_C1 = hv_Column;
	}
	else
	{
		//Transform image to window coordinates
		hv_FactorRow = (1.*hv_HeightWin) / ((hv_Row2Part - hv_Row1Part) + 1);
		hv_FactorColumn = (1.*hv_WidthWin) / ((hv_Column2Part - hv_Column1Part) + 1);
		hv_R1 = ((hv_Row - hv_Row1Part) + 0.5)*hv_FactorRow;
		hv_C1 = ((hv_Column - hv_Column1Part) + 0.5)*hv_FactorColumn;
	}
	//
	//Display text box depending on text size
	hv_UseShadow = 1;
	hv_ShadowColor = "gray";
	if (0 != (HTuple(hv_Box[0]) == HTuple("true")))
	{
		hv_Box[0] = "#fce9d4";
		hv_ShadowColor = "#f28d26";
	}
	if (0 != ((hv_Box.TupleLength())>1))
	{
		if (0 != (HTuple(hv_Box[1]) == HTuple("true")))
		{
			//Use default ShadowColor set above
		}
		else if (0 != (HTuple(hv_Box[1]) == HTuple("false")))
		{
			hv_UseShadow = 0;
		}
		else
		{
			hv_ShadowColor = ((const HTuple&)hv_Box)[1];
			//Valid color?
			try
			{
				SetColor(hv_WindowHandle, HTuple(hv_Box[1]));
			}
			// catch (Exception) 
			catch (HalconCpp::HException &HDevExpDefaultException)
			{
				HDevExpDefaultException.ToHTuple(&hv_Exception);
				hv_Exception = "Wrong value of control parameter Box[1] (must be a 'true', 'false', or a valid color string)";
				throw HalconCpp::HException(hv_Exception);
			}
		}
	}
	if (0 != (HTuple(hv_Box[0]) != HTuple("false")))
	{
		//Valid color?
		try
		{
			SetColor(hv_WindowHandle, HTuple(hv_Box[0]));
		}
		// catch (Exception) 
		catch (HalconCpp::HException &HDevExpDefaultException)
		{
			HDevExpDefaultException.ToHTuple(&hv_Exception);
			hv_Exception = "Wrong value of control parameter Box[0] (must be a 'true', 'false', or a valid color string)";
			throw HalconCpp::HException(hv_Exception);
		}
		//Calculate box extents
		hv_String = (" " + hv_String) + " ";
		hv_Width = HTuple();
		{
			HTuple end_val93 = (hv_String.TupleLength()) - 1;
			HTuple step_val93 = 1;
			for (hv_Index = 0; hv_Index.Continue(end_val93, step_val93); hv_Index += step_val93)
			{
				GetStringExtents(hv_WindowHandle, HTuple(hv_String[hv_Index]), &hv_Ascent,
					&hv_Descent, &hv_W, &hv_H);
				hv_Width = hv_Width.TupleConcat(hv_W);
			}
		}
		hv_FrameHeight = hv_MaxHeight*(hv_String.TupleLength());
		hv_FrameWidth = (HTuple(0).TupleConcat(hv_Width)).TupleMax();
		hv_R2 = hv_R1 + hv_FrameHeight;
		hv_C2 = hv_C1 + hv_FrameWidth;
		//Display rectangles
		GetDraw(hv_WindowHandle, &hv_DrawMode);
		SetDraw(hv_WindowHandle, "fill");
		//Set shadow color
		SetColor(hv_WindowHandle, hv_ShadowColor);
		if (0 != hv_UseShadow)
		{
			DispRectangle1(hv_WindowHandle, hv_R1 + 1, hv_C1 + 1, hv_R2 + 1, hv_C2 + 1);
		}
		//Set box color
		SetColor(hv_WindowHandle, HTuple(hv_Box[0]));
		DispRectangle1(hv_WindowHandle, hv_R1, hv_C1, hv_R2, hv_C2);
		SetDraw(hv_WindowHandle, hv_DrawMode);
	}
	//Write text.
  {
	  HTuple end_val115 = (hv_String.TupleLength()) - 1;
	  HTuple step_val115 = 1;
	  for (hv_Index = 0; hv_Index.Continue(end_val115, step_val115); hv_Index += step_val115)
	  {
		  hv_CurrentColor = ((const HTuple&)hv_Color)[hv_Index % (hv_Color.TupleLength())];
		  if (0 != (HTuple(hv_CurrentColor != HTuple("")).TupleAnd(hv_CurrentColor != HTuple("auto"))))
		  {
			  SetColor(hv_WindowHandle, hv_CurrentColor);
		  }
		  else
		  {
			  SetRgb(hv_WindowHandle, hv_Red, hv_Green, hv_Blue);
		  }
		  hv_Row = hv_R1 + (hv_MaxHeight*hv_Index);
		  SetTposition(hv_WindowHandle, hv_Row, hv_C1);
		  WriteString(hv_WindowHandle, HTuple(hv_String[hv_Index]));
	  }
  }
  //Reset changed window settings
  SetRgb(hv_WindowHandle, hv_Red, hv_Green, hv_Blue);
  SetPart(hv_WindowHandle, hv_Row1Part, hv_Column1Part, hv_Row2Part, hv_Column2Part);
  return;
}

void set_display_font(HTuple hv_WindowHandle, HTuple hv_Size, HTuple hv_Font, HTuple hv_Bold,
	HTuple hv_Slant)
{

	// Local iconic variables

	// Local control variables
	HTuple  hv_OS, hv_BufferWindowHandle, hv_Ascent;
	HTuple  hv_Descent, hv_Width, hv_Height, hv_Scale, hv_Exception;
	HTuple  hv_SubFamily, hv_Fonts, hv_SystemFonts, hv_Guess;
	HTuple  hv_I, hv_Index, hv_AllowedFontSizes, hv_Distances;
	HTuple  hv_Indices, hv_FontSelRegexp, hv_FontsCourier;

	//This procedure sets the text font of the current window with
	//the specified attributes.
	//It is assumed that following fonts are installed on the system:
	//Windows: Courier New, Arial Times New Roman
	//Mac OS X: CourierNewPS, Arial, TimesNewRomanPS
	//Linux: courier, helvetica, times
	//Because fonts are displayed smaller on Linux than on Windows,
	//a scaling factor of 1.25 is used the get comparable results.
	//For Linux, only a limited number of font sizes is supported,
	//to get comparable results, it is recommended to use one of the
	//following sizes: 9, 11, 14, 16, 20, 27
	//(which will be mapped internally on Linux systems to 11, 14, 17, 20, 25, 34)
	//
	//Input parameters:
	//WindowHandle: The graphics window for which the font will be set
	//Size: The font size. If Size=-1, the default of 16 is used.
	//Bold: If set to 'true', a bold font is used
	//Slant: If set to 'true', a slanted font is used
	//
	GetSystem("operating_system", &hv_OS);
	// dev_get_preferences(...); only in hdevelop
	// dev_set_preferences(...); only in hdevelop
	if (0 != (HTuple(hv_Size == HTuple()).TupleOr(hv_Size == -1)))
	{
		hv_Size = 16;
	}
	if (0 != ((hv_OS.TupleSubstr(0, 2)) == HTuple("Win")))
	{
		//Set font on Windows systems
		try
		{
			//Check, if font scaling is switched on
			OpenWindow(0, 0, 256, 256, 0, "buffer", "", &hv_BufferWindowHandle);
			SetFont(hv_BufferWindowHandle, "-Consolas-16-*-0-*-*-1-");
			GetStringExtents(hv_BufferWindowHandle, "test_string", &hv_Ascent, &hv_Descent,
				&hv_Width, &hv_Height);
			//Expected width is 110
			hv_Scale = 110.0 / hv_Width;
			hv_Size = (hv_Size*hv_Scale).TupleInt();
			CloseWindow(hv_BufferWindowHandle);
		}
		// catch (Exception) 
		catch (HalconCpp::HException &HDevExpDefaultException)
		{
			HDevExpDefaultException.ToHTuple(&hv_Exception);
			//throw (Exception)
		}
		if (0 != (HTuple(hv_Font == HTuple("Courier")).TupleOr(hv_Font == HTuple("courier"))))
		{
			hv_Font = "Courier New";
		}
		else if (0 != (hv_Font == HTuple("mono")))
		{
			hv_Font = "Consolas";
		}
		else if (0 != (hv_Font == HTuple("sans")))
		{
			hv_Font = "Arial";
		}
		else if (0 != (hv_Font == HTuple("serif")))
		{
			hv_Font = "Times New Roman";
		}
		if (0 != (hv_Bold == HTuple("true")))
		{
			hv_Bold = 1;
		}
		else if (0 != (hv_Bold == HTuple("false")))
		{
			hv_Bold = 0;
		}
		else
		{
			hv_Exception = "Wrong value of control parameter Bold";
			throw HalconCpp::HException(hv_Exception);
		}
		if (0 != (hv_Slant == HTuple("true")))
		{
			hv_Slant = 1;
		}
		else if (0 != (hv_Slant == HTuple("false")))
		{
			hv_Slant = 0;
		}
		else
		{
			hv_Exception = "Wrong value of control parameter Slant";
			throw HalconCpp::HException(hv_Exception);
		}
		try
		{
			SetFont(hv_WindowHandle, ((((((("-" + hv_Font) + "-") + hv_Size) + "-*-") + hv_Slant) + "-*-*-") + hv_Bold) + "-");
		}
		// catch (Exception) 
		catch (HalconCpp::HException &HDevExpDefaultException)
		{
			HDevExpDefaultException.ToHTuple(&hv_Exception);
			//throw (Exception)
		}
	}
	else if (0 != ((hv_OS.TupleSubstr(0, 2)) == HTuple("Dar")))
	{
		//Set font on Mac OS X systems. Since OS X does not have a strict naming
		//scheme for font attributes, we use tables to determine the correct font
		//name.
		hv_SubFamily = 0;
		if (0 != (hv_Slant == HTuple("true")))
		{
			hv_SubFamily = hv_SubFamily | 1;
		}
		else if (0 != (hv_Slant != HTuple("false")))
		{
			hv_Exception = "Wrong value of control parameter Slant";
			throw HalconCpp::HException(hv_Exception);
		}
		if (0 != (hv_Bold == HTuple("true")))
		{
			hv_SubFamily = hv_SubFamily | 2;
		}
		else if (0 != (hv_Bold != HTuple("false")))
		{
			hv_Exception = "Wrong value of control parameter Bold";
			throw HalconCpp::HException(hv_Exception);
		}
		if (0 != (hv_Font == HTuple("mono")))
		{
			hv_Fonts.Clear();
			hv_Fonts[0] = "Menlo-Regular";
			hv_Fonts[1] = "Menlo-Italic";
			hv_Fonts[2] = "Menlo-Bold";
			hv_Fonts[3] = "Menlo-BoldItalic";
		}
		else if (0 != (HTuple(hv_Font == HTuple("Courier")).TupleOr(hv_Font == HTuple("courier"))))
		{
			hv_Fonts.Clear();
			hv_Fonts[0] = "CourierNewPSMT";
			hv_Fonts[1] = "CourierNewPS-ItalicMT";
			hv_Fonts[2] = "CourierNewPS-BoldMT";
			hv_Fonts[3] = "CourierNewPS-BoldItalicMT";
		}
		else if (0 != (hv_Font == HTuple("sans")))
		{
			hv_Fonts.Clear();
			hv_Fonts[0] = "ArialMT";
			hv_Fonts[1] = "Arial-ItalicMT";
			hv_Fonts[2] = "Arial-BoldMT";
			hv_Fonts[3] = "Arial-BoldItalicMT";
		}
		else if (0 != (hv_Font == HTuple("serif")))
		{
			hv_Fonts.Clear();
			hv_Fonts[0] = "TimesNewRomanPSMT";
			hv_Fonts[1] = "TimesNewRomanPS-ItalicMT";
			hv_Fonts[2] = "TimesNewRomanPS-BoldMT";
			hv_Fonts[3] = "TimesNewRomanPS-BoldItalicMT";
		}
		else
		{
			//Attempt to figure out which of the fonts installed on the system
			//the user could have meant.
			QueryFont(hv_WindowHandle, &hv_SystemFonts);
			hv_Fonts.Clear();
			hv_Fonts.Append(hv_Font);
			hv_Fonts.Append(hv_Font);
			hv_Fonts.Append(hv_Font);
			hv_Fonts.Append(hv_Font);
			hv_Guess.Clear();
			hv_Guess.Append(hv_Font);
			hv_Guess.Append(hv_Font + "-Regular");
			hv_Guess.Append(hv_Font + "MT");
			{
				HTuple end_val100 = (hv_Guess.TupleLength()) - 1;
				HTuple step_val100 = 1;
				for (hv_I = 0; hv_I.Continue(end_val100, step_val100); hv_I += step_val100)
				{
					TupleFind(hv_SystemFonts, HTuple(hv_Guess[hv_I]), &hv_Index);
					if (0 != (hv_Index != -1))
					{
						hv_Fonts[0] = HTuple(hv_Guess[hv_I]);
						break;
					}
				}
			}
			//Guess name of slanted font
			hv_Guess.Clear();
			hv_Guess.Append(hv_Font + "-Italic");
			hv_Guess.Append(hv_Font + "-ItalicMT");
			hv_Guess.Append(hv_Font + "-Oblique");
			{
				HTuple end_val109 = (hv_Guess.TupleLength()) - 1;
				HTuple step_val109 = 1;
				for (hv_I = 0; hv_I.Continue(end_val109, step_val109); hv_I += step_val109)
				{
					TupleFind(hv_SystemFonts, HTuple(hv_Guess[hv_I]), &hv_Index);
					if (0 != (hv_Index != -1))
					{
						hv_Fonts[1] = HTuple(hv_Guess[hv_I]);
						break;
					}
				}
			}
			//Guess name of bold font
			hv_Guess.Clear();
			hv_Guess.Append(hv_Font + "-Bold");
			hv_Guess.Append(hv_Font + "-BoldMT");
			{
				HTuple end_val118 = (hv_Guess.TupleLength()) - 1;
				HTuple step_val118 = 1;
				for (hv_I = 0; hv_I.Continue(end_val118, step_val118); hv_I += step_val118)
				{
					TupleFind(hv_SystemFonts, HTuple(hv_Guess[hv_I]), &hv_Index);
					if (0 != (hv_Index != -1))
					{
						hv_Fonts[2] = HTuple(hv_Guess[hv_I]);
						break;
					}
				}
			}
			//Guess name of bold slanted font
			hv_Guess.Clear();
			hv_Guess.Append(hv_Font + "-BoldItalic");
			hv_Guess.Append(hv_Font + "-BoldItalicMT");
			hv_Guess.Append(hv_Font + "-BoldOblique");
			{
				HTuple end_val127 = (hv_Guess.TupleLength()) - 1;
				HTuple step_val127 = 1;
				for (hv_I = 0; hv_I.Continue(end_val127, step_val127); hv_I += step_val127)
				{
					TupleFind(hv_SystemFonts, HTuple(hv_Guess[hv_I]), &hv_Index);
					if (0 != (hv_Index != -1))
					{
						hv_Fonts[3] = HTuple(hv_Guess[hv_I]);
						break;
					}
				}
			}
		}
		hv_Font = ((const HTuple&)hv_Fonts)[hv_SubFamily];
		try
		{
			SetFont(hv_WindowHandle, (hv_Font + "-") + hv_Size);
		}
		// catch (Exception) 
		catch (HalconCpp::HException &HDevExpDefaultException)
		{
			HDevExpDefaultException.ToHTuple(&hv_Exception);
			//throw (Exception)
		}
	}
	else
	{
		//Set font for UNIX systems
		hv_Size = hv_Size*1.25;
		hv_AllowedFontSizes.Clear();
		hv_AllowedFontSizes[0] = 11;
		hv_AllowedFontSizes[1] = 14;
		hv_AllowedFontSizes[2] = 17;
		hv_AllowedFontSizes[3] = 20;
		hv_AllowedFontSizes[4] = 25;
		hv_AllowedFontSizes[5] = 34;
		if (0 != ((hv_AllowedFontSizes.TupleFind(hv_Size)) == -1))
		{
			hv_Distances = (hv_AllowedFontSizes - hv_Size).TupleAbs();
			TupleSortIndex(hv_Distances, &hv_Indices);
			hv_Size = ((const HTuple&)hv_AllowedFontSizes)[HTuple(hv_Indices[0])];
		}
		if (0 != (HTuple(hv_Font == HTuple("mono")).TupleOr(hv_Font == HTuple("Courier"))))
		{
			hv_Font = "courier";
		}
		else if (0 != (hv_Font == HTuple("sans")))
		{
			hv_Font = "helvetica";
		}
		else if (0 != (hv_Font == HTuple("serif")))
		{
			hv_Font = "times";
		}
		if (0 != (hv_Bold == HTuple("true")))
		{
			hv_Bold = "bold";
		}
		else if (0 != (hv_Bold == HTuple("false")))
		{
			hv_Bold = "medium";
		}
		else
		{
			hv_Exception = "Wrong value of control parameter Bold";
			throw HalconCpp::HException(hv_Exception);
		}
		if (0 != (hv_Slant == HTuple("true")))
		{
			if (0 != (hv_Font == HTuple("times")))
			{
				hv_Slant = "i";
			}
			else
			{
				hv_Slant = "o";
			}
		}
		else if (0 != (hv_Slant == HTuple("false")))
		{
			hv_Slant = "r";
		}
		else
		{
			hv_Exception = "Wrong value of control parameter Slant";
			throw HalconCpp::HException(hv_Exception);
		}
		try
		{
			SetFont(hv_WindowHandle, ((((((("-adobe-" + hv_Font) + "-") + hv_Bold) + "-") + hv_Slant) + "-normal-*-") + hv_Size) + "-*-*-*-*-*-*-*");
		}
		// catch (Exception) 
		catch (HalconCpp::HException &HDevExpDefaultException)
		{
			HDevExpDefaultException.ToHTuple(&hv_Exception);
			if (0 != (HTuple((hv_OS.TupleSubstr(0, 4)) == HTuple("Linux")).TupleAnd(hv_Font == HTuple("courier"))))
			{
				QueryFont(hv_WindowHandle, &hv_Fonts);
				hv_FontSelRegexp = (("^-[^-]*-[^-]*[Cc]ourier[^-]*-" + hv_Bold) + "-") + hv_Slant;
				hv_FontsCourier = (hv_Fonts.TupleRegexpSelect(hv_FontSelRegexp)).TupleRegexpMatch(hv_FontSelRegexp);
				if (0 != ((hv_FontsCourier.TupleLength()) == 0))
				{
					hv_Exception = "Wrong font name";
					//throw (Exception)
				}
				else
				{
					try
					{
						SetFont(hv_WindowHandle, ((HTuple(hv_FontsCourier[0]) + "-normal-*-") + hv_Size) + "-*-*-*-*-*-*-*");
					}
					// catch (Exception) 
					catch (HalconCpp::HException &HDevExpDefaultException)
					{
						HDevExpDefaultException.ToHTuple(&hv_Exception);
						//throw (Exception)
					}
				}
			}
			//throw (Exception)
		}
	}
	// dev_set_preferences(...); only in hdevelop
	return;
}

IplImage *HImageToIplImage(HObject &Hobj){
	IplImage *pImage = NULL;
	HObject Hobj_Converted;
	HTuple htChannels, width = 0, height = 0, cType;
	int w, h;

	//start to convert
	ConvertImageType(Hobj, &Hobj_Converted, "byte");
	CountChannels(Hobj_Converted, &htChannels);

	if (htChannels[0].I() == 1){
		HTuple ptr;
		GetImagePointer1(Hobj_Converted, &ptr, &cType, &width, &height);
		w = width[0].I(), h = width[0].I();
		pImage = cvCreateImage(cvSize(w,h), IPL_DEPTH_8U, 1);
		for (int i = 0; i < h; i++){
			memcpy(pImage->imageData + pImage->widthStep*i, (BYTE *)ptr[0].L() + w*i, w);
		}
	}

	if (htChannels[0].I() == 3){
		HTuple ptrR, ptrG, ptrB;

		GetImagePointer3(Hobj_Converted, &ptrR, &ptrG, &ptrB, &cType, &width, &height);

		w = width[0].I();
		h = height[0].I();

		IplImage *pImageR, *pImageG, *pImageB;
		pImage = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, 3);
		pImageR = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, 1);
		pImageG = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, 1);
		pImageB = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, 1);

		/*=================================================================================================
		copy rule as below:
		i:0  #widthStep
		i:1  #
		i:2  #
		i:3  #
		......
		i:h  #
		==================================================================================================*/
		for (int i = 0; i < h; i++){
			std::cout << "current iR:" << i << std::endl;
			memcpy(pImageR->imageData + pImageR->widthStep*i, (unsigned char*)ptrR[0].L() + w*i, w);
			std::cout << "current iG:" << i << std::endl;
			memcpy(pImageG->imageData + pImageG->widthStep*i, (unsigned char*)ptrG[0].L() + w*i, w);
			std::cout << "current iB:" << i << std::endl;
			memcpy(pImageB->imageData + pImageB->widthStep*i, (unsigned char*)ptrB[0].L() + w*i, w);
		}

		cvMerge(pImageB, pImageG, pImageR, NULL, pImage);
		cvReleaseImage(&pImageR);
		cvReleaseImage(&pImageG);
		cvReleaseImage(&pImageB);
	}
	return pImage;
}

HObject IplImageToHImage(IplImage *pImage){
	HObject hobj;
	HTuple gray, w, h;
	int height = pImage->height;
	int width = pImage->width;
	int halconErrCode;

	w[0] = width;
	h[0] = height;

	if (3 == pImage->nChannels){
		IplImage *pImageR, *pImageG, *pImageB;
		pImageR = cvCreateImage(cvGetSize(pImage), IPL_DEPTH_8U, 1);
		pImageG = cvCreateImage(cvGetSize(pImage), IPL_DEPTH_8U, 1);
		pImageB = cvCreateImage(cvGetSize(pImage), IPL_DEPTH_8U, 1);

		cvSplit(pImage, pImageB, pImageG, pImageR, NULL);
		BYTE *pDataR = new BYTE[width*height];
		BYTE *pDataG = new BYTE[width*height];
		BYTE *pDataB = new BYTE[width*height];

		for (int i = 0; i < height; i++){
			memcpy(pDataR + width*i, pImageR->imageData + pImageR->widthStep*i, width);
			memcpy(pDataG + width*i, pImageG->imageData + pImageG->widthStep*i, width);
			memcpy(pDataB + width*i, pImageB->imageData + pImageB->widthStep*i, width);
		}		

		try{
			GenImage3(&hobj, "byte", w, h, (Hlong)pDataR, (Hlong)pDataG, (Hlong)pDataB);
		}
		catch (HOperatorException e){
			cout << "Error:" << e.ErrorMessage() << endl;
			halconErrCode = e.ErrorNumber();
		}
		
		cvReleaseImage(&pImageR);
		cvReleaseImage(&pImageG);
		cvReleaseImage(&pImageB);

		delete[] pDataR;
		delete[] pDataG;
		delete[] pDataB;
	}
	if (1 == pImage->nChannels){
		BYTE *pDataGray = new BYTE[width*height];

		for (int i = 0; i < height; i++){
			memcpy(pDataGray + width*i, pImage->imageData + pImage->widthStep*i,width);
		}

		try{
			GenImage1(&hobj, "byte", w, h, (Hlong)pDataGray);
		}
		catch (HOperatorException e){
			cout << "Error:" << e.ErrorMessage() << endl;
			halconErrCode = e.ErrorNumber();
		}
		delete[] pDataGray;
	}

	return hobj;
}

string GetCurrentDir(){
	TCHAR exeFullPath[256] = {0};
	string strPath;


	GetModuleFileName(NULL, exeFullPath, 255);
	(_tcsrchr(exeFullPath, _T('\\')))[1] = 0;
	for (int i = 0; exeFullPath[i]; i++){
		if (exeFullPath[i] != _T('\\')){
			strPath += exeFullPath[i];
		}
		else{
			strPath += _T('\\\\');
		}
	}
	return strPath;
}
void action()
{
START:

	// Local iconic variables
	HObject  ho_Image_ori, ho_Img_m, ho_GrayImage;
	HObject  ho_Region, ho_RegionClosing, ho_BinImage, ho_ImageInvert;
	HObject  ho_SymbolRegions, ho_Characters;

	// Local control variables
	HTuple  hv_BarCodeHandle, hv_Wo, hv_Ho, hv_WindowID;
	HTuple  hv_Width, hv_Height, hv_DecodedDataStrings, hv_bar_code_xy;
	HTuple  hv_Area1, hv_Row1, hv_Column1, hv_TextModel, hv_TextResultID;
	HTuple  hv_ResultValue, hv_Area, hv_Row, hv_Column, hv_Index;

	//init icon
	HObject icon_good, icon_bad;
	HTuple fileName,HicB,HicG;
	string workingPath,fullpath,fname,icBad,icGood;

	workingPath = GetCurrentDir();

REENTER:
	cout << "请输入文件名(带后缀名)：";
	fname.clear();
	fullpath.clear();
	cin >>fname;
	fullpath += workingPath;
	fullpath += fname;
	cout << "full dir name is:"<<fullpath << endl;
	//check file
	ifstream in(fullpath.c_str());
	if (!in){
		cout << "文件不存在" << endl;
		goto REENTER;
	}
	fileName = fullpath.c_str();
	cout << fileName[0].C() << endl;

	icBad += workingPath, icBad += "bad-icon.jpg";
	icGood += workingPath, icGood += "good-icon.jpg";

	HicB = icBad.c_str();
	HicG = icGood.c_str();
	try{
		ReadImage(&icon_bad, HicB);
	}
	catch(HOperatorException e){
		cout << "ReadImage exception:" << e.ErrorMessage() << endl;
		cout << "ReadImage exception:" << e.ErrorCode() << endl;
		return;
	}
	ReadImage(&icon_good, HicG);
	//end
	
	//OCR init begin
	CreateBarCodeModel(HTuple(), HTuple(), &hv_BarCodeHandle);
	//OCR init end
	//stop ()
	//WaitSeconds(1);
	ReadImage(&ho_Image_ori, fileName);
	//test HObj to IplImage
	/*cvShowImage("TestHtI", HImageToIplImage(ho_Image_ori));
	cvWaitKey(0);*/
	//end

	//test IplImage to HObj
	/*IplImage *tttt = cvLoadImage("D:\\work\\Halcon\\bar-code-result-finder_v0.1\\x64\\Release\\gray.bmp",0);
	HObject test = IplImageToHImage(tttt);
	*/
	//after open window, the code below can run normally.

	//end

	GetImageSize(ho_Image_ori, &hv_Wo, &hv_Ho);
	OpenWindow(0, 0, hv_Wo, hv_Ho, 0, "", "", &hv_WindowID);
	SetWindowAttr("background_color", "black");

	HDevWindowStack::Push(hv_WindowID);
	set_display_font(hv_WindowID, 18, "mono", "true", "false");
	if (HDevWindowStack::IsOpen())
		SetDraw(HDevWindowStack::GetActive(), "margin");
	if (HDevWindowStack::IsOpen())
		SetLineWidth(HDevWindowStack::GetActive(), 3);

	ZoomImageSize(ho_Image_ori, &ho_Img_m, hv_Wo, hv_Ho, "constant");
	//equ_histo_image (Image_z, Image)
	//mean_image (Image_z, Img_m, 3, 3)
	GetImageSize(ho_Img_m, &hv_Width, &hv_Height);
	Rgb1ToGray(ho_Img_m, &ho_GrayImage);
	Threshold(ho_GrayImage, &ho_Region, 0, 100);
	ClosingCircle(ho_Region, &ho_RegionClosing, 1.0);
	RegionToBin(ho_RegionClosing, &ho_BinImage, 255, 0, hv_Width, hv_Height);
	InvertImage(ho_BinImage, &ho_ImageInvert);
	//stop ()
	//find bar code begin
	SetBarCodeParam(hv_BarCodeHandle, "check_char", "present");
	FindBarCode(ho_ImageInvert, &ho_SymbolRegions, hv_BarCodeHandle, "auto", &hv_DecodedDataStrings);
	RegionFeatures(ho_SymbolRegions, (((HTuple("row1").Append("column1")).Append("width")).Append("height")),
		&hv_bar_code_xy);
	//print bar code region x/y
	int rx, ry, rw, rh;
	for (int i = 0; i < 4; i++){
		cout << "hv_bar_code_xy[" << i << "]" << ":" << hv_bar_code_xy[i].D() << endl;
	}
	ry = hv_bar_code_xy[0].D();
	rx = hv_bar_code_xy[1].D();
	rw = hv_bar_code_xy[2].D();
	rh = hv_bar_code_xy[3].D();
	IplImage *pTestRegion = HImageToIplImage(ho_Image_ori);
	cvRectangle(pTestRegion, cvPoint(rx, ry), cvPoint(rx + rw, ry + rh), cvScalar(203, 192, 255), 8);
	cvShowImage("TestRegion", pTestRegion);
	cvWaitKey(1);
	
	//end print
	AreaCenter(ho_SymbolRegions, &hv_Area1, &hv_Row1, &hv_Column1);
	//find bar code end
	//stop ()

	if (HDevWindowStack::IsOpen())
		DispObj(ho_Img_m, HDevWindowStack::GetActive());
	if (HDevWindowStack::IsOpen())
		DispObj(ho_SymbolRegions, HDevWindowStack::GetActive());
	CreateTextModelReader("auto", "Industrial_0-9_NoRej.omc", &hv_TextModel);
	SetTextModelParam(hv_TextModel, "text_line_structure", "8 2");
	FindText(ho_GrayImage, hv_TextModel, &hv_TextResultID);
	GetTextObject(&ho_Characters, hv_TextResultID, "all_lines");
	GetTextResult(hv_TextResultID, "class", &hv_ResultValue);
	AreaCenter(ho_Characters, &hv_Area, &hv_Row, &hv_Column);

	//disp icon	
	bool icon_flag = true;
	char *pD = (char *)hv_DecodedDataStrings[0].C();
	char *p = NULL;
	int errIndex= -1;
	for (int i = 0; i < 10; i++){
		if (8 == i){
			cout << "skip the '-' " << endl;
			continue;
		}
		p = (char *)hv_ResultValue[i].C();
		if (*p != *(pD + i)){
			cout << "i:" << i << " p " << *p << " pD " << *(pD + i) << endl;
			icon_flag = false;
			errIndex = i;
			break;
		}
	}

	disp_message(hv_WindowID, hv_DecodedDataStrings, "image", hv_Row1 - 70, hv_Column1 - 50,
		"red", "true");
	//WaitSeconds(1);
	if (HDevWindowStack::IsOpen())
		if (icon_flag){
			DispObj(icon_good, HDevWindowStack::GetActive());
		}
		else{
			DispObj(icon_bad, HDevWindowStack::GetActive());
		}
	{
		HTuple end_val41 = (hv_ResultValue.TupleLength()) - 1;
		HTuple step_val41 = 1;
		for (hv_Index = 0; hv_Index.Continue(end_val41, step_val41); hv_Index += step_val41)
		{
			if (errIndex == hv_Index){
				disp_message(hv_WindowID, HTuple(hv_ResultValue[hv_Index]), "image", HTuple(hv_Row[hv_Index]) + 20,
					HTuple(hv_Column[hv_Index]) + (hv_Index * 3), "red", "false");
			}
			else{
				disp_message(hv_WindowID, HTuple(hv_ResultValue[hv_Index]), "image", HTuple(hv_Row[hv_Index]) + 20,
					HTuple(hv_Column[hv_Index]) + (hv_Index * 3), "blue", "false");
			}
		}
	}
	// stop(); only in hdevelop
	ClearTextResult(hv_TextResultID);
	ClearTextModel(hv_TextModel);
	system("pause");
}
bool roiAddImage(IplImage *pDst,bool flag){
	IplImage *p = NULL;
	if (false == flag){
		string path = GetCurrentDir() + "bad-icon.jpg";
		p = cvLoadImage(path.c_str());
	}
	else{
		string path = GetCurrentDir() + "good-icon.jpg";
		p = cvLoadImage(path.c_str());
	}

	return true;
}
int drawBarcodeRect(IplImage *pI, vector<BarcodeRect> *t){
	string label;
	CvFont font;
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX | CV_FONT_ITALIC, 5, 5,0, 20, 2);

	for (int i = 0; i < t->capacity(); i++){
		label.clear();
		label = i + 48;
		cvRectangle(pI,
			cvPoint(t->at(i).column, t->at(i).row),
			cvPoint(t->at(i).column + t->at(i).width, t->at(i).row + t->at(i).height),
			cvScalar(0, 0, 255), 5);
		//set label
		cvPutText(pI, label.c_str(), cvPoint(t->at(i).column, t->at(i).row), &font, CV_RGB(0, 255, 0));
	}
	return 1;
}
int checkBarCode(IplImage *pOriImg, IplImage **pDstImg, void **pCommonData){
	int ret = false;
	bool resultFlag = true;
	IplImage *pResultFlagIcon = NULL;
	//HO
	HObject ho_Img, ho_GrayImage, ho_Region, ho_RegionClosing;
	HObject ho_BinImage, ho_ImageInvert, ho_SymbolRegions, ho_Characters, ho_c;
	//HV
	HTuple hv_Width, hv_Height;
	HTuple hv_BarCodeHandle, hv_DecodedDataStrings, hv_bar_code_xy;
	HTuple hv_TextModel, hv_TextResultID, hv_ResultValue, hv_Area, hv_Row, hv_Column,hv_Numbers_xy;

	//clone ori image
	*pDstImg = cvCloneImage(pOriImg);
	//first convert OpenCV ipl to Halcon type
	ho_Img = IplImageToHImage(*pDstImg);
	
	//do halcon operation
	GetImageSize(ho_Img, &hv_Width, &hv_Height);
	Rgb1ToGray(ho_Img, &ho_GrayImage);
	Threshold(ho_GrayImage, &ho_Region, 0, 100);
	ClosingCircle(ho_Region, &ho_RegionClosing, 1.0);
	RegionToBin(ho_RegionClosing, &ho_BinImage, 255, 0, hv_Width, hv_Height);
	InvertImage(ho_BinImage, &ho_ImageInvert);

	//find bar code begin
	CreateBarCodeModel(HTuple(), HTuple(), &hv_BarCodeHandle);
	SetBarCodeParam(hv_BarCodeHandle, "check_char", "present");
	FindBarCode(ho_ImageInvert, &ho_SymbolRegions, hv_BarCodeHandle, "auto", &hv_DecodedDataStrings);
	RegionFeatures(ho_SymbolRegions, (((HTuple("row1").Append("column1")).Append("width")).Append("height")),
		&hv_bar_code_xy);

	//OCR
	CreateTextModelReader("auto", "Industrial_0-9_NoRej.omc", &hv_TextModel);
	SetTextModelParam(hv_TextModel, "text_line_structure", "8 2");
	FindText(ho_GrayImage, hv_TextModel, &hv_TextResultID);
	GetTextObject(&ho_Characters, hv_TextResultID, "all_lines");
	GetTextResult(hv_TextResultID, "class", &hv_ResultValue);
	AreaCenter(ho_Characters, &hv_Area, &hv_Row, &hv_Column);
	RegionFeatures(ho_Characters, (((HTuple("row1").Append("column1")).Append("width")).Append("height")),
		&hv_Numbers_xy);

	//print bar code decoded data string and store in *pCommonData
	cout << "Type is:" << hv_DecodedDataStrings[0].Type() << endl;
	char *pD = (char *)hv_DecodedDataStrings[0].C();
	if (pCommonData != NULL){
		*pCommonData = new BYTE[strlen(pD)+1];
		memcpy(*pCommonData, pD, strlen(pD)+1);
	}
	cout << "DecodedString is:" << pD << endl;

	int rx, ry, rw, rh;
	char *pTemp = pD;
	CvScalar cor;

	for (int i = 0,j=0; i < hv_Numbers_xy.Length(); i+=4,j++){
		ry = hv_Numbers_xy[0 + i].D();
		rx = hv_Numbers_xy[1 + i].D();
		rw = hv_Numbers_xy[2 + i].D();
		rh = hv_Numbers_xy[3 + i].D();

		if (*pTemp == '-'){
			pTemp++;
		}
		char *p = (char *)hv_ResultValue[j].C();
		if (*p == *pTemp){
			cor = cvScalar(0,255,0);
		}
		else{
			cor = cvScalar(0,0,255);
			resultFlag = false;
		}
		pTemp++;

		cvRectangle(*pDstImg, cvPoint(rx, ry), cvPoint(rx + rw, ry + rh), cor, 1);
	}

#if 0
	RegionToBin(ho_Characters, &ho_c, 255, 0, hv_Width, hv_Height);
	cvShowImage("TT", HImageToIplImage(ho_c));
	cvWaitKey(0);
	cvDestroyWindow("TT");

	for (int i = 0; i < hv_Row.Length(); i++){
		cout << "Row [" << i << "]" << hv_Row[i].D() << endl;
		cout << "Col [" << i << "]" << hv_Column[i].D() << endl;
	}

	//print bar code region x/y
	for (int i = 0; i < 4; i++){
		cout << "hv_bar_code_xy[" << i << "]" << ":" << hv_bar_code_xy[i].D() << endl;
	}
#endif
	ry = hv_bar_code_xy[0].D();
	rx = hv_bar_code_xy[1].D();
	rw = hv_bar_code_xy[2].D();
	rh = hv_bar_code_xy[3].D();

	//label the bar code region
	cvRectangle(*pDstImg, cvPoint(rx, ry), cvPoint(rx + rw, ry + rh), cvScalar(0, 0, 255), 2);

	//disp the bar code string
	CvFont font;
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX | CV_FONT_ITALIC, 1, 1, 0, 2);
	cvPutText(*pDstImg, (const char*)(pD), cvPoint(rx, ry), &font, CV_RGB(0, 255, 0));

	//add result flag icon
	if (false == resultFlag){
		string path = GetCurrentDir() + "bad-icon.jpg";
		pResultFlagIcon = cvLoadImage(path.c_str());
	}
	else{
		string path = GetCurrentDir() + "good-icon.jpg";
		pResultFlagIcon = cvLoadImage(path.c_str());
	}

	



#ifdef OPENCV_UI_DBG
	cvShowImage("Test", *pDstImg);
	cvWaitKey(0);
	cvDestroyWindow("Test");
#endif

	return ret;
}
int checkPhonePackageBarcode(IplImage *pOriImg0, int barcodeIndex0, 
							IplImage *pOriImg1, int barcodeIndex1, 
							IplImage **pDstImg, void **pCommandData){
	//vector store barcode string and barcode x.y
	vector<BarcodeRect> barVec0,barVec1;

	findBarcodeByIndex(pOriImg0, barcodeIndex0, &barVec0);
	findBarcodeByIndex(pOriImg1, barcodeIndex1, &barVec1);

	drawBarcodeRect(pOriImg0, &barVec0);
	drawBarcodeRect(pOriImg1, &barVec1);

	//merge two pics
	*pDstImg = cvCreateImage(cvSize(pOriImg0->width + pOriImg1->width, (pOriImg0->height>=pOriImg1->height?pOriImg0->height:pOriImg1->height)),
		IPL_DEPTH_8U, 3);
	CvRect rect = cvRect(0, 0, pOriImg0->width, pOriImg0->height);
	cvSetImageROI(*pDstImg, rect);
	cvCopy(pOriImg0, *pDstImg);

	cvResetImageROI(*pDstImg);
	rect = cvRect(pOriImg1->width, 0, pOriImg1->width, pOriImg1->height);
	cvSetImageROI(*pDstImg, rect);
	cvCopy(pOriImg1, *pDstImg);

	cvResetImageROI(*pDstImg);

	return 1;

}
int findBarcodeByIndex(IplImage *pOriImg, int index, vector<BarcodeRect> *t){
	int ret = false;
	BarcodeRect rect;

	//HO
	HObject ho_Img, ho_GrayImage, ho_Region, ho_RegionClosing;
	HObject ho_BinImage, ho_ImageInvert, ho_SymbolRegions, ho_Characters;
	//HV
	HTuple hv_Width, hv_Height;
	HTuple hv_BarCodeHandle, hv_DecodedDataStrings, hv_bar_code_xy;
	
	//first convert OpenCV ipl to Halcon type
	ho_Img = IplImageToHImage(pOriImg);

	//do halcon operation
	GetImageSize(ho_Img, &hv_Width, &hv_Height);
	Rgb1ToGray(ho_Img, &ho_GrayImage);
	Threshold(ho_GrayImage, &ho_Region, 0, 100);
	ClosingCircle(ho_Region, &ho_RegionClosing, 1.0);
	RegionToBin(ho_RegionClosing, &ho_BinImage, 255, 0, hv_Width, hv_Height);
	InvertImage(ho_BinImage, &ho_ImageInvert);

	//find bar code begin
	CreateBarCodeModel(HTuple(), HTuple(), &hv_BarCodeHandle);
	SetBarCodeParam(hv_BarCodeHandle, "check_char", "present");
	FindBarCode(ho_ImageInvert, &ho_SymbolRegions, hv_BarCodeHandle, "auto", &hv_DecodedDataStrings);
	RegionFeatures(ho_SymbolRegions, (((HTuple("row1").Append("column1")).Append("width")).Append("height")),
		&hv_bar_code_xy);

	if (index >= hv_DecodedDataStrings.Length()){
		cout << "Index out of bound" << " " << "range is 0 ~ " << hv_DecodedDataStrings.Length() << endl;
		return -1;
	}
	if (-1 == index){
		for (int i = 0; i < hv_DecodedDataStrings.Length();i++){
			memset(&rect, 0, sizeof(rect));
			rect.decoded_str = hv_DecodedDataStrings[i].S();
			rect.row = hv_bar_code_xy[4 * i + 0].D();
			rect.column = hv_bar_code_xy[4 * i + 1].D();
			rect.width = hv_bar_code_xy[4 * i + 2].D();
			rect.height = hv_bar_code_xy[4 * i + 3].D();

			t->push_back(rect);
		}
		return 1;
	}

	
	rect.decoded_str = hv_DecodedDataStrings[index].S();
	rect.row	= hv_bar_code_xy[4 * index +0].D();
	rect.column = hv_bar_code_xy[4 * index +1].D();
	rect.width	= hv_bar_code_xy[4 * index +2].D();
	rect.height = hv_bar_code_xy[4 * index +3].D();

#ifdef OPENCV_UI_DBG
	cvRectangle(pOriImg, 
			cvPoint(rect.column, rect.row), 
			cvPoint(rect.column + rect.width, rect.row + rect.height), 
			cvScalar(0, 0, 255), 5);
	cvNamedWindow("Bar", CV_WINDOW_NORMAL);
	cvShowImage("Bar", pOriImg);
	cvWaitKey(0);
#endif
	//x.y
	cout << "hv_bar_code_xy :" << hv_bar_code_xy.Length() << endl;
	cout << "hv_bar_code_xy: row, column, width, height" << endl;
	for (int i = 0; i < 4; i++){
		cout << hv_bar_code_xy[4 * index + i].D() << " ";
	}
	t->push_back(rect);
	return 1;
}