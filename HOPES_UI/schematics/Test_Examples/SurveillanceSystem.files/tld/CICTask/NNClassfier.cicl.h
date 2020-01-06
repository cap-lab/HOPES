#include <opencv/cv.h>
#include "format.h"
#include "TLDUtil.h"

// Two functions following are the initialization/finalization functions, which are essential
extern LIBFUNC(void, init, void);
extern LIBFUNC(void, wrapup, void);

extern LIBFUNC(int, filter, const cv::Mat &img, int windowIdx);
extern LIBFUNC(void, learn, std::vector<NormalizedPatch> patches);
extern LIBFUNC(float, classifyBB, cv::Mat &img, cv::Rect *bb);
extern LIBFUNC(float, getThetaTP, void);
extern LIBFUNC(float, getThetaFP, void);