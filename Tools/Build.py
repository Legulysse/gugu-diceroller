
dirEngineBuildScripts = '../GuguEngine/Tools/Build'
dirBuildScripts = '.'


import sys, os, collections

sys.path.append(dirEngineBuildScripts)
import GuguUtility


def Main():
    
    dirCurrent = os.getcwd()
    os.chdir(dirBuildScripts)

    #Menu : Actions
    dictChoices = collections.OrderedDict()
    dictChoices['premake'] = 'Generate Solution'
    aActions = GuguUtility.PromptMenu("Select Operations", dictChoices, True)

    #Menu : Compilers
    aCompilers = []
    if any(x in ['premake'] for x in aActions):
        dictChoices = collections.OrderedDict()
        dictChoices['vs2019'] = 'Visual 2019'
        aCompilers = GuguUtility.PromptMenu("Select Compilers", dictChoices, True)
    
    #Start Build
    for compiler in aCompilers:
        if 'premake' in aActions:
            GuguUtility.Premake(dirEngineBuildScripts, 'BuildGame.lua', compiler)

    os.chdir(dirCurrent)

    print('')
    input("Press Enter to close.")


Main()
