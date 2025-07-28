function AlPrint(text)
    DrawString(0, 0, 255, 255, 255, text)
end

local input = ""

while true do
    local letter = GetKey()

    if letter == "\r" then
        Clear(0, 0, 0)
        AlPrint("Hello " .. input .. "!")
        input = ""
    elseif letter == "\b" then
        input = input:sub(1, -2)
    elseif letter == "\x1b" then
        break
    elseif letter and #letter == 1 then
        input = input .. letter
    end

    Clear(0, 0, 0)
    AlPrint(input)
end
