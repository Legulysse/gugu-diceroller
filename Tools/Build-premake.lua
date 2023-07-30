
-- Setup the include path of the engine build scripts directory
package.path = package.path .. ";../GuguEngine/Tools/Build/?.lua"

require "PremakeUtility"

-- Solution Configuration
local pathDev       = EnsureSlash("../Dev")
local pathVersion   = EnsureSlash("../Version")

BuildCfg = SetupBuildCfg("../GuguEngine")


-- Build Solution
solution "DiceRoller"
    IncludeDefaultSolutionDefinition(BuildCfg, pathDev.."Build")

    package.guid = "8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942"

    ProjectDefault(BuildCfg, "DiceRoller", pathDev.."Sources", pathVersion, "D17E774D-04C3-4F95-BFE4-707B0E89395C")
    ProjectLibGuguEngine(BuildCfg)
    ProjectLibSFML(BuildCfg)
    ProjectLibPugiXml(BuildCfg)
    ProjectLibImGui(BuildCfg)
   
    