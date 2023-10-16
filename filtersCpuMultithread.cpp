#include <iostream>
#include <vector>
#include <thread>
#include <future>
#include <functional>
//#include <iostream>

using namespace std;

inline void operation(int parteInicio, int parteFinal, int channels, unsigned char *img) {
    for (int i = parteInicio; i < parteFinal; i += channels) {
        img[i] = 0;     // Canal vermelho
        img[i+1] = 0;   // Canal verde
    }
}

inline void parallel(unsigned nbElements, int width, int height, int channels,
    unsigned char *img,
    bool use_threads = true) {

    unsigned nbThreadsHint = thread::hardware_concurrency();
    unsigned nbThreads = nbThreadsHint == 0 ? 8 : (nbThreadsHint);

    unsigned batchSize = (nbElements / nbThreads);
    unsigned batchRemainder = (nbElements % nbThreads);

    std::vector<std::thread> myThreads(nbThreads);

    if (use_threads)
    {
        for (unsigned i = 0; i < nbThreads; ++i) {
            // int start = i * batchSize;
            int parteInicio = i * batchSize;
            myThreads[i] = std::thread(operation, parteInicio, parteInicio + batchSize, channels, img);
        }
    }

    // TODO: resolver o problema de número ímpar 
    // (width*height*channels) / numberThreads não pode ser quebrado

    int inicio = nbThreads * batchSize;

    if (use_threads)
        std::for_each(myThreads.begin(), myThreads.end(), std::mem_fn(&std::thread::join));
}