function printf(format, ...)
    local arg = {...}
    io.write(string.format(format, unpack(arg)))
end

function bool(value)
    local typev = type(value)
    if typev == "boolean" then
        return value
    elseif typev == "number" or typev == "integer" then
        return value ~= 0
    end
    return false
end