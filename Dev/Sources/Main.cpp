////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Common.h"
#include "Demo.h"

#include "Gugu/Engine.h"
#include "Gugu/Resources/ManagerResources.h"

#if defined(GUGU_ENV_VISUAL )

    #define _CRTDBG_MAP_ALLOC
    #include <stdlib.h>
    #include <crtdbg.h>

#endif

using namespace demoproject;
using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

int main(int argc, char* argv[])
{
    #if defined(GUGU_ENV_VISUAL )

        _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

    #endif

    //Init engine
    EngineConfig config;
    config.applicationName = "Gugu Dice Roller";
    config.pathAssets = "Assets";
    config.defaultFont = "Roboto-Regular.ttf"; // Spaceranger  Roboto-Regular  
    config.debugFont = "Roboto-Regular.ttf";
    config.defaultTextureSmooth = true;
    config.windowWidth = 1024;
    config.windowHeight = 520;
    config.backgroundColor = sf::Color(232, 232, 232);

    GetEngine()->Init(config);

    //--------

    GetEngine()->SetApplication(new Demo);
    GetResources()->PreloadAll();

    //--------

    GetEngine()->RunMainLoop();
    GetEngine()->Release();

    return 0;
}
