/*
Plugin of Miranda IM for reading/writing PNG images.
Copyright (c) 2004-5 George Hazan (ghazan@postman.ru)

Portions of this code are gotten from the libpng codebase.
Copyright 2000, Willem van Schaik.  For conditions of distribution and
use, see the copyright/license/disclaimer notice in png.h

Miranda IM: the free icq client for MS Windows
Copyright (C) 2000-2002 Richard Hughes, Roland Rabien & Tristan Van de Vreede

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

File name      : $Source: /cvsroot/miranda/miranda/include/m_png.h,v $
Revision       : $Revision: 2876 $
Last change on : $Date: 2006-05-16 17:56:03 -0400 (Tue, 16 May 2006) $
Last change by : $Author: ghazan $

*/

/* Image/Dib2Png
Converts a Device Independent Bitmap to a png stored in memory
  wParam=0
  lParam=(WPARAM)(DIB2PNG*)descr
*/

typedef struct
{
	BITMAPINFO* pbmi;
	BYTE* pDiData;
	BYTE* pResult;
	long* pResultLen;
}
   DIB2PNG;

#define MS_DIB2PNG "Image/Dib2Png"

/* Image/Png2Dib
Converts a png stored in memory to a Device Independent Bitmap
  wParam=0
  lParam=(WPARAM)(PNG2DIB*)descr
*/

typedef struct
{
	BYTE* pSource;
	DWORD cbSourceSize;
	BITMAPINFOHEADER** pResult;
}
   PNG2DIB;

#define MS_PNG2DIB "Image/Png2Dib"
