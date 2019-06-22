/*  Copyright 2011 AIT Austrian Institute of Technology
*
*   This file is part of OpenTLD.
*
*   OpenTLD is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   OpenTLD is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with OpenTLD.  If not, see <http://www.gnu.org/licenses/>.
*
*/

/*
 * util.h
 *
 *  Created on: 30.05.2011
 *      Author: Georg Nebehay
 */

#ifndef TLDUTIL_H_
#define TLDUTIL_H_

#include <utility>
#include <vector>
#include "format.h"

#include <opencv/cv.h>
#include <opencv2/opencv.hpp>

template <class T1, class T2>
void tldConvertBB(T1 *src, T2 *dest)
{
    dest[0] = src[0];
    dest[1] = src[1];
    dest[2] = src[2];
    dest[3] = src[3];
}

template <class T>
void tldCopyBB(T *src, T *dest)
{
    tldConvertBB<T, T>(src, dest);
}

template <class T>
void tldCopyBoundaryToArray(T x, T y, T width, T height, T *array)
{
    array[0] = x;
    array[1] = y;
    array[2] = width;
    array[3] = height;
}

template <class T>
void tldExtractDimsFromArray(T *boundary, T *x, T *y, T *width, T *height)
{
    *x = boundary[0];
    *y = boundary[1];
    *width = boundary[2];
    *height = boundary[3];
}

template <class T>
void tldRectToArray(cv::Rect rect, T *boundary)
{
    boundary[0] = rect.x;
    boundary[1] = rect.y;
    boundary[2] = rect.width;
    boundary[3] = rect.height;
}

template <class T>
cv::Rect tldArrayToRect(T *boundary)
{
    cv::Rect rect;
    rect.x = boundary[0];
    rect.y = boundary[1];
    rect.width = boundary[2];
    rect.height = boundary[3];

    return rect;
}

void tldRectToPoints(cv::Rect rect, CvPoint *p1, CvPoint *p2);

void tldBoundingBoxToPoints(int *bb, CvPoint *p1, CvPoint *p2);


//Returns mean-normalized patch, image must be greyscale
void tldNormalizeImg(const cv::Mat &img, float *output);

CvRect tldBoundaryToRect(int *boundary);

void tldExtractSubImage(const cv::Mat &img, cv::Mat &subImage, CvRect rect);

void tldExtractSubImage(const cv::Mat &img, cv::Mat &subImage, int *boundary);

void tldExtractNormalizedPatch(const cv::Mat &img, int x, int y, int w, int h, float *output);

//TODO: Rename
void tldExtractNormalizedPatchBB(const cv::Mat &img, int *boundary, float *output);

void tldExtractNormalizedPatchRect(const cv::Mat &img, cv::Rect *rect, float *output);

float CalculateMean(float *value, int n);

float tldCalcVariance(float *value, int n);


float tldBBOverlap(int *bb1, int *bb2);

void tldOverlap(int *windows, int numWindows, int *boundary, float *overlap);

void tldOverlapOne(int *windows, int numWindows, int index, std::vector<int> * indices, float *overlap);

float tldOverlapRectRect(cv::Rect r1, cv::Rect r2);

cv::Rect *tldCopyRect(cv::Rect *r);

void tldOverlapRect(int *windows, int numWindows, cv::Rect *boundary, float *overlap);



bool tldSortByOverlapDesc(std::pair<int, float> bb1 , std::pair<int, float> bb2);
//Checks whether bb1 is completely inside bb2
int tldIsInside(int *bb1, int *bb2);

#endif /* UTIL_H_ */
