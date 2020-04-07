package.path = package.path .. ";../GuguEngine/Tools/Build/?.lua"
require "PremakeEngine"
require "PremakeDemos"


-- Config
local pathDev = EnsureSlash("../Dev/")
local pathDevEngine = EnsureSlash("../GuguEngine/Dev/")
local pathSolution = EnsureSlash(pathDev.."Build/".._ACTION.."/")
local pathVersion = EnsureSlash("../Version")

BuildCfg = {
    -- Solution
    DirSolution     = EnsureSlash(pathSolution),
    SubDirBinaries  = EnsureSlash("Build_".._ACTION),
    
    -- Engine
    DirSourcesEngine    = EnsureSlash(pathDevEngine.."SourcesEngine"),
    DirSourcesSfml      = EnsureSlash(pathDevEngine.."SourcesSFML"),
    DirSourcesPugiXml   = EnsureSlash(pathDevEngine.."SourcesPugiXml"),
    DirLibEngine        = EnsureSlash(pathSolution.."Build"),
}


-- Build Solution
solution "DiceRoller"

    location (BuildCfg.DirSolution)
    
    package.guid = "8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942"
    configurations { "Debug", "Release" }
    platforms { "x86", "x64" }

    ProjectDefault(BuildCfg, "DiceRoller", pathDev.."Sources", pathVersion, "D17E774D-04C3-4F95-BFE4-707B0E89395C")
    ProjectLibGuguEngine(BuildCfg)
    ProjectLibSFML(BuildCfg)
    ProjectLibPugiXml(BuildCfg)
   
    