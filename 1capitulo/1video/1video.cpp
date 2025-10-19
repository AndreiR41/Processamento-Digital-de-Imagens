/*$ mkdir build
$ cd build
$ cmake ..
$ make
$ ./1video ../Mico.mp4
*/

#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv){
  cv::VideoCapture cap;
  double width, height;
  cv::Mat frame;
  int counter;
 // verifica se o arquivo passado como argumento existe
  cap.open(argv[1]);
  if(!cap.isOpened())
  return -1;
//vou precisar redimensionar que o original é gigante
  width=640;
  height=360;
  cv::Size frameSize(static_cast<int>(width), static_cast<int>(height)); 
  
  int type = cap.get(cv::CAP_PROP_FOURCC);
  //cria o objeto para escrever o video
  cv::VideoWriter out ("output.avi", type, cap.get(cv::CAP_PROP_FPS), frameSize, true);

//le o video ate o final
  for(counter=0; cap.read(frame); counter++){
    //operações
    // Redimensiona o frame
    cv::resize(frame, frame, cv::Size(640, 360));
    // Converte para escala de cinza
    cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
    // agora aplicando o negativo
    cv::bitwise_not(frame, frame);  // frame = 255 - frame

    out << frame;
    cv::imshow("Frame", frame);
    if(cv::waitKey(30) >= 0) break;
  }
  std::cout << "Numero de frames: " << counter << "\n";
  return 0;
}

