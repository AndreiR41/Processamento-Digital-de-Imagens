#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
    if (argc != 2) return -1;

    VideoCapture cap(argv[1]);
    if (!cap.isOpened()) return -1;

    // Configurações fixas
    float centro = 0.5f;
    float faixa_altura = 0.4f;  
    float decaimento = 5.0f;
    float saturacao = 1.7f;

    string saida = "video_tiltshift.mp4";
    
    int largura = cap.get(CAP_PROP_FRAME_WIDTH);
    int altura = cap.get(CAP_PROP_FRAME_HEIGHT);
    double fps = cap.get(CAP_PROP_FPS);
    
    // comparação lado a lado
    VideoWriter writer(saida, VideoWriter::fourcc('a','v','c','1'), fps, Size(largura * 2, altura));

    Mat frame, blurred, mask, frame_original;
    
    // Pré-calcular máscara
    mask.create(altura, largura, CV_32FC1);
    float l1 = centro * altura - (faixa_altura * altura)/2;
    float l2 = centro * altura + (faixa_altura * altura)/2;
    
    for(int i = 0; i < altura; i++) {
        float fx = 0.5 * (tanh((i - l1)/decaimento) - tanh((i - l2)/decaimento));
        for(int j = 0; j < largura; j++) {
            mask.at<float>(i, j) = fx;
        }
    }

    while (true) {
        cap >> frame;
        

        // frame original
        frame.copyTo(frame_original);

        GaussianBlur(frame, blurred, Size(15, 15), 0);

        Mat frame_f, blurred_f;
        frame.convertTo(frame_f, CV_32FC3);
        blurred.convertTo(blurred_f, CV_32FC3);

        vector<Mat> canais_frame, canais_blurred, canais_result(3);
        split(frame_f, canais_frame);
        split(blurred_f, canais_blurred);
        
        for (int i = 0; i < 3; i++) {
            multiply(canais_frame[i], mask, canais_frame[i]);
            multiply(canais_blurred[i], 1.0 - mask, canais_blurred[i]);
            add(canais_frame[i], canais_blurred[i], canais_result[i]);
        }

        Mat result_float;
        merge(canais_result, result_float);
        result_float.convertTo(frame, CV_8UC3);

        // Ajustar saturação
        Mat hsv;
        cvtColor(frame, hsv, COLOR_BGR2HSV);
        vector<Mat> canais_hsv;
        split(hsv, canais_hsv);
        canais_hsv[1] = canais_hsv[1] * saturacao;
        min(canais_hsv[1], 255, canais_hsv[1]);
        merge(canais_hsv, hsv);
        cvtColor(hsv, frame, COLOR_HSV2BGR);

        // Concatenar pro lado a lado
        Mat comparacao;
        hconcat(frame_original, frame, comparacao);

        writer.write(comparacao);
    }

    cap.release();
    writer.release();
    
    return 0;
}