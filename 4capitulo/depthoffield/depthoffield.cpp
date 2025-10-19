#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
    cv::VideoCapture cap(argv[1]);
    cv::Mat frame, img_laplac;
    cv::Mat maxLaplacian, output;
    
    float laplacian[] = {0, -1, 0, 
                        -1, 4, -1, 
                         0, -1, 0};
                         
    cv::Mat maskLaplac(3, 3, CV_32F, laplacian);

    bool firstFrame = true;

    while (true) {
        cap >> frame;
        if (frame.empty()) break;

        cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
        cv::resize(frame, frame, cv::Size(frame.cols/2, frame.rows/2));

        cv::filter2D(frame, 
                     img_laplac, // output
                     CV_32F,          // float para evitar overflow
                     maskLaplac);  // aplicar o filtro laplaciano definido antes
        //valor absoluto do laplaciano pq pode dar negativo
        //então pegamos as bordas como positivas para ter mais nitidez
        img_laplac = cv::abs(img_laplac);  

        if (firstFrame) {
            maxLaplacian = img_laplac.clone();
            output = frame.clone();
            firstFrame = false;
        }

// ------------------ ALGORITMO DE DEPTH OF FIELD ------------------
        //máscara onde o frame atual é mais nítido
        cv::Mat mask_nitidez = img_laplac > maxLaplacian;
        
        // Copia para maxLaplacian apenas as regiões mais nítidas
        //Copia apenas os pixels de lapacianResult para maxLaplacian quando mask é diferente de zero
        img_laplac.copyTo(maxLaplacian, mask_nitidez);
        /*img_laplac -> se img atual mais nitida substitui o valor em maxlaplacian
        mask_nitidez determina quais posições os pixels estão mais nitidos*/
    
        frame.copyTo(output, mask_nitidez);// pegamos o valor de frame onde a máscara é verdadeira


        cv::imshow("Original", frame);
        cv::imshow("DOF", output);

        
    }
    std::cout << "Fim do vídeo" << std::endl;
    cv::imwrite("final_dof.png", output);
    cap.release();
    cv::waitKey();
    return 0;
}