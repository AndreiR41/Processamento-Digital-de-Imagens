#include <iostream>
#include <opencv2/opencv.hpp>
#include "camera.hpp"

int main(){
    cv::Mat frame, gray, hist, hist_antigo;
    int camera;
    cv::VideoCapture cap;
    
    //seleciona a camera
    camera = cameraEnumerator();
    cap.open(camera);
    
    if(!cap.isOpened()){
        std::cout << "Cameras indisponiveis";
        return -1;
    }
    
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    int width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    
    // --------------------------------------CORRELAÇÃO DE HISTOGRAMAS--------------------------------------
    
    // limite para correlação 
    const double limite = 0.98; 
    
    cv::VideoWriter video("detecao_movimento.avi", 
                         cv::VideoWriter::fourcc('M','J','P','G'), // Codec
                         30,                          // FPS - mais baixo que o comum
                         cv::Size(width, height));    // Resolução fixa
    
    while(true){
        cap >> frame;
        
        
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        
        // CÁLCULO E NORMALIZAÇÃO DO HISTOGRAMA
        // Normalizar para [0,1] permite comparar frames de diferentes cenas
        int bins = 64;           // 256 tons ÷ 64 = 4 tons por bin
        // Intervalo de valores para cada bin 
        float range[] = {0, 256};
        const float* ranges = range;  
        cv::calcHist(&gray, 1, 0, cv::Mat(), hist, 1, &bins, &ranges);

        cv::normalize(hist, hist, 0, 1, cv::NORM_MINMAX); // normaliza para [0,1]
        
        
        if(!hist_antigo.empty()){
            double correlation = cv::compareHist(hist_antigo, hist, cv::HISTCMP_CORREL);// HISTCMP_CORREL-> correlação
            
            if(correlation < limite){
              // coloca o texto "MEXEU!" no frame quando detecta movimento
              cv::putText(frame,  
                          "MEXEU!", 
                           cv::Point(50, 50), // posição
                           cv::FONT_HERSHEY_SIMPLEX,// fonte
                            1, // escala
                           cv::Scalar(122, 30, 255), // cor (BGR)
                           2);
              std::cout << "Algo se mexeu-> Correlacao: " << correlation << std::endl;
            }
        }
        
        hist.copyTo(hist_antigo);
        
      
        video.write(frame);
        cv::imshow("Detector de Movimento", frame);
        
        if(cv::waitKey(30) == 27) break; // ESC para sair
    }
    
    video.release();
    cap.release();
    
    return 0;
}
