function NewGame()
  human = {
    name = "Human"
  }

  starting = { amount = 10, invested = 0 }

  unowned = {
    name = "Sol",
    color = "yellow",
    owner = "Human",
    x = .5, y = .5,
    planet = { name = "Earth", base_population = 100, population = starting, factories = starting }
  }

  empty = {
    name = "Alpha Centurai",
    color = green,
    x = .9, y = .5
  }

  return {
    players = { human },
    galaxy = { empty, unowned }
  }
end

width = 100