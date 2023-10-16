#include <iostream>
#include <vector>

using namespace std;

inline void filterBlueSimple(unsigned char* img, int width, int height, int channels) {
    // Altere os canais vermelho e verde para zero
    for (int i = 0; i < width * height * channels; i += channels) {
        img[i] = 0;     // Canal vermelho
        img[i + 1] = 0;   // Canal verde
    }
}

inline void filterGrayScaleSimple(unsigned char* img, int width, int height, int channels) {
    for (int i = 0; i < width * height * channels; i += channels) {
        int gray = (img[i] + img[i + 1] + img[i + 2]) / 3;
        img[i] = gray;     // Canal vermelho
        img[i + 1] = gray;   // Canal verde
        img[i + 2] = gray;
    }
}

inline void filterDarknessSimple(unsigned char* img, int width, int height, int channels) {
    // simple darkness
    for (int i = 0; i < width; i++) {
        for (int y = 0; y < height; y++) {
            for (int c = 0; c < 3; c++) {
                if (img[(y * width + i) * 3 + c] >= 50) {
                    img[(y * width + i) * 3 + c] -= 50;
                }
                else if (img[(y * width + i) * 3 + c] <= 50) {
                    img[(y * width + i) * 3 + c] = 0;
                }
            }

        }
    }
}

inline void filterSaltAndPepperSimple(unsigned char* img, int width, int height, int channels) {
    // salt and pepper noise
    for (int i = 0; i < width; i++) {
        for (int y = 0; y < height; y++) {
            for (int c = 0; c < 3; c++) {
                int num = rand() % 100;
                if (num < 4) {
                    if (num % 2 == 0) {
                        img[(y * width + i) * 3 + c] = 255;
                    }
                    else {
                        img[(y * width + i) * 3 + c] = 0;
                    }
                }
            }
        }
    }
}