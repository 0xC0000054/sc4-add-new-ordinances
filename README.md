# sc4-add-new-ordinances

A DLL Plugin for SimCity 4 that experiments with adding new ordinances to the game.

The plugin will show up in the ordinances list as `New ordinance test1`. The ordinance is an expense ordinance with the same monthly cost as the Power Conservation ordinance ($-0.005 \times city population$).

The plugin currently sends the following effects to the game: Mayor Rating, Commercial Demand Effect, Travel Strategy Modifier and Air Effect by zone type.
All of those effects are using the default values from Ingred.ini, so the shouldn't have any in-game effect.

## Disclaimer:

This mod is experimental, you probably should not test it in a city you care about.

## Known Issues

* The ordinance dialog budget calculation preview is incorrect when this ordinance is enabled.
  * This appears to be a bug in that dialog's code as the correct value is shown in the monthly income/expenses panel. 
* The ordinance on/off state is not preserved when loading/saving a city.

## Download

The plugin can be downloaded from the Releases tab: https://github.com/0xC0000054/sc4-add-new-ordinances/releases   

# Installation

1. Close SimCity 4.
2. Copy `SC4AddNewOrdinance.dll` into the Plugins folder in the SimCity 4 installation directory.
3. Start SimCity 4.

# License

This project is licensed under the terms of the MIT License.    
See [LICENSE.txt](LICENSE.txt) for more information.

## 3rd party code

[gzcom-dll](https://github.com/nsgomez/gzcom-dll/tree/master) Located in the vendor folder, MIT License.    

# Source Code

## Prerequisites

* Visual Studio 2022

## Building the plugin

* Open the solution in the `src` folder
* Update the post build events to copy the build output to you SimCity 4 application plugins folder.
* Build the solution

## Debugging the plugin

Visual Studio can be configured to launch SimCity 4 on the Debugging page of the project properties.
I configured the debugger to launch the game in a window with the following command line:    
`-intro:off -CPUcount:1 -w -CustomResolution:enabled -r1920x1080x32`

You may need to adjust the resolution for your screen.

## Source Code Layout

[cGZNewOrdinanceTest.cpp](src/cGZNewOrdinanceTest.cpp) contains the plugin's COM director and registers the ordinance in the game.

[TestOrdinance.h](src/TestOrdinance.h) is the file that set the ordinance costs. Note that there are other methods that can be overridden
to customize the ordinance logic. For example:

* `GetCurrentMonthlyIncome` could be used to change the ordinance cost calculation, built-in examples include the `Legalized Gambling` and `Tourism Promotion Program`  ordinance.
* `CheckConditions` could be overridden to add custom conditions for enabling the ordinance, built-in examples include the `Neighborhood Watch` and `Trash Presort Requirement` ordinances.

[OrdinanceBase.cpp](src/OrdinanceBase.cpp) is the base class for `TestOrdinance`. It handles the common logic and provides a default implementation for `GetCurrentMonthlyIncome`.
The default monthly cost calculation is used by a number of ordinances, e.g. the `Community CPR Training Program`, `Clean Air`, etc. 

[OrdinancePropertyHolder.cpp](src/OrdinancePropertyHolder.cpp) provides the game with a list of effects that the ordinance that should be applied when the ordinance is enabled.
The effects can include Mayor Rating boosts, Demand boosts, etc.