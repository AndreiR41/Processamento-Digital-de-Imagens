#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

// Variáveis globais
Mat image, blurred, result;
int altura, largura;

// Parâmetros iniciais
int slider_altura = 50;
int slider_decaimento = 50;
int slider_centro = 50;
int slider_saturacao = 50;  // Valor de saturação inicial (0-100)


//---------------------------------------------FUNÇÃO TILTSHIFT---------------------------------------------
void tiltshift(int, void*) {
    // Obter valores atuais das trackbars
    slider_altura = getTrackbarPos("Altura Foco", "Tiltshift");
    slider_decaimento = getTrackbarPos("Suavização", "Tiltshift");
    slider_centro = getTrackbarPos("Centro", "Tiltshift");
    slider_saturacao = getTrackbarPos("Saturação", "Tiltshift");
    
    // Calcular parâmetros
    float centro = slider_centro * altura / 100.0;
    float faixa_altura = slider_altura * altura / 100.0;
    float decaimento = max(0.1f, slider_decaimento / 10.0f);
    
    float l1 = centro - faixa_altura/2;
    float l2 = centro + faixa_altura/2;
    
    // Criar máscara
    Mat mask(altura, largura, CV_32FC1);
    // equação da máscara usando tangente hiperbólica
    for(int i = 0; i < altura; i++) {
        float fx = 0.5 * (tanh((i - l1)/decaimento) - tanh((i - l2)/decaimento));
        //
        for(int j = 0; j < largura; j++) {
            mask.at<float>(i, j) = fx;
        }
    }
    
    // MÁSCARA PARA DEBUG
    Mat mask_display;
    mask.convertTo(mask_display, CV_8UC1, 255.0);
    imshow("Máscara TiltShift", mask_display);
    
    Mat resultado1, resultado2;// resultados parciais
    // converter a imagem e seu borrão para float
    Mat image_f, blurred_f;
    image.convertTo(image_f, CV_32FC3);
    blurred.convertTo(blurred_f, CV_32FC3);

    //seperando os canais da imagem original
    // Separar os canais da imagem original e da borrada
    vector<Mat> canais(3);
    vector<Mat> blurred_canais(3);
    vector<Mat> resultado_canais(3);

    split(image_f, canais);
    split(blurred_f, blurred_canais);

    // Aplicar a máscara a cada canal
    for (int i = 0; i < 3; i++) {
        Mat parte_nitida, parte_borrada;
        multiply(canais[i], mask, parte_nitida);           // parte nítida (área em foco)
        multiply(blurred_canais[i], 1.0 - mask, parte_borrada); // parte borrada (fora de foco)
        add(parte_nitida, parte_borrada, resultado_canais[i]);
    }

    // Reunir os 3 canais de volta
    Mat resultado_float;
    merge(resultado_canais, resultado_float);
        
    resultado_float.convertTo(result, CV_8UC3);
    
    // APLICAR AJUSTE DE SATURAÇÃO
    if(slider_saturacao != 50) {  
        Mat hsv;
        cvtColor(result, hsv, COLOR_BGR2HSV);// Converter para HSV
        
        vector<Mat> canais;
        split(hsv, canais);// o canal 1 é o da saturação
        
        // Calcular fator de saturação: 0.5x a 2.0x
        float fator = slider_saturacao / 50.0f;
        canais[1] = canais[1] * fator;//multiplicar o canal de saturação
        // Garantir que valores não ultrapassem 255
        min(canais[1], 255, canais[1]);//truncando
        
        merge(canais, hsv);
        cvtColor(hsv, result, COLOR_HSV2BGR);
    }
    
    imshow("Tiltshift", result);
}

//-----------------------------------------------MAIN-------------------------------------------------
int main(int argc, char** argv) {
    if (argc != 2) {
        cout << "Uso: " << argv[0] << " <imagem>" << endl;
        return -1;
    }

    image = imread(argv[1], IMREAD_COLOR);
    if (image.empty()) {
        cout << "Erro ao carregar imagem!" << endl;
        return -1;
    }

    altura = image.rows;
    largura = image.cols;
    

    // Criar versão borrada
    GaussianBlur(image, blurred, Size(15, 15), 0);

    namedWindow("Tiltshift", WINDOW_AUTOSIZE);
    namedWindow("Máscara TiltShift", WINDOW_AUTOSIZE); // Janela da máscara

    // Criar as barras
    createTrackbar("Altura Foco", "Tiltshift", &slider_altura, 100, tiltshift);
    createTrackbar("Suavização", "Tiltshift", &slider_decaimento, 100, tiltshift);
    createTrackbar("Centro", "Tiltshift", &slider_centro, 100, tiltshift);
    createTrackbar("Saturação", "Tiltshift", &slider_saturacao, 100, tiltshift);

    // Processamento inicial
    tiltshift(0, 0);
    
    waitKey(0);
    return 0;
}