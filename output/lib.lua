function printf(format, ...)
    local arg = {...}
    io.write(string.format(format, unpack(arg)))
end