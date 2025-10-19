#include <iostream>
#include <opencv2/opencv.hpp>
#include "camera.hpp"

int main(int argc, char** argv){
    cv::Mat image, gray, equalized, concatenated;
    cv::VideoCapture cap;
    int camera = cameraEnumerator();
    int key;

    cap.open(camera);
    
    if(!cap.isOpened()){
        std::cout << "cameras indisponiveis";
        return -1;
    }
    
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);

   //para salvar o video
    cv::VideoWriter video("comparacao_equalizacao.avi", cv::VideoWriter::fourcc('M','J','P','G'), 30, cv::Size(640 * 3, 480), true);

    while(1){
        cap >> image;
        
        
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
        
        // Aplicar equalização de histograma
        cv::equalizeHist(gray, equalized);
        
        // Converter gray e equalized para colorido para exibição consistente
        cv::Mat grayColor, equalizedColor;
        cv::cvtColor(gray, grayColor, cv::COLOR_GRAY2BGR);
        cv::cvtColor(equalized, equalizedColor, cv::COLOR_GRAY2BGR);
        
        // Juntando os videos lado a lado
        cv::hconcat(image, grayColor, concatenated);
        cv::hconcat(concatenated, equalizedColor, concatenated);
        video.write(concatenated);
        
        // Exibir as imagens individuais
        cv::imshow("Original", image);
        cv::imshow("Tons de Cinza", gray);
        cv::imshow("Equalizada", equalized);
        // Exibir também a versão concatenada
        cv::imshow("Comparacao", concatenated);
        

        key = cv::waitKey(30);
        if(key == 27) break;
    }

    // Liberar o vídeo writer
    video.release();
    return 0;
}
/*fiz os testes com o ps eye do ps3
comecei com iluminação natural fraca, depois aumentei a iluminação com lanterna do celular
e depois no escuro e com lanterna do celular.
*/