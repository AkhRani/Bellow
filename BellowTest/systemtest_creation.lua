ResetFailures()
ExpectEq(1, GetPlayerCount(), "Player Count")
ExpectEq(30, GetGalaxySize(), "Galaxy Size")
ExpectEq(48, GetSystemCount(), "System Count")
ExpectEq({y=7.9220779220779 , x=4.5454545454545, name="Indus", fact=10, pop=50}, GetSystemInfo(1), "System Info")
ExpectEq(1, GetFleetCount(), "Fleet Count")
ExpectEq({y=7.9220779220779 , x=4.5454545454545}, GetFleetInfo(1), "Fleet Info")