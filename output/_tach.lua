unpack = unpack or table.unpack

function gen_puts(string)
    print(string)
end

function gen_putc(string)
    print(string)
end

function gen_itoa(number)
    return tostring(number)
end

function gen_printf(format, ...)
    arg = {...}
    io.write(string.format(format, unpack(arg)))
end

function gen_printfln(format, ...)
    arg = {...}
    io.write(string.format(format, unpack(arg)))
    io.write("\n")
end

function gen_atoi(string)
    return tonumber(string)
end

function _op_add(num1, num2)
    return num1 + num2
end

function _op_mul(num1, num2)
    return num1 * num2
end

function _op_sub(num1, num2)
    return num1 - num2
end

function _op_div(num1, num2)
    return num1 / num2
end