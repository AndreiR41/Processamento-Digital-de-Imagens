#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv){
  cv::VideoCapture cap;
  double width, height;
  cv::Mat frame,lightPaint1,lightPaint2,img_max,img_xor,img_min,comb;
  int counter;


  cap.open(argv[1]);
  if(!cap.isOpened())
  return -1;
  width=640;
  height=360;
  cv::Size frameSize(static_cast<int>(width), static_cast<int>(height)); 
  
  int type = cap.get(cv::CAP_PROP_FOURCC);
  //cria o objeto para escrever o video
  cv::VideoWriter out ("output.avi", type, cap.get(cv::CAP_PROP_FPS), frameSize, true);
  lightPaint1= cv::Mat::zeros(frameSize, CV_8UC3);
  lightPaint2= cv::Mat::zeros(frameSize, CV_8UC3);

//le o video ate o final
  for(counter=0; cap.read(frame); counter++){
    cv::resize(frame, frame, cv::Size(640, 360));
    cv::resize(lightPaint1, lightPaint1, cv::Size(640, 360));
    cv::resize(lightPaint2, lightPaint2, cv::Size(640, 360));

    img_max= cv::max(lightPaint1,frame);
    img_min= cv::min(lightPaint1,frame);

    lightPaint1= img_max+frame;
    
    //mais bunitu
    cv::bitwise_xor(img_min,frame, img_xor);
    lightPaint2= cv::max(img_xor,lightPaint2); ;
    

    cv::hconcat(lightPaint1 ,lightPaint2 ,comb);
    cv::imshow("Combinacao", comb);

    out <<comb ;
    if(cv::waitKey(30) >= 0) break;
  }
  std::cout << "Numero de frames: " << counter << "\n";
  return 0;
}

