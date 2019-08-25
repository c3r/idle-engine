#include <unistd.h>

constexpr auto FD_READ = 0;
constexpr auto FD_WRITE = 1;

class Pipe {
private:
    int _rd_channel;
    int _wr_channel;

public:
    Pipe()
    {
        int pipefds[2];
        pipe(pipefds);
        _rd_channel = pipefds[FD_READ];
        _wr_channel = pipefds[FD_WRITE];
    };
    ~Pipe() {};
    int GetReadChannel();
    int GetWriteChannel();
};

int Pipe::GetWriteChannel()
{
    return _wr_channel;
}
int Pipe::GetReadChannel()
{
    return _rd_channel;
}