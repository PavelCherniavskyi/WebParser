#include "WebParser.h"

int main(int argc, char *argv[])
{
    WebParser app(argc, argv);
    app.init();
    app.run();

    return app.exec();
}
