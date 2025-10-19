//fazer um array com as string ou um dicionario de color maps e um loop para mostrar todas
//e depois fazer adaptado o LUT do de antes
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <filesystem> 

namespace fs = std::filesystem;

int main(int argc, char** argv){
  cv::Mat image,imagecolor;
  cv::Mat hsv, rgb, h, s, v;

  std::vector<cv::Mat> planes;
  int camera;
  double min, max;

  if(argc != 2){
    std::cerr << "Uso: " << argv[0] << " <Image_Path>\n";
    return -1;
  }

  image = cv::imread(argv[1], cv::IMREAD_COLOR);

  if (!image.data) {
    std::cerr << "Nao abriu " << argv[1] << std::endl;
    return -1;
  }

  std::vector<int> colormaps = {
    cv::COLORMAP_JET,
    cv::COLORMAP_HOT,
    cv::COLORMAP_COOL,
    cv::COLORMAP_AUTUMN,
    cv::COLORMAP_BONE, 
    
};

  cv::namedWindow("image", cv::WINDOW_NORMAL);
  cv::imshow("image", image);
  for (int i=0;i<colormaps.size();i++){
    // depois transformar em uma função
    cv::applyColorMap(image, imagecolor, colormaps[i]);
    cv::split (imagecolor, planes);
    cv::hconcat(planes[0], planes[1], rgb);
    cv::hconcat(rgb, planes[2], rgb);
    std::string titulo = "Colormap " + std::to_string(i); // título dinâmico
    cv::imshow(titulo, rgb);
    
    cv::cvtColor(imagecolor, imagecolor, cv::COLOR_BGR2HSV_FULL);
    cv::split (imagecolor, planes);
    cv::hconcat(planes[0], planes[1], hsv);
    cv::hconcat(hsv, planes[2], hsv);
    std::string tituloHSV = "HSV " + std::to_string(i);
    cv::imshow(tituloHSV, hsv);

    cv::applyColorMap(planes[0], h, cv::COLORMAP_HSV); // NÃO TEM O FULL
    std::string tituloh = "h " + std::to_string(i);
    cv::imshow(tituloh, h);
    
    //salvando as imagens
        std::string pasta = "../ImagensColorspaces/" + std::to_string(i) ;
        fs::create_directories(pasta);  // Cria subpastas recursivamente
        cv::imwrite(pasta + "/RGB.png", rgb);
        cv::imwrite(pasta + "/HSV.png", hsv);
        cv::imwrite(pasta + "/H_colormap.png", h);


  }
    

  cv::waitKey();
}

