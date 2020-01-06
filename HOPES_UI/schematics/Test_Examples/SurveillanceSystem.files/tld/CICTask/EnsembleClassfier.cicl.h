#include <opencv/cv.h>
#include "TLDUtil.h"
#include "format.h"

// Two functions following are the initialization/finalization functions, which are essential
extern LIBFUNC(void, init, void);
extern LIBFUNC(void, wrapup, void);
extern LIBFUNC(void, nextIteration, const cv::Mat &capImg);
extern LIBFUNC(int, filter, int i);
extern LIBFUNC(void, learn, int* boundary, int positive, int *featureVector);

