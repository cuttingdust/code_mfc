#include <afxwin.h>

UINT MyFunc(LPVOID)
{
    printf("我的线程ID是:%d\n", AfxGetThread()->m_nThreadID);
    return 0;
}

int main(int argc, char *argv[])
{
    for (int i = 0; i < 10; i++)
    {
        AfxBeginThread(MyFunc, NULL);
    }

    return 0;
}
