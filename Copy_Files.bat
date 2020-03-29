copy "data files\country_mappings.txt" "release\EU3ToVic2\country_mappings.txt"
copy "data files\readme.txt" "release\EU3ToVic2\readme.txt"
copy "data files\province_mappings.txt" "release\EU3ToVic2\province_mappings.txt"
copy "data files\cultureMap.txt" "release\EU3ToVic2\cultureMap.txt"
copy "data files\slaveCultureMap.txt" "release\EU3ToVic2\slaveCultureMap.txt"
copy "data files\religionMap.txt" "release\EU3ToVic2\religionMap.txt"
copy "data files\unions.txt" "release\EU3ToVic2\unions.txt"
copy "data files\governmentMapping.txt" "release\EU3ToVic2\governmentMapping.txt"
copy "data files\port_whitelist.txt" "release\EU3ToVic2\port_whitelist.txt"
copy "data files\port_blacklist.txt" "release\EU3ToVic2\port_blacklist.txt"
copy "data files\starting_factories.txt" "release\EU3ToVic2\starting_factories.txt"
copy "data files\merge_nations.txt" "release\EU3ToVic2\merge_nations.txt"
copy "data files\configuration.txt" "release\EU3ToVic2\configuration.txt"
copy "data files\blocked_tech_schools.txt" "release\EU3ToVic2\blocked_tech_schools.txt"
copy "data files\leader_traits.txt" "release\EU3ToVic2\leader_traits.txt
copy "data files\regiment_costs.txt" "release\EU3ToVic2\regiment_costs.txt
copy "data files\license.txt" "release\EU3ToVic2\license.txt"
copy "data files\Eu3ToVic2DefaultConfiguration.xml" "release\Configuration\Eu3ToVic2DefaultConfiguration.xml"
copy "data files\SupportedConvertersDefault.xml" "release\Configuration\SupportedConvertersDefault.xml"

del release\EU3ToVic2\changelog.txt
git log --oneline --decorate >> release/log.txt
(for /f "delims=" %%i in (release/log.txt) do @echo %%i)>release/changelog.txt
del release\EU3ToVic2\log.txt

del "release\EU3ToVic2\blankMod" /Q
mkdir "release\EU3ToVic2\blankMod"
mkdir "release\EU3ToVic2\blankMod\output"
mkdir "release\EU3ToVic2\blankMod\output\history"
mkdir "release\EU3ToVic2\blankMod\output\history\provinces"
mkdir "release\EU3ToVic2\blankMod\output\history\provinces\africa"
mkdir "release\EU3ToVic2\blankMod\output\history\provinces\asia"
mkdir "release\EU3ToVic2\blankMod\output\history\provinces\australia"
mkdir "release\EU3ToVic2\blankMod\output\history\provinces\austria"
mkdir "release\EU3ToVic2\blankMod\output\history\provinces\balkan"
mkdir "release\EU3ToVic2\blankMod\output\history\provinces\canada"
mkdir "release\EU3ToVic2\blankMod\output\history\provinces\carribean"
mkdir "release\EU3ToVic2\blankMod\output\history\provinces\central asia"
mkdir "release\EU3ToVic2\blankMod\output\history\provinces\china"
mkdir "release\EU3ToVic2\blankMod\output\history\provinces\france"
mkdir "release\EU3ToVic2\blankMod\output\history\provinces\germany"
mkdir "release\EU3ToVic2\blankMod\output\history\provinces\india"
mkdir "release\EU3ToVic2\blankMod\output\history\provinces\indonesia"
mkdir "release\EU3ToVic2\blankMod\output\history\provinces\italy"
mkdir "release\EU3ToVic2\blankMod\output\history\provinces\japan"
mkdir "release\EU3ToVic2\blankMod\output\history\provinces\low countries"
mkdir "release\EU3ToVic2\blankMod\output\history\provinces\mexico"
mkdir "release\EU3ToVic2\blankMod\output\history\provinces\pacific island"
mkdir "release\EU3ToVic2\blankMod\output\history\provinces\portugal"
mkdir "release\EU3ToVic2\blankMod\output\history\provinces\scandinavia"
mkdir "release\EU3ToVic2\blankMod\output\history\provinces\south america"
mkdir "release\EU3ToVic2\blankMod\output\history\provinces\soviet"
mkdir "release\EU3ToVic2\blankMod\output\history\provinces\spain"
mkdir "release\EU3ToVic2\blankMod\output\history\provinces\united kingdom"
mkdir "release\EU3ToVic2\blankMod\output\history\provinces\usa"
mkdir "release\EU3ToVic2\blankMod\output\history\countries"
mkdir "release\EU3ToVic2\blankMod\output\history\diplomacy"
mkdir "release\EU3ToVic2\blankMod\output\history\units"
mkdir "release\EU3ToVic2\blankMod\output\history\pops"
mkdir "release\EU3ToVic2\blankMod\output\history\pops\1836.1.1"
mkdir "release\EU3ToVic2\blankMod\output\history\wars"
mkdir "release\EU3ToVic2\blankMod\output\common"

xcopy "Data Files\countries" "release\EU3ToVic2\blankmod\output\common\countries" /Y /E /I
xcopy "Data Files\gfx" "release\EU3ToVic2\blankmod\output\gfx" /Y /E /I
xcopy "Data Files\interface" "release\EU3ToVic2\blankmod\output\interface" /Y /E /I
xcopy "Data files\localisation" "release\EU3ToVic2\blankmod\output\localisation" /Y /E /I
xcopy "Data files\wars" "release\EU3ToVic2\blankmod\output\wars" /Y /E /I
copy "Data Files\countries.txt" "release\EU3ToVic2\blankMod\output\common\countries.txt"
copy "Data Files\country_colors.txt" "release\EU3ToVic2\blankMod\output\common\country_colors.txt"
copy "Data Files\religion.txt" "release\EU3ToVic2\blankMod\output\common\religion.txt"
copy "Data Files\cultures.txt" "release\EU3ToVic2\blankMod\output\common\cultures.txt"
