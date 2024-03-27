function is_num(new_char) {
    return new_char === "0" || new_char === "1" || new_char === "2" || new_char === "3"
            || new_char === "4" || new_char === "5" || new_char === "6" || new_char === "7"
            || new_char === "8" || new_char === "9" || new_char === ".";
}

function is_opt(new_char)
{
    return new_char === "/" || new_char === "x" || new_char === "-" || new_char === "+" ||
            new_char === "(" || new_char === ")";
}

function text_input(cur_text, new_char) {
    if (is_num(new_char) || is_opt(new_char)) {
        if (cur_text === "0" && new_char !== "0")
        {
            return new_char;
        }
        return cur_text + new_char;
    }
    else if (new_char === "DEL" && cur_text !== "0" && cur_text !== "") {
        var result = cur_text.slice(0, -1);
        return result === "" ? "0" : result;
    }
    else if (new_char === "AC")
    {
        return "0";
    }

    return cur_text;
}
