#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
    
    cv::Mat img = cv::imread(argv[1]);
    
    if (img.empty()) {
        std::cout << "Erro ao carregar a imgm: " << argv[1] << std::endl;
        return -1;
    }

    // Redimensiona para facilitar a visualização
    
    double fx = 0.5; // Scale factor for width (half the original width)
    double fy = 0.5; // Scale factor for height (half the original height)
    cv::resize(img, img, cv::Size(), fx, fy, cv::INTER_CUBIC);

    // tamanhos de kernel
    cv::Mat blur3x3, blur11x11, blur21x21;
    
    // Usando blur do OpenCV -> filtro de média
    cv::blur(img, blur3x3, cv::Size(3, 3));
    cv::blur(img, blur11x11, cv::Size(11, 11));
    cv::blur(img, blur21x21, cv::Size(21, 21));

    // Mostra os resultados
    cv::imshow("Original", img);
    cv::imshow("Media 3x3", blur3x3);
    cv::imshow("Media 11x11", blur11x11);
    cv::imshow("Media 21x21", blur21x21);

    cv::waitKey();//

    return 0;
}