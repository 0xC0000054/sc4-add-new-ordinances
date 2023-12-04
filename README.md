# sc4-add-new-ordinances

A DLL Plugin for SimCity 4 that experiments with adding a new ordinance to the game. The game's ordinance simulator is one of the few systems that allows a DLL to add new code for the game to use in its simulations. 

The plugin will show up in the ordinances list as `New ordinance test1`. The ordinance is an expense ordinance with the same monthly cost as the Power Conservation ordinance ($-0.005 \times city population$).

The plugin currently sends the following effects to the game: Mayor Rating, Commercial Demand Effect, Travel Strategy Modifier and Air Effect by zone type.
All of those effects are using the default values from Ingred.ini, so the shouldn't have any in-game effect.

## Disclaimer:

This mod is experimental, you probably should not test it in a city you care about.

## Features:

* Adds a new ordinance into the game that does not replace any of the built-in ones.
* Allows the ordinance to have a custom monthly cost calculation or availability conditions (e.g. starting year or specific buildings that must be in the city).
* The game persists the ordinance's state to the city save file.

## Limitations:

* The available ordinance effects are restricted to the ones that Maxis already included in the game.
  * These effects are hard-coded into the various game systems, e.g. the *Travel Strategy Modifier* behavior is hard-coded into the traffic simulator.

## System Requirements

Windows 10 or later.

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

[cGZNewOrdinanceTest.cpp](src/cGZNewOrdinanceTest.cpp) is the main plugin file. It handles the setup that SC4 requires to load a DLL, setting the ordinance effects, and adding the ordinance into the game.

[TestOrdinance.h](src/TestOrdinance.h) configures the ordinance costs and whether it is an income or expense. Note that there are other methods that can be overridden
to customize the ordinance logic, see [OrdinanceBase.h](src/OrdinanceBase.h). For example:

* `GetCurrentMonthlyIncome` could be overridden to use a custom monthly cost calculation, built-in examples include the `Legalized Gambling` and `Tourism Promotion Program` ordinances.
* `CheckConditions` could be overridden to set custom conditions for the ordinance to become available, built-in examples include the `Neighborhood Watch` and `Trash Presort Requirement` ordinances. Note that you do not need to override `CheckConditions` to set the in-game year that the ordinance becomes available, for that you override the `GetYearFirstAvailable` method that is inherited from `OrdinanceBase`.

[OrdinanceBase.cpp](src/OrdinanceBase.cpp) is the base class for `TestOrdinance`. It handles the common logic and provides a default implementation for `GetCurrentMonthlyIncome`.
The default monthly cost calculation is used by a number of ordinances, e.g. the `Community CPR Training Program`, `Clean Air`, etc. 

[OrdinancePropertyHolder.cpp](src/OrdinancePropertyHolder.cpp) provides the game with a list of effects that should be applied when the ordinance is enabled.
The effects can include Mayor Rating boosts, Demand boosts, etc.