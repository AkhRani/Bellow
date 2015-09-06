Map = function (galaxy, width, height)
  height = height or 30
  width = width or 30

  local rows = { }
  for i = 1, height do
    rows[i] = { }
  end

  for i,sys in ipairs(galaxy.planets) do
    local x = math.floor((sys.x * width + width - 1)/ galaxy.size)
    local y = math.floor((sys.y * height + height - 1)/ galaxy.size)
    if x > 30 or y > 30 then
      print (sys.name, sys.x, sys.y, x, y)
    end
    rows[y][x] = 1
  end

  for i = 1, height do
    for j = 1, width do
      if (rows[i][j] == 1) then
        io.write("*")
      else
        io.write(".")
      end
    end
    io.write("\n")
  end

end
