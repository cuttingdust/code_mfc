#include <iostream>

class CBoy
{
public:
    const int *GetRuntimeClass()
    {
        return &classCBoy;
    }
    static const int classCBoy;
};

const int CBoy::classCBoy = 1;

class CGirl
{
public:
    const int *GetRuntimeClass()
    {
        return &classCGirl;
    }
    static const int classCGirl;
};

const int CGirl::classCGirl = 1;


int main(int argc, char *argv[])
{
    CBoy  boy;
    CGirl girl;
    if (boy.GetRuntimeClass() == &CBoy::classCBoy)
    {
        printf("这是一个男孩\n");
    }

    if (girl.GetRuntimeClass() == &CGirl::classCGirl)
    {
        printf("这是一个女孩\n");
    }

    return 0;
}
