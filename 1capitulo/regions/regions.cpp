#include <iostream>
#include <opencv2/opencv.hpp>

int main(int, char**) {
    //declarar as coordernadas dos pontos para formar o retangulo
    int p1x,p1y,p2x,p2y; // os pontos acabam podendo ficar em posições erradas
    const int limiteMax = 280; //limite maximo para as coordenadas
    std::cout << "Digite as coordenadas do ponto 1 (x y): ";
    std::cin >> p1x >> p1y;
    std::cout << "Digite as coordenadas do ponto 2 (x y): ";
    std::cin >> p2x >> p2y;
    
    cv::Mat imagem;
    cv::Vec3b val;
        // para podermos acessar os pixels da imagem como .colunas e .rows
    imagem = cv::imread("teozin.jpg", cv::IMREAD_GRAYSCALE);
    if (!imagem.data) std::cout << "nao abriu a iamgem" << std::endl;

    // criar uma janela,formato autosize
    cv::namedWindow("janela", cv::WINDOW_AUTOSIZE);
    // para fazer o negativo da imagem fazemos 255 - Valor(pixel) - caso cinza
    // trabalhando com apenas 8 bits para representar os tons de cinza- uchar
    // imagem.at<uchar>(linha,coluna) para acessar o pixel
    cv::Point P1(p1x, p1y); // P(x,y)-> P(linha,coluna)
    cv::Point P2(p2x, p2y);// não precisava mas tava aprendendo a usar o ponto
        //fazer o negativo da regiao delimitada pelos pontos P1 e P2
    for (int i = P1.y; i < P2.y; i++) {
        for (int j = P1.x; j < P2.x; j++) {
        imagem.at<uchar>(i, j) = 255 - imagem.at<uchar>(i, j);
        }
    }
        
    // exibir a imagem na janela
    cv::imshow("janela", imagem);
    //esperar uma tecla ser pressionada para finalizar o programa
    cv::waitKey();



}