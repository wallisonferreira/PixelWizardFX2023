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
        img[i] = 0;     // red
        img[i + 1] = 0;   // green
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
    int channels,
    unsigned char* img) {

    for (int i = parteInicio; i < parteFinal; i++) {
        if (img[i] >= 50) {
            img[i] -= 50;
        } else {
            img[i] = 0;
        }
    }
}

inline void filterSaltAndPepperMultithread(
    int parteInicio,
    int parteFinal,
    int channels,
    unsigned char* img) {

    /*for (int i = 0; i < width; i++) {
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
    }*/

    for (int i = parteInicio; i < parteFinal; i++) {
        int num = rand() % 100;

        if (num < 4) {
            if (num % 2 == 0) {
                img[i] = 255;
            }
            else {
                img[i] = 0;
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
        for (unsigned i = 0; i < nbThreads; i++) {
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

    // grayscale
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

    // darkness
    if (filterNumber == 3) {

        for (unsigned i = 0; i < nbThreads; ++i) {

            /*int parteFinal = (i == nbThreads - 1) 
                ? parteInicio + batchWidthSize : parteInicio + batchWidthSizeRemainder;*/

            int parteInicio = i * batchSize;

            myThreads[i] = std::thread(
                filterDarknessMultithread,
                parteInicio,
                parteInicio + batchSize,
                channels,
                img
            );
        }
    }

    // salt and pepper
    if (filterNumber == 4) {
        for (unsigned i = 0; i < nbThreads; ++i) {
            int parteInicio = i * batchSize;
            myThreads[i] = std::thread(
                filterSaltAndPepperMultithread,
                parteInicio,
                parteInicio + batchSize,
                channels,
                img
            );
        }
    }

    std::for_each(myThreads.begin(), myThreads.end(), std::mem_fn(&std::thread::join));
}