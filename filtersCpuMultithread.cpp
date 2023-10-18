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
    int width,
    int height,
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
    int width, 
    int height, 
    int channels, 
    unsigned char* img) {

    for (int i = 0; i < width * height * channels; i += channels) {
        int gray = (img[i] + img[i + 1] + img[i + 2]) / 3;
        img[i] = gray;     // Canal vermelho
        img[i + 1] = gray;   // Canal verde
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

}

inline void filterSaltAndPepperMultithread(
    int parteInicio,
    int parteFinal,
    int width,
    int height,
    int channels,
    unsigned char* img) {

}

using MapFilter = std::map<int, std::function<void(int, int, int, int, int, unsigned char*)>>;

inline void filterMultithread(int filterNumber, 
    unsigned nbElements, 
    int width, 
    int height, 
    int channels, 
    unsigned char *img,
    bool use_threads = true) {

    unsigned nbThreadsHint = thread::hardware_concurrency();
    unsigned nbThreads = nbThreadsHint == 0 ? 8 : (nbThreadsHint);

    unsigned batchSize = (nbElements / nbThreads);
    unsigned batchRemainder = (nbElements % nbThreads);

    std::vector<std::thread> myThreads(nbThreads);

    MapFilter mapFilter = {
        { 1, filterBlueMultithread },
        { 2, filterGrayScaleMultithread },
        { 3, filterDarknessMultithread },
        { 4, filterSaltAndPepperMultithread },
    };

    // filter blue
    if (use_threads)
    {
        for (unsigned i = 0; i < nbThreads; ++i) {
            int parteInicio = i * batchSize;

            myThreads[i] = std::thread(
                mapFilter[filterNumber],
                parteInicio,
                parteInicio + batchSize,
                width,
                height,
                channels,
                img
            );
        }
    }

    int inicio = nbThreads * batchSize;

    if (use_threads)
        std::for_each(myThreads.begin(), myThreads.end(), std::mem_fn(&std::thread::join));
}