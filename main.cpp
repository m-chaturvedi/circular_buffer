#include <chrono>
#include <csignal>
#include <iostream>
#include <vector>
using namespace std;
using namespace chrono;

#define DEBUG 0
#if DEBUG == 1
    #include <icecream.hpp>
#else
    #define IC(...)
#endif


// Size of the Circular Buffer:
const size_t N = 1LL << (30); // 1 GB

// const size_t N = 1LL << (30 + 4);

namespace {
    volatile std::sig_atomic_t gSignalStatus;
}

unsigned char* A;
size_t tail = 0;
bool wrapped = 0;

void signal_handler(int signal)
{
    auto start = high_resolution_clock::now();
    gSignalStatus = signal;
    IC(signal, tail, wrapped);
    FILE* fp = fopen("buffer_output.bin", "wb");
    size_t head = wrapped ? (tail + 1) % N : 0;
    size_t i = head;

    if (wrapped) {
        fwrite(A + head, sizeof(char), N - head, fp);
        fwrite(A, sizeof(char), head, fp);
    } else
        fwrite(A, sizeof(char), tail + 1, fp);

    fclose(fp);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    float seconds = static_cast<float>(duration.count()) / 1'000'000;
    cout << "Seconds: " << seconds << endl;
    cout << "Write speed: " << N / (seconds * (1 << 20)) << " MBps" << endl;
    delete[] A;
    exit(0);
}

void write_pid()
{
    FILE* fp = fopen("circular_buffer.pid", "w");
    fprintf(fp, "%d", getpid());
    fclose(fp);
}

int main(int argc, char* argv[])
{
    write_pid();
    std::signal(SIGINT, signal_handler);
    A = new unsigned char[N];
    unsigned int b;
    size_t num_loops = 0;
    while (1) {
        size_t num_read = fread(A, sizeof(unsigned char), N, stdin);
        IC(num_read);
        if (num_read == 0)
            signal_handler(SIGUSR1);
        tail = num_read - 1;
        if (num_loops > 1 && num_read < N)
            wrapped = 1;
        ++num_loops;
    }

    sleep(100);
    delete[] A;
    return 0;
}
