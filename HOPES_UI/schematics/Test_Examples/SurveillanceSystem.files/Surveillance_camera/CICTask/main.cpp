#include <stdio.h>
#include <iostream>
#include <string.h>
#include <chrono>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

#include "edgetpu.h"
#include "utils.h"
#include "tensorflow/lite/interpreter.h"
#include "tensorflow/lite/model.h"
#include "tensorflow/lite/error_reporter.h"
#include "tensorflow/lite/graph_info.h"
#include "tensorflow/lite/kernels/register.h"
#include "tensorflow/lite/kernels/internal/tensor.h"
#include "tensorflow/lite/kernels/internal/tensor_utils.h"
#include "tensor.h"

/*** Model parameters ***/
#define MODEL_FILENAME "examples/mobilenet_ssd_v2_coco_quant_postprocess_edgetpu.tflite"
#define MODEL_WIDTH  300
#define MODEL_HEIGHT 300
#define MODEL_CHANNEL 3

std::unique_ptr<tflite::Interpreter> interpreter;

template<typename T>
T* TensorData(TfLiteTensor* tensor, int batch_index);
template<>
float* TensorData(TfLiteTensor* tensor, int batch_index) {
        int nelems = 1;
        for (int i = 1; i < tensor->dims->size; i++) nelems *= tensor->dims->data[i];
        switch (tensor->type) {
                case kTfLiteFloat32:
                        return tensor->data.f + nelems * batch_index;
                default:
                        //LOG(FATAL) << "Should not reach here!";
                        std::cout<< "sould not reach here!"<<std::endl;
        }
        return nullptr;
}

template<>
uint8_t* TensorData(TfLiteTensor* tensor, int batch_index) {
        int nelems = 1;
        for (int i = 1; i < tensor->dims->size; i++) nelems *= tensor->dims->data[i];
        switch (tensor->type) {
                case kTfLiteUInt8:
                        return tensor->data.uint8 + nelems * batch_index;
                default:
                        //LOG(FATAL) << "Should not reach here!";
                        std::cout<< "sould not reach here!"<<std::endl;
        }
        return nullptr;
}

void FeedInMat(const cv::Mat& mat, int batch_index) {
        TfLiteTensor* input_tensor_ = interpreter->tensor(interpreter->inputs()[0]);
        uint8_t* dst = TensorData<uint8_t>(input_tensor_, batch_index);
        const int row_elems = MODEL_WIDTH * MODEL_CHANNEL ;
        for (int row = 0; row < MODEL_HEIGHT; row++) {
                memcpy(dst, mat.ptr(row), row_elems);
                dst += row_elems;
        }
        
}

using namespace std;
using namespace cv; 

int main(int argc, char** argv)
{
        /*** Create interpreter ***/
        /* read model */
        std::unique_ptr<tflite::FlatBufferModel> model = tflite::FlatBufferModel::BuildFromFile(MODEL_FILENAME);

        /* initialize edgetpu_context */
        edgetpu::EdgeTpuContext* edgetpu_context = edgetpu::EdgeTpuManager::GetSingleton()->NewEdgeTpuContext().release();
        /* create interpreter */
        interpreter = coral::BuildEdgeTpuInterpreter(*model, edgetpu_context);

        //int CAM_ID = 0; //notebook cam
        int CAM_ID = 1; //usb connected cam
        Mat inputImage;
        VideoCapture cap(CAM_ID); //Generate Camera
        if(!cap.isOpened()){ //Check if Camera Opened
                printf("Can't open the CAM(%d)\n",CAM_ID);
                return -1; 
        } 

        double width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
        double height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
        //printf("size = [%f, %f]\n",width, height);

        namedWindow("CAM_Window", 0); 
        resizeWindow("CAM_Window", 300, 300);

        while(1){
                cap >> inputImage;
                //printf("[profiling] sizeof captured image : %lld\n",sizeof(inputImage)); // = 96

                /*** Read input image data ***/
                cv::cvtColor(inputImage, inputImage, cv::COLOR_RGB2BGR);
                cv::resize(inputImage, inputImage, cv::Size(300, 300));
                FeedInMat(inputImage, 0);

                /*** Run inference ***/
                interpreter->Invoke();
                coral::AnnotateMat(inputImage, interpreter.get());
                cv::cvtColor(inputImage, inputImage, CV_RGB2BGR);
                std::cout<<"imshow!"<<std::endl;
                cv::imshow( "image", inputImage);
                printf("[profiling] sizeof result image : %lld\n",sizeof(inputImage)); // = 96
                cv::waitKey(20);
        }
}
