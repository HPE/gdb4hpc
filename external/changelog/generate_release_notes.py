#! /usr/bin/env python3

# https://realpython.com/primer-on-jinja-templating/
# https://pypi.org/project/Jinja2/

from argparse import ArgumentParser
from os import path

import yaml
from jinja2 import Environment, FileSystemLoader

if __name__ == "__main__":
    home = path.expanduser("~")
    parser = ArgumentParser()
    parser.add_argument('-t', "--template_file", required=True,
                        help="Jinja2 template file for release notes")
    parser.add_argument('-y', "--yaml_data_file", required=True, help="YAML data file for release notes")
    parser.add_argument('-d', "--dest_dir", default=home, help="Dir to put new release note .md file")
    args = parser.parse_args()

    abs_path = path.abspath(args.template_file)
    template_dir, template_file = path.split(abs_path)

    print("Reading yaml data file: " + args.yaml_data_file)

    # Load template
    environment = Environment(loader=FileSystemLoader(template_dir))
    template = environment.get_template(template_file)

    # Load YAML data
    with open(args.yaml_data_file, encoding='utf-8', mode='r') as file:
        data = yaml.safe_load(file)

    # Apply the YAML data to the template and write it to a file
    content = template.render(data=data)
    filename = args.dest_dir + "/release_notes.md.in"
    with open(filename, mode='w', encoding="utf-8") as message:
        message.write(content)
        print(filename)