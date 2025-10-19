#include <iostream>
#include <opencv2/opencv.hpp>

int main(int, char**) {
    
    cv::Mat imagem;
    cv::Vec3b val;
    // para podermos acessar os pixels da imagem como .colunas e .rows
    imagem = cv::imread("teozin.jpg", cv::IMREAD_GRAYSCALE);
    if (!imagem.data) std::cout << "nao abriu a iamgem" << std::endl;

    // criar uma janela,formato autosize
    cv::namedWindow("janela", cv::WINDOW_AUTOSIZE);
    // agora vamos pegar a imagem e dividir em 4 partes iguais(ABCD) para depois trocar
    /*
      A B      D C
           ->   
      C D      B A
      */
    // começar pegando o tamanho da imagem e por simplificação considerar que o tamanho dela é par
    int linhas = imagem.rows;
    int colunas = imagem.cols;
    int meioLin = linhas / 2;
    int meioCol = colunas / 2;// no caso teozin.jpg é 280x280
    // agora vamos fazer a troca das regiões
    // pimeiro um clone
    cv::Mat clone = imagem.clone();
    // agora vamos fazer a troca
    for (int i = 0; i < meioLin; i++) {
        for (int j = 0; j < meioCol; j++) {
            // D <- A
            imagem.at<uchar>(i+ meioCol, j + meioCol) = clone.at<uchar>(i, j);
            // A <- D
            imagem.at<uchar>(i, j)                    = clone.at<uchar>(i+ meioCol, j + meioCol);
            // C <- B
            imagem.at<uchar>(i + meioLin, j)          = clone.at<uchar>(i, j + meioCol);
            // B <- C
            imagem.at<uchar>(i, j + meioCol)          = clone.at<uchar>(i + meioLin, j);
        }
    }
    // exibir a imagem na janela
    cv::imshow("janela", imagem);
    //esperar uma tecla ser pressionada para finalizar o programa
    cv::waitKey();
}