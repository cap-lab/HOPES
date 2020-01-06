#include "utils.h"

#include <memory>

#include "tensorflow/lite/builtin_op_data.h"
#include "tensorflow/lite/kernels/register.h"

#include <opencv2/opencv.hpp>
#include <iostream>

// __cxa_thread_atexit is not part of C++ ABI. Define it here to allow program
// to link with libc++ successfully.
extern "C" int __cxa_thread_atexit(void (*func)(), void* obj,
                                   void* dso_symbol) {
  int __cxa_thread_atexit_impl(void (*)(), void*, void*);
  return __cxa_thread_atexit_impl(func, obj, dso_symbol);
}
int PRINT_IGNORED_DETECTION = 0;

namespace coral {
namespace {
std::vector<uint8_t> DecodeBmp(const uint8_t* input, int row_size, int width,
                               int height, int channels, bool top_down) {
  std::vector<uint8_t> output(height * width * channels);
  for (int i = 0; i < height; i++) {
    int src_pos;
    int dst_pos;
    for (int j = 0; j < width; j++) {
      if (!top_down) {
        src_pos = ((height - 1 - i) * row_size) + j * channels;
      } else {
        src_pos = i * row_size + j * channels;
      }
      dst_pos = (i * width + j) * channels;
      switch (channels) {
        case 1:
          output[dst_pos] = input[src_pos];
          break;
        case 3:
          // BGR -> RGB
          output[dst_pos] = input[src_pos + 2];
          output[dst_pos + 1] = input[src_pos + 1];
          output[dst_pos + 2] = input[src_pos];
          break;
        case 4:
          // BGRA -> RGBA
          output[dst_pos] = input[src_pos + 2];
          output[dst_pos + 1] = input[src_pos + 1];
          output[dst_pos + 2] = input[src_pos];
          output[dst_pos + 3] = input[src_pos + 3];
          break;
        default:
          std::cerr << "Unexpected number of channels: " << channels
                    << std::endl;
          break;
      }
    }
  }
  return output;
}

std::vector<uint8_t> ResizeImage(const std::vector<uint8_t>& in,
                                 const ImageDims& in_dims,
                                 const ImageDims& out_dims) {
  const int image_height = in_dims[0];
  const int image_width = in_dims[1];
  const int image_channels = in_dims[2];
  const int wanted_height = out_dims[0];
  const int wanted_width = out_dims[1];
  const int wanted_channels = out_dims[2];
  int number_of_pixels = image_height * image_width * image_channels;
  assert(number_of_pixels == in.size());
  std::unique_ptr<tflite::Interpreter> interpreter(new tflite::Interpreter);
  int base_index = 0;
  // two inputs: input and new_sizes
  interpreter->AddTensors(2, &base_index);
  // one output
  interpreter->AddTensors(1, &base_index);
  // set input and output tensors
  interpreter->SetInputs({0, 1});
  interpreter->SetOutputs({2});
  // set parameters of tensors
  TfLiteQuantizationParams quant;
  interpreter->SetTensorParametersReadWrite(
      0, kTfLiteFloat32, "input",
      {1, image_height, image_width, image_channels}, quant);
  interpreter->SetTensorParametersReadWrite(1, kTfLiteInt32, "new_size", {2},
                                            quant);
  interpreter->SetTensorParametersReadWrite(
      2, kTfLiteFloat32, "output",
      {1, wanted_height, wanted_width, wanted_channels}, quant);
  tflite::ops::builtin::BuiltinOpResolver resolver;
  const TfLiteRegistration* resize_op =
      resolver.FindOp(tflite::BuiltinOperator_RESIZE_BILINEAR, 1);
  auto* params = reinterpret_cast<TfLiteResizeBilinearParams*>(
      malloc(sizeof(TfLiteResizeBilinearParams)));
  params->align_corners = false;
  interpreter->AddNodeWithParameters({0, 1}, {2}, nullptr, 0, params, resize_op,
                                     nullptr);
  interpreter->AllocateTensors();
  // fill input image
  // in[] are integers, cannot do memcpy() directly
  auto input = interpreter->typed_tensor<float>(0);
  for (int i = 0; i < number_of_pixels; i++) {
    input[i] = in[i];
  }
  // fill new_sizes
  interpreter->typed_tensor<int>(1)[0] = wanted_height;
  interpreter->typed_tensor<int>(1)[1] = wanted_width;
  interpreter->Invoke();
  auto output = interpreter->typed_tensor<float>(2);
  auto output_number_of_pixels =
      wanted_height * wanted_height * wanted_channels;
  std::vector<uint8_t> result(output_number_of_pixels);
  for (int i = 0; i < output_number_of_pixels; i++) {
    result[i] = static_cast<uint8_t>(output[i]);
  }
  return result;
}

bool EndsWith(std::string const& value, std::string const& ending) {
  if (ending.size() > value.size()) return false;
  return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

int ImageDimsToSize(const ImageDims& dims) {
  int size = 1;
  for (const auto& dim : dims) {
    size *= dim;
  }
  return size;
}

std::vector<uint8_t> ReadBmp(const std::string& input_bmp_name,
                             ImageDims* image_dims) {
  int* height = image_dims->data();
  int* width = image_dims->data() + 1;
  int* channels = image_dims->data() + 2;
  int begin, end;
  std::ifstream file(input_bmp_name, std::ios::in | std::ios::binary);
  if (!file.good()) {
    std::cerr << "Failed to open file: " << input_bmp_name << std::endl;
    std::abort();
  }
  begin = file.tellg();
  file.seekg(0, std::ios::end);
  end = file.tellg();
  size_t len = end - begin;
  std::vector<uint8_t> img_bytes(len);
  file.seekg(0, std::ios::beg);
  file.read(reinterpret_cast<char*>(img_bytes.data()), len);
  const int32_t header_size =
      *(reinterpret_cast<const int32_t*>(img_bytes.data() + 10));
  *width = *(reinterpret_cast<const int32_t*>(img_bytes.data() + 18));
  *height = *(reinterpret_cast<const int32_t*>(img_bytes.data() + 22));
  const int32_t bpp =
      *(reinterpret_cast<const int32_t*>(img_bytes.data() + 28));
  *channels = bpp / 8;
  // there may be padding bytes when the width is not a multiple of 4 bytes
  // 8 * channels == bits per pixel
  const int row_size = (8 * *channels * *width + 31) / 32 * 4;
  // if height is negative, data layout is top down
  // otherwise, it's bottom up
  bool top_down = (*height < 0);
  // Decode image, allocating tensor once the image size is known
  const uint8_t* bmp_pixels = &img_bytes[header_size];
  return DecodeBmp(bmp_pixels, row_size, *width, abs(*height), *channels,
                   top_down);
}
}  // namespace

std::vector<uint8_t> GetInputFromBmpImage(const std::string& image_path,
                                          const ImageDims& target_dims) {
  std::vector<uint8_t> result;
  if (!EndsWith(image_path, ".bmp")) {
    std::cerr << "Unsupported image type: " << image_path << std::endl;
    return result;
  }
  result.resize(ImageDimsToSize(target_dims));
  ImageDims image_dims;
  std::vector<uint8_t> in = ReadBmp(image_path, &image_dims);
  return ResizeImage(in, image_dims, target_dims);
}

std::unique_ptr<tflite::Interpreter> BuildEdgeTpuInterpreter(
    const tflite::FlatBufferModel& model,
    edgetpu::EdgeTpuContext* edgetpu_context) {
  tflite::ops::builtin::BuiltinOpResolver resolver;
  resolver.AddCustom(edgetpu::kCustomOp, edgetpu::RegisterCustomOp());
  std::unique_ptr<tflite::Interpreter> interpreter;
  if (tflite::InterpreterBuilder(model, resolver)(&interpreter) != kTfLiteOk) {
    std::cerr << "Failed to build interpreter." << std::endl;
  }
  // Bind given context with interpreter.
  interpreter->SetExternalContext(kTfLiteEdgeTpuContext, edgetpu_context);
  interpreter->SetNumThreads(1);

  if (interpreter->AllocateTensors() != kTfLiteOk) {
    std::cerr << "Failed to allocate tensors." << std::endl;
  }
  return interpreter;
}

template<typename T>
T* TensorData(TfLiteTensor* tensor, int batch_index);

template<>
float* TensorData(TfLiteTensor* tensor, int batch_index) {
    int nelems = 1;
    for (int i = 1; i < tensor->dims->size; i++) nelems *= tensor->dims->data[i];
	//for(int i = 0; i < tensor->dims->size; i++)
	//	std::cout<<"tensor->data.f "<<tensor->data.f[i]<<std::endl;
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
			std::cout<< "sould not reach here22!"<<std::endl;
    }
    return nullptr;
}



bool ReadLines(const std::string& file_name, std::vector<std::string>* lines) {
    std::ifstream file(file_name);
    if (!file) {
        std::cout<< "Failed to open file " << file_name<<std::endl;;
        return false;
    }
    std::string line;
    while (std::getline(file, line)) lines->push_back(line);
    return true;
}

std::vector<float> RunInference(cv::Mat& input_data,
                                tflite::Interpreter* interpreter) {
  std::vector<float> output_data;
  //uint8_t* input = interpreter->typed_input_tensor<uint8_t>(0);
  //std::memcpy(input, input_data.data(), input_data.size());
  //std::vector<int> inputs = interpreter->inputs();
  //for(unsigned int i=0;i<interpreter->inputs().size();i++)
  //{
  //    int input = inputs[i];
  //    std::cout<<"interpreter type : "<< i<<": "<<interpreter->tensor(input)->type << std::endl;
  //}

  interpreter->Invoke();

  AnnotateMat(input_data, interpreter);
  //Dowhan fixed to implement object detction.
}




void AnnotateMat(cv::Mat& mat, tflite::Interpreter* interpreter) {
  //std::cout<<"interpreter output size : " <<interpreter->outputs().size()<<std::endl;
  for(int i = 0; i < interpreter->outputs().size();i++)
  {
	  //std::cout<<"output type "<<i<<" : "<<interpreter->tensor(interpreter->outputs()[i])->type<<std::endl;
  }

  TfLiteTensor*  output_locations_ = interpreter->tensor(interpreter->outputs()[0]);
  TfLiteTensor*  output_classes_ = interpreter->tensor(interpreter->outputs()[1]);
  TfLiteTensor*  output_scores_ = interpreter->tensor(interpreter->outputs()[2]);
  TfLiteTensor*  num_detections_ = interpreter->tensor(interpreter->outputs()[3]);

int batch_index = 0;

  const float* detection_locations = TensorData<float>(output_locations_, batch_index);
  const float* detection_classes = TensorData<float>(output_classes_, batch_index);
  const float* detection_scores = TensorData<float>(output_scores_, batch_index);
  const int num_detections = *TensorData<float>(num_detections_, batch_index);
  //std::cout<<"num_detections : " << num_detections<<std::endl;
	std::vector<std::string> labels;

if (!ReadLines("labelmap.txt", &labels)){std::cout<<"cannot read file!"<<std::endl;}
	
  for (int d = 0; d < num_detections; d++) {
	  //std::cout<<"detection_classes : " << detection_classes[d]<<std::endl;
	  //std::cout<<"detection_class label : " << labels[detection_classes[d]]<<std::endl;
	  //std::cout<<"labelsize : " << labels.size()<<std::endl;
		std::string cls = "";
	  if(detection_classes[d] < labels.size())
	  {
		  cls = labels[detection_classes[d] + 1];
	  }


	  const float score = detection_scores[d];
	  //const float ymin = detection_locations[4 * d];
	  //const float xmin = detection_locations[4 * d + 1];
	  //const float ymax = detection_locations[4 * d + 2];
	  //const float xmax = detection_locations[4 * d + 3];
	  const float ymin = detection_locations[4 * d] * mat.rows;
	  const float xmin = detection_locations[4 * d + 1] * mat.cols;
	  const float ymax = detection_locations[4 * d + 2] * mat.rows;
	  const float xmax = detection_locations[4 * d + 3] * mat.cols;	
	  //if (score < .3f) {
	  if (score < 0.5) { //default threshold value.
		  if(PRINT_IGNORED_DETECTION)
		  {
			  std::cout << "Ignore detection " << d << " of '" << cls << "' with score " << score
				  << " @[" << xmin << "," << ymin << ":" << xmax << "," << ymax << "]"<<std::endl;

		  }
	  } else {
		  if(detection_classes[d] == 0)
		  {
			  std::cout << "Detected " << d << " of '" << cls << "' with score " << score
				  << " @[" << xmin << "," << ymin << ":" << xmax << "," << ymax << "]"<<std::endl;
			  cv::rectangle(mat, cv::Rect(xmin, ymin, xmax - xmin, ymax - ymin),
					  cv::Scalar(255, 0, 0), 1);
			  cv::putText(mat, cls, cv::Point(xmin, ymin - 5),
					  cv::FONT_HERSHEY_COMPLEX, .8, cv::Scalar(10, 255, 30));
		  }
	  }
  }

  
  }
}  // namespace coral
using namespace coral;
bool isPersonDetected(tflite::Interpreter* interpreter)
  {
	  TfLiteTensor*  output_locations_ = interpreter->tensor(interpreter->outputs()[0]);
	  TfLiteTensor*  output_classes_ = interpreter->tensor(interpreter->outputs()[1]);
	  TfLiteTensor*  output_scores_ = interpreter->tensor(interpreter->outputs()[2]);
	  TfLiteTensor*  num_detections_ = interpreter->tensor(interpreter->outputs()[3]);

	  int batch_index = 0;

	  const float* detection_locations = TensorData<float>(output_locations_, batch_index);
	  const float* detection_classes = TensorData<float>(output_classes_, batch_index);
	  const float* detection_scores = TensorData<float>(output_scores_, batch_index);
	  const int num_detections = *TensorData<float>(num_detections_, batch_index);

	  for (int d = 0; d < num_detections; d++) {
		  const float score = detection_scores[d];
		  if(detection_classes[d] == 0 && score >= 0.1) //person detected
		  {
			  return true;
		  }
	  }

	  return false;
  }
