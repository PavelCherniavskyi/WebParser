#pragma once

class LibCurlConfig
{
public:
    LibCurlConfig();
    ~LibCurlConfig();

    void init();

private:
    static bool initialysed;
};
