Map = function (galaxy, width, height)
  height = height or 10
  width = width or 10

  local rows = { }
  for i = 1, height do
    rows[i] = { }
  end

  for i,sys in ipairs(galaxy) do
    local x = math.floor(sys.x * width)
    local y = math.floor(sys.y * height)
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
