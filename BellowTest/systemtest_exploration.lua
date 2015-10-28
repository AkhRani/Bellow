ResetFailures()
ExpectEq({x=5.4545454545455, y=4.4155844155844, name="?"}, GetSystemInfo(10), "Unexplored")
SetFleetDestination(1, 10)
EndTurn()
ExpectEq(0, GetExplorationEventCount(), "turn 1")
EndTurn()
ExpectEq(0, GetExplorationEventCount(), "turn 2")
EndTurn()
ExpectEq(0, GetExplorationEventCount(), "turn 3")
EndTurn()
ExpectEq(1, GetExplorationEventCount(), "arrival")
ExpectEq(10, GetExplorationEvent(1))
ExpectEq({x=5.4545454545455, y=4.4155844155844, name="Leo Minor"}, GetSystemInfo(10), "Explored")
EndTurn()
ExpectEq(0, GetExplorationEventCount(), "idle")