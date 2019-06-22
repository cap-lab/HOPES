
#ifndef _CAP_FORMAT_H_
#define _CAP_FORMAT_H_

#include <time.h>
#include <stdio.h>
#include <opencv/cv.h>

#define TLD_PATCH_SIZE 15
#define TLD_WINDOW_SIZE 5
#define TLD_WINDOW_OFFSET_SIZE 6

#define CAP_HEIGHT 180
#define CAP_WIDTH 320
#define CAP_CHANNEL 3

#define BILLION 1000000000L

#define sub2idx(x,y,widthstep) ((int) (floor((x)+0.5) + floor((y)+0.5)*(widthstep)))


template <class T>
class IntegralImage
{
public:
    T *data;  /* Array containg the entries for the integral image in row-first manner. Of size width*height. Allocated by tldAllocIntImg */
    /* width, height: Dimensions of integral image.*/
    int width;
    int height;

    IntegralImage(const cv::Mat &img)
    {
		data = new T[img.cols * img.rows];
    }

    virtual ~IntegralImage()
    {
        delete[] data;
    }

    void calcIntImg(const cv::Mat &img, bool squared = false)
    {
        const unsigned char *input = (const unsigned char *)(img.data);
        //T *output = data;
        for(int i = 0; i < img.cols; i++)
        {
            for(int j = 0; j < img.rows; j++)
            {
                T A = (i > 0) ? data[img.cols * j + i - 1] : 0;
                T B = (j > 0) ? data[img.cols * (j - 1) + i] : 0;
                T C = (j > 0 && i > 0) ? data[img.cols * (j - 1) + i - 1] : 0;
                T value = input[img.step * j + i];

                if(squared)
                {
                    value = value * value;
                }

                data[img.cols * j + i] = A + B - C + value;
            }
        }

    }
};

class DetectionResult
{
public:
    bool containsValidData;
    std::vector<cv::Rect>* fgList;
    float *posteriors;  /* Contains the posteriors for each slding window. Is of size numWindows. Allocated by tldInitClassifier. */
    std::vector<int>* confidentIndices;
    int *featureVectors;
    float *variances;
    int numClusters;
    cv::Rect *detectorBB; //Contains a valid result only if numClusters = 1

    DetectionResult()
    {
        containsValidData = false;
        fgList = new std::vector<cv::Rect>();
        confidentIndices = new std::vector<int>();
        numClusters = 0;
        detectorBB = NULL;

        variances = NULL;
        posteriors = NULL;
        featureVectors = NULL;
    }

    void init(int numWindows, int numTrees)
    {
        variances = new float[numWindows];
        posteriors = new float[numWindows];
        featureVectors = new int[numWindows * numTrees];
        confidentIndices = new std::vector<int>();
    }

    void reset()
    {
        containsValidData = false;

        if(fgList != NULL) fgList->clear();
    
        if(confidentIndices != NULL) confidentIndices->clear();

        numClusters = 0;
        delete detectorBB;
        detectorBB = NULL;
    }
    void release()
    {
        fgList->clear();
        delete[] variances;
        variances = NULL;
        delete[] posteriors;
        posteriors = NULL;
        delete[] featureVectors;
        featureVectors = NULL;
        delete confidentIndices;
        confidentIndices = NULL;
        delete detectorBB;
        detectorBB = NULL;
        containsValidData = false;
    }
    
    ~DetectionResult(){ release();}
    
};

extern DetectionResult *detectionResult;


typedef struct _normalizedPatch
{
	float values[TLD_PATCH_SIZE *TLD_PATCH_SIZE];
    bool positive;
} NormalizedPatch;

typedef struct _cvMatColor
{
    unsigned char data[CAP_HEIGHT*CAP_WIDTH*CAP_CHANNEL];
} CapMatColor;

typedef struct _cvMat
{
	unsigned char data[CAP_HEIGHT*CAP_WIDTH];
} CapMat;

typedef struct _cvRect
{
	int valid;
	cv::Rect value;
} CapRect;

typedef struct _detectorResult
{
	int numClusters;
	CapRect BB;
} DetectorResult;

struct _Result
{
	CapMat mat;
	CapRect BB;
};


typedef struct _Result TrackerResult;
typedef struct _Result FuseResult;

extern int numWindows;
extern int *windows;
extern int *windowOffsets;

extern cv::Size *scales;
extern int numScales;

extern int imgWidthStep;
extern int imgWidth;
extern int imgHeight;

extern int numFeatures;
extern int numTrees;
extern int minScale;
extern int maxScale;
extern int minSize;

#endif
