#Territorial

A game of territorial ants!

##Introduction
Territorial is a 2D RTS game seen from above. The player controls an ant colony and the goal is to defeat the opposing teams by destroying their anthills. To succeed, the colony must be in constant growth, both by production, resource acquisition and territory, and hinder the growth of enemy colonies. 


###Game flow
Resources may only be collected from owned territory, so the colony's territory must expand by claiming it. If no other colony can contest the claim, it is given to the claiming colony. This will eventually result in clashing borders. To expand the territory the colony needs a military, and to build a military the colony needs resources, and to get resources the colony needs territory. 

Since borders are dynamic, the players have complete control in strategizing border placements. A good bottleneck could be key to victory.

There is a simple state of diplomacy between the ant colonies. Either they are enemies, neutral or allies. Diplomatic status to another colony may be changed at will at any time during a match. A bit of diplomacy could be key to defeating that unstoppable enemy.



##Development
The game is currently very early in development and is far away from a working product. Before starting to implement the actual content listed under [future plans]( "future plans") below, the following must be developed:

* ~~Camera movement~~
* Unit movement
* Unit AI
* Combat
* Different teams
* Diplomacy
* Territory claiming

and lots of other basic, frameworky things.


###Future plans
As a finished product, Territorial is planned to implement the following (in no particular order).

####Different maps
Being able to choose between different maps to play in.


####Resources
Two resources:

* Building material - For constructing buildings and maintaining them.
* Food - For constructing units and maintaining them.

There are spots of natural resources generated from the start of the game. Both building material and food is collected from different types of spots. Food may also be collected from dead creatures (including ants).

####Units
A small set of simple units:

* Worker - Construction and resource acquisition.
* Scout - Scouting uncharted terrain.
* Fighter - Combat.

####Buildings
There is not much to construct except:

* Anthill - Resource storage and unit constructor.
* Resource line - A path connecting an anthill to resource collection sites.


##Technical information
Here follows technical information concerning the contents of this repository.

##Building
Building has only been tested on Windows 7 64-bit.

###Dependancies
####SFML 2.1
Building has only been tested with SFML's static debugging libraries using MinGW g++ 32-bit.   

##Running
Running has only been tested on Windows 7 64-bit.




