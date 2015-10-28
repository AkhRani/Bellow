g_Failures = ""

Rep = function (ob)
  local retval = ""
  if (type(ob) == 'table') then
    retval = "{"
    for i,v in pairs(ob) do
      retval = retval .. tostring(i) .. "=" .. Rep(v) .. " "
    end
    retval = retval .. "}"
  else
    retval = tostring(ob)
  end
  return retval
end

function ResetFailures()
  g_Failures = ""
end

function ExpectEq(e, a, desc)
  local desc = desc or "test"
  local re = Rep(e)
  local ra = Rep(a)
  if re ~= ra then
    local failure = desc .. " failed: expected " .. re .. " actual " .. ra .. "\n"
    -- io.write(failure)
    g_Failures = g_Failures .. failure
  end
end
