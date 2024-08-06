#!/usr/bin/env python3

import sys
import io
import re

import lib

if len(sys.argv) < 4:
    print(f"usage: {sys.argv[0]} <input md> <input config.h> <output md> <output md-html>")
    exit(-1)

input_md_path = sys.argv[1]
config_path = sys.argv[2]
output_md_path = sys.argv[3]
output_web_path = sys.argv[4]

config = lib.get_config_dict(config_path)

input = open(input_md_path).read()
input = lib.apply_config(config, input)

# remove C++ names from titles
# e.g. '## show python `cmd_show_python_help`' -> '## show python'
input = re.sub(r'^(##.*) `.*`$', r'\1', input, flags=re.M)

# output human friendly markdown
output = open(output_md_path, "w")
output.write(input)

# now process the markdown a little more to make it html friendly

# # escape < and > so we don't accidentally open html tags
input = re.sub(r'<', r'&lt;', input)
input = re.sub(r'>', r'&gt;', input)

output = open(output_web_path, "w")

blurb = """<!--
this is an auto generated file, don't edit it by hand.
edit help_input.md and regenerate with generate_help_web.py.
see /doc/help/README.md.
-->
"""

output.write(blurb)
output.write(input)
