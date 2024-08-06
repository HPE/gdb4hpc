# given a path to a .h file, return a dictionary with all the #defines in the .h file
def get_config_dict(path):
    config = open(path, "r")
    result = {}

    for line in config.readlines():
        define = line.find("#define ")
        if define == -1:
            continue

        cursor = len("#define ")
        key = line[cursor : line.find(" ", cursor)]

        cursor = line.find(" ", cursor)
        value = line[cursor + 1:].strip()

        if len(value) > 1 and value[0] == '"' and value[len(value) - 1] == '"':
            value = value[1:-1]

        result[key] = value

    return result

# search for @KEYS@ in `text` and replace them with values in `config_dict`
def apply_config(config_dict, text) -> str:
    for k, v in config_dict.items():
        text = text.replace(f"@{k}@", v)
    return text

