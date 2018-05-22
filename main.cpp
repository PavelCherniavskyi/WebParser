#include "WebParser.h"

int main(int argc, char *argv[])
{
    WebParser app(argc, argv);
    app.init();

    return app.exec();
}
