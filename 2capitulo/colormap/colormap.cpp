#include <opencv2/opencv.hpp>

int main(int argc, const char *argv[]){
  cv::Mat image, imagecolor;
  
  image = imread(argv[1], cv::IMREAD_GRAYSCALE);
  if(!image.data){
    std::cout << "nao abriu a imagem" << std::endl;
  }

  cv::applyColorMap(image, imagecolor, cv::COLORMAP_JET);
  cv::imshow("colormap_jet", imagecolor);

  cv::applyColorMap(image, imagecolor, cv::COLORMAP_HOT);
  cv::imshow("colormap_hot", imagecolor);

  cv::applyColorMap(image, imagecolor, cv::COLORMAP_COOL);
  cv::imshow("colormap_cool", imagecolor);

  cv::applyColorMap(image, imagecolor, cv::COLORMAP_PARULA);
  cv::imshow("colormap_parula", imagecolor);

  cv::applyColorMap(image, imagecolor, cv::COLORMAP_OCEAN);
  cv::imshow("colormap_ocean", imagecolor);

  cv::applyColorMap(image, imagecolor, cv::COLORMAP_RAINBOW);
  cv::imshow("colormap_rainbow", imagecolor);
  
   // Cria as LUTs separadas para cada canal
    cv::Mat M1(1, 256, CV_8UC1); // Canal B
    cv::Mat M2(1, 256, CV_8UC1); // Canal G
    cv::Mat M3(1, 256, CV_8UC1); // Canal R

    for (int i = 0; i < 256; i++) {
        M1.at<uchar>(0, i) = i;         // Azul crescente
        M2.at<uchar>(0, i) = 255 - i;   // Verde decrescente
        M3.at<uchar>(0, i) = 0;         // Vermelho zero
    }

    // Aplicar LUT separadamente
    cv::Mat r1, r2, r3;
    cv::LUT(image, M1, r1);  // Canal B
    cv::LUT(image, M2, r2);  // Canal G
    cv::LUT(image, M3, r3);  // Canal R
    // mapeia a imagem original para cada canal
    // Juntar os 3 canais na ordem BGR
    std::vector<cv::Mat> channels;
    channels.push_back(r1); // B
    channels.push_back(r2); // G
    channels.push_back(r3); // R
// junta os canais em uma imagem colorida
    cv::merge(channels, imagecolor); // agora imagecolor é colorida

    // Mostrar imagem final
    cv::imshow("LUT personalizada", imagecolor);



  cv::waitKey(0);
  return 0;
}

// ./colormap ../teozin.jpg
