star_names = {
  "Andromeda",
  "Antlia",
  "Apus",
  "Aquarius",
  "Aquila",
  "Ara",
  "Aries",
  "Auriga",
  "Bootes",
  "Caelum",
  "Camelopardalis",
  "Cancer",
  "Canes Venatici",
  "Canis Major",
  "Canis Minor",
  "Capricornus",
  "Carina",
  "Cassiopeia",
  "Centaurus",
  "Cepheus",
  "Cetus",
  "Chamaeleon",
  "Circinus",
  "Columba",
  "Coma Berenices",
  "Corona Australis",
  "Corona Borealis",
  "Corvus",
  "Crater",
  "Crux",
  "Cygnus",
  "Delphinus",
  "Dorado",
  "Draco",
  "Equuleus",
  "Eridanus",
  "Fornax",
  "Gemini",
  "Grus",
  "Hercules",
  "Horologium",
  "Hydra",
  "Hydrus",
  "Indus",
  "Lacerta",
  "Leo",
  "Leo Minor",
  "Lepus",
  "Libra",
  "Lupus",
  "Lynx",
  "Lyra",
  "Mensa",
  "Microscopium",
  "Monoceros",
  "Musca",
  "Norma",
  "Octans",
  "Ophiuchus",
  "Orion",
  "Pavo",
  "Pegasus",
  "Perseus",
  "Phoenix",
  "Pictor",
  "Pisces",
  "Piscis Austrinus",
  "Puppis",
  "Pyxis",
  "Reticulum",
  "Sagitta",
  "Sagittarius",
  "Scorpius",
  "Sculptor",
  "Scutum",
  "Serpens",
  "Sextans",
  "Taurus",
  "Telescopium",
  "Triangulum",
  "Triangulum Australe",
  "Tucana",
  "Ursa Major",
  "Ursa Minor",
  "Vela",
  "Virgo",
  "Volans",
  "Vulpecula"
}


function Shuffle(coll)
  local retval = { }
  local length = #coll
  for i = 1, length do
    retval[i] = coll[i]
  end

  for i = 1, length*2 do
    local idx1 = math.random(length)
    local idx2 = math.random(length)
    local swap = retval[idx2]
    retval[idx2] = retval[idx1]
    retval[idx1] = swap
  end

  return retval
end


function RandomMinDistance(galaxy, minDistance)
  local x, y
  -- min distance squared
  local mds = minDistance * minDistance
  local attempts = 0
  while attempts < 10 do
    x = math.random(231) * galaxy.size / 231
    y = math.random(231) * galaxy.size / 231
    local d = mds + 1
    for k, v in pairs(galaxy.systems) do
      local dx = v.x - x
      local dy = v.y - y
      local ds = dx * dx + dy * dy
      if ds < d then
        d = ds
        if d < mds then
          break
        end
      end
    end
    if d > mds then
      break
    end
  end
  if attempts < 10 then
    return x, y
  else
    return nil, nil
  end
end

function Homeworld(owner)
  return {
    name = "Alpha",
    owner = owner.name,
    base_population = 100,
    population = { amount = 50, invested = 0 },
    factories = { amount = 10, invested = 0 },
  }
end

function NewGame(planetCount, size, minDistance)
  planetCount = planetCount or 48
  size = size or 30
  minDistance = minDistance or 2.5

  players = { { name = "human" } }

  -- Select random subset of names
  randomNames = Shuffle(star_names)

  galaxy = { size = size, systems = { } }

  -- Place homeworlds first
  for i, player in ipairs(players) do
    print(player.name)
    x, y = RandomMinDistance(galaxy, minDistance * 4)
    if x and y then
      table.insert(galaxy.systems, {
        name = randomNames[i],
        color = "Yellow",
        x = x, y = y,
        planet = Homeworld(player)
      })
    else
      print ("Ran out of space.")
      return nil
    end
  end

  -- TODO:  Place 2 systems near homeworlds
  -- TODO:  Place Orion
  -- Place remaining systems
  for i = #players + 1, planetCount do
    x, y = RandomMinDistance(galaxy, minDistance)
    -- TODO:  Star colors and planet types
    if x and y then
      table.insert(galaxy.systems, {
        name = randomNames[i],
        color = "Yellow",
        x = x, y = y,
        planet = nil
      })
    else
      print ("Ran out of space.  ")
      break
    end
  end
  galaxy.size = size
  print (string.format("%d systems created", #galaxy.systems))

  return {
    players = players,
    galaxy = galaxy
  }
end
