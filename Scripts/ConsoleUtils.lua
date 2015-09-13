CommonMap = function (width, height, size, systemCount, getSystemInfo)
  height = height or 30
  width = width or 30
  local gsize = galaxy.size

  local rows = { }
  for i = 1, height do
    rows[i] = { }
  end

  for i = 1, systemCount do
    system = getSystemInfo(i)
    local x = math.floor((system.x * width + gsize - 1)/ gsize)
    local y = math.floor((system.y * height + gsize - 1)/ gsize)
    if x > 30 or y > 30 or x < 1 or y < 1 then
      print (system.name, system.x, system.y, x, y)
    end
    rows[y][x] = i
  end

  for i = 1, height do
    for j = 1, width do
      id = rows[i][j]
      if (id) then
        local system = getSystemInfo(id)
        if (system.name) then
          io.write(string.sub(system.name, 1, 1))
        else
          io.write("?")
        end
      else
        io.write(" ")
      end
    end
    io.write("\n")
  end

end


-- Global map function, for testing only
GMap = function (galaxy, width, height)
  CommonMap(width, height, galaxy.size, #galaxy.systems,
      function(i) return galaxy.systems[i] end)
end

-- Player-specific map function, for use in game context
Map = function (width, height)
  CommonMap(width, height, GetGalaxySize(), GetSystemCount(), GetSystemInfo)
end
