#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
    cv::Mat imagemEsteganografada, imagemPortadora, imagemEscondida;
    cv::Vec3b valFinal, valPortadora, valEscondida;
    // numero de bits usados na esteganografia
    int nbits = 3;
    
    imagemEsteganografada = cv::imread(argv[1], cv::IMREAD_COLOR);

    if (imagemEsteganografada.empty()) {
        std::cout << "Erro ao carregar a imagem esteganografada." << std::endl;
        return -1;
    }

    imagemPortadora = imagemEsteganografada.clone();
    imagemEscondida = imagemEsteganografada.clone();

    
    
    for (int i = 0; i < imagemEsteganografada.rows; i++) {
        for (int j = 0; j < imagemEsteganografada.cols; j++) {
            valFinal = imagemEsteganografada.at<cv::Vec3b>(i, j);

            // Recupera os bits mais significativos (imagem portadora)
            valPortadora[0] = (valFinal[0] & 0b11111000); // só penso em binário;
            valPortadora[1] = (valFinal[1] & 0b11111000);
            valPortadora[2] = (valFinal[2] & 0b11111000);

            // Recupera os bits menos significativos (imagem escondida)
            valEscondida[0] = (valFinal[0] & 0b00000111);
            valEscondida[1] = (valFinal[1] & 0b00000111);
            valEscondida[2] = (valFinal[2] & 0b00000111);

            imagemPortadora.at<cv::Vec3b>(i, j) = valPortadora;
            imagemEscondida.at<cv::Vec3b>(i, j) = valEscondida;
        }
    }

    cv::imwrite("portadora_.png", imagemPortadora);
    cv::imwrite("escondida_.png", imagemEscondida);

    std::cout << "Recuperação concluída!" << std::endl;

    return 0;
}
