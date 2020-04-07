
dirEngineBuildScripts = '../GuguEngine/Tools/Build'
dirBuildScripts = '.'


import sys, os, collections

sys.path.append(dirEngineBuildScripts)
import GuguEngine
import Utility


def Main():
    
    dirCurrent = os.getcwd()
    os.chdir(dirBuildScripts)

    #Menu : Actions
    dictChoices = collections.OrderedDict()
    dictChoices['premake'] = 'Generate Solution'
    aActions = Utility.PromptMenu("Select Operations", dictChoices, True)

    #Menu : Compilers
    aCompilers = []
    if any(x in ['premake'] for x in aActions):
        dictChoices = collections.OrderedDict()
        dictChoices['vs2017'] = 'Visual 2017'
        aCompilers = Utility.PromptMenu("Select Compilers", dictChoices, True)
    
    #Start Build
    for compiler in aCompilers:
        if 'premake' in aActions:
            GuguEngine.Premake(dirEngineBuildScripts, 'BuildGame.lua', compiler)

    os.chdir(dirCurrent)

    print('')
    input("Press Enter to close.")


Main()
