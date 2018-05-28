#pragma once

class LibCurlConfig
{
public:
    LibCurlConfig();
    ~LibCurlConfig();

    static void init();

private:
    static bool initialysed;
};
