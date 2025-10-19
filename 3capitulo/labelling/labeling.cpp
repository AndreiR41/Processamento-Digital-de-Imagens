#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
  cv::Mat image, temp_image, image_32bit;
  int width, height;
  int nobjects, buracos;

  cv::Point p;
  image = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);

  if (!image.data) {
    std::cout << "imagem nao carregou corretamente\n";
    return (-1);
  }

  width = image.cols;
  height = image.rows;
  std::cout << width << "x" << height << std::endl;

  p.x = 0;
  p.y = 0;

  //------------------------------------------AUMENTAR CONTAGEM DE OBJETOS ------------------------------------------------
  image_32bit = Mat(height, width, CV_32SC1, Scalar(0)); // Inteiros de 32 bits para rótulos

  //------------------------------------------REMOVER BOLHAS NA BORDA ------------------------------------------------
  temp_image = image.clone(); 
  // Preenche bordas com floodFill se encontrar bolhas
  for (int i = 0; i < image.rows; i++) {
    // Bordas esquerda e direita
    if (temp_image.at<uchar>(i, 0) == 255) 
      floodFill(temp_image, Point(0, i), 0);
    if (temp_image.at<uchar>(i, image.cols-1) == 255) 
      floodFill(temp_image, Point(image.cols-1, i), 0);
  }
  for (int j = 0; j < image.cols; j++) {
    // Bordas superior e inferior
    if (temp_image.at<uchar>(0, j) == 255) 
      floodFill(temp_image, Point(j, 0), 0);
    if (temp_image.at<uchar>(image.rows-1, j) == 255) 
      floodFill(temp_image, Point(j, image.rows-1), 0);
  }

  //------------------------------------------CONTAR BOLHAS ------------------------------------------------
  nobjects = 0;

  for (int i = 0; i < temp_image.rows; i++) {
    for (int j = 0; j < temp_image.cols; j++) {
      // achou um objeto
      if (temp_image.at<uchar>(i, j) == 255) {
        nobjects++;
        
        p.x = j;
        p.y = i;
        
        // FloodFill normal e depois image_32bit
        floodFill(temp_image, p, 0);  // Remove a bolha marcando como 0
        
        Mat mask = Mat::zeros(height + 2, width + 2, CV_8UC1);
        floodFill(image, mask, p, 255, 0, 0, 4);  // Usa a imagem original como referência
        
        // Marca na image_32bit onde a máscara tem 255
        for (int y = 0; y < height; y++) {
          for (int x = 0; x < width; x++) {
            if (mask.at<uchar>(y + 1, x + 1) == 255) {
              image_32bit.at<int>(y, x) = nobjects;
            }
          }
        }
      }
    }
  }

  //------------------------------------------CONTAR BURACOS ------------------------------------------------
  buracos = 0;
  Mat image_buracos = image.clone();  // Trabalha em cópia da original

  p.x = 0;
  p.y = 0;
  floodFill(image_buracos, p, 255);  // Preenche fundo externo 255

  // Varredura para encontrar buracos
  for(int i = 1; i < height-1; i++) {
    for(int j = 1; j < width-1; j++) {
      // Procura por pixels pretos 
      if(image_buracos.at<uchar>(i, j) == 0) {
        bool completamente_cercado = true;
        
        // Checa os vizinhos
        if(image_buracos.at<uchar>(i-1, j) == 255 ||  // ACIMA é fundo
           image_buracos.at<uchar>(i+1, j) == 255 ||  // ABAIXO é fundo  
           image_buracos.at<uchar>(i, j-1) == 255 ||  // ESQUERDA é fundo
           image_buracos.at<uchar>(i, j+1) == 255) {  // DIREITA é fundo
           completamente_cercado = false;
        }
        
        if(completamente_cercado) {
          buracos++;
          p.x = j;
          p.y = i;
          floodFill(image_buracos, p, 200);  // Marca este buraco como processado
        }
      }
    }
  }

  //------------------------------------------CONCLUINDO------------------------------------------------
  std::cout << "a figura tem " << nobjects << " bolhas\n";
  std::cout << "a figura tem " << buracos << " buracos internos\n";
 

  cv::imshow("Original", image);
  cv::imshow("Detecao de Buracos", image_buracos);
  
  cv::imwrite("buracos.png", image_buracos);
  cv::waitKey();
  return 0;
}