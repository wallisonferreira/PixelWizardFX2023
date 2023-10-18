#include <iostream>
#include <vector>
#include <thread>
#include <future>
#include <functional>
#include <map>

using namespace std;

inline void filterBlueMultithread(
    int parteInicio,
    int parteFinal,
    int channels,
    unsigned char* img) {

    for (int i = parteInicio; i < parteFinal; i += channels) {
        img[i] = 0;     // Canal vermelho
        img[i + 1] = 0;   // Canal verde
    }
}

inline void filterGrayScaleMultithread(
    int parteInicio, 
    int parteFinal,
    int channels,
    unsigned char* img) {

    for (int i = parteInicio; i < parteFinal; i += channels) {
        int gray = (img[i] + img[i + 1] + img[i + 2]) / 3;
        img[i] = gray;
        img[i + 1] = gray;
        img[i + 2] = gray;
    }
}

inline void filterDarknessMultithread(
    int parteInicio,
    int parteFinal,
    int width,
    int height,
    int channels,
    unsigned char* img) {

    // simple darkness
    for (int i = parteInicio; i < parteFinal; i++) {
        int x = i % width;
        int y = i / width;

        for (int c = 0; c < 3; c++) {
            if (img[(y * width + x) * 3 + c] >= 50) {
                img[(y * width + x) * 3 + c] -= 50;
            }
            else if (img[(y * width + x) * 3 + c] <= 50) {
                img[(y * width + x) * 3 + c] = 0;
            }
        }
    }
}

inline void filterSaltAndPepperMultithread(
    int parteInicio,
    int parteFinal,
    int width,
    int height,
    int channels,
    unsigned char* img) {

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

inline void filterMultithread(int filterNumber,
    int width, 
    int height, 
    int channels, 
    unsigned char *img) {

    unsigned nbThreadsHint = thread::hardware_concurrency();
    unsigned nbThreads = nbThreadsHint == 0 ? 8 : (nbThreadsHint);

    unsigned nbElements = width * height * channels;

    unsigned batchSize = (nbElements / nbThreads);
    unsigned batchRemainder = (nbElements % nbThreads);

    std::vector<std::thread> myThreads(nbThreads);

    // blue
    if (filterNumber == 1) {
        for (unsigned i = 0; i < nbThreads; ++i) {
            int parteInicio = i * batchSize;
            myThreads[i] = std::thread(
                filterBlueMultithread,
                parteInicio,
                parteInicio + batchSize,
                channels,
                img
            );
        }
    }

    if (filterNumber == 2) {
        for (unsigned i = 0; i < nbThreads; ++i) {
            int parteInicio = i * batchSize;
            myThreads[i] = std::thread(
                filterGrayScaleMultithread,
                parteInicio,
                parteInicio + batchSize,
                channels,
                img
            );
        }
    }

    std::for_each(myThreads.begin(), myThreads.end(), std::mem_fn(&std::thread::join));
}