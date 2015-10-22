# Bellow
This is intended to be a clone of the original Master Of Orion.  The goal is to be as close as possible to the original, but allow modifications (to races, AI, and tech tree for example) via scripts.  Even if this never gets close to completion, it will give me an excuse to mess around with technologies and techniques I don't get to use at work.

I'm not about to give detailed instructions to the approximately zero people who want to build this on their own.  I'm using gtest for the BellowTest project, and Lua for LuaConsole and (eventually) the full game.  Currently using VisualStudio 2013 to build the thing.

Design Notes, here for lack of a better place:

I'm trying to limit the interactions among my classes, but not having much success.

Handling the arrival of fleets at their destinations:
When a fleet arrives at its destination, it can trigger several events.

1. The fleet may merge with other fleets arriving this turn or already in orbit at the destination.
2. Combat may occur.
3. The fleet owner's information about the planet will be updated.
4. If the player has not explored this planet before, a notification will be queued.
5. If this is the first player to explore a planet with artifacts, the player will be awarded a technology and a notification will be queued.  (TODO after techs are added)

Trying to figure out which classes are responsible for which parts of this logic.
* StarSystems track "nearby" (orbiting and incoming) fleets.
* Arriving fleet queries system for nearby player fleet and calls owner.PendingMerge(id1, id2) or system.AddNearby(player, id1)
* After all fleets are moved, player merges fleets and updates fleet collection.  Approaching fleets will merge into orbiting fleets, if possible.
* After all players have moved, game scans systems for conflict situations and resolves conflicts.  Surviving fleets transition from "Approaching" to either "Retreating" or "Arriving" as a result of combat (or the lack therof).  Orbiting fleets may transition to "Retreating".
* Player scans fleets for "Arriving" status.  Arriving fleets transition to orbiting and call owner.Explore(system).
