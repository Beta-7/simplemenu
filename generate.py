import yaml
import subprocess
import os
import json
import jsonschema
import sys
from jsonschema import validate

def validate_and_load_yaml_from_file(yaml_file_path):

    with open("schema.json", 'r') as schema_file:
        schema = json.load(schema_file)

    with open(yaml_file_path, 'r') as yaml_file:
        yaml_data = yaml.safe_load(yaml_file)

    try:
        validate(instance=yaml_data, schema=schema)
        return yaml_data
    except jsonschema.exceptions.ValidationError as err:
        print(f"YAML validation error: {err.message}")
        raise
    except Exception as e:
        print(f"An error occurred: {str(e)}")
        raise

def generate_config_file(file_path, menu_data):
    with open(file_path, 'r') as file:
        config = file.read()
        config += f"#define HOME_ASSISTANT_URL \"{menu_data["homeAssistantUrl"]}\" \n"
        config += f"#define ACCESS_TOKEN \"{menu_data["accessToken"]}\" \n"
        config += f"#define SSID \"{menu_data["ssid"]}\" \n"
        config += f"#define PASSWORD \"{menu_data["password"]}\" \n"

    with open(file_path.replace(".template", ""), 'w') as file:
        file.write(config)
    print("Generating config from YAML format...")

def generate_cpp_file(file_path, menu_data):
    with open(file_path, 'r') as file:
        cpp_code = file.read()
        cpp_code = cpp_code.replace("MenuItem mainMenu;", generate_cpp_code(menu_data))
        with open(file_path.replace(".template", ""), 'w') as file:
            file.write(cpp_code)
    print("Generating menu from YAML format...")


def generate_cpp_code(menu_data):
    cpp_code = []
    
    def create_menu_item(name, items=None):
        cpp_item = f'MenuItem("{name}", {{'
        if items:
            sub_items = [generate_menu_item(sub_item) for sub_item in items]
            cpp_item += '\n'.join(sub_items)
        cpp_item += '}),'
        return cpp_item

    def generate_menu_item(item):
        if 'items' in item:
            return create_menu_item(item['name'], item['items'])
        elif 'entity' in item:
            entity = item['entity']
            if 'service' in item:
                service = item['service']
                return f'MenuItem("{item["name"]}", [](Adafruit_ST7735 &tft) {{ callServiceScreen(tft, "{service}", "{entity}"); }}),'
            else:
                return f'MenuItem("{item["name"]}", [](Adafruit_ST7735 &tft) {{ showValue(tft, "{entity}"); }}),'
        else:
            return f'MenuItem("{item["name"]}"),'

    def convert_main_menu(menu_data):
        return f'MenuItem("{menu_data["name"]}", {{' + '\n'.join(generate_menu_item(item) for item in menu_data["items"]) + '\n})'

    cpp_code.append(f'MenuItem mainMenu = {convert_main_menu(menu_data["mainMenu"])};')
    return '\n'.join(cpp_code)


def build_project():
    try:
        print("Building the project with PlatformIO...")
        result = subprocess.Popen(
            ['platformio', 'run'],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True
        )
        
        for line in result.stdout:
            print(line, end='')
        for line in result.stderr:
            print(line, end='', file=sys.stderr)
        
        result.wait()
        
        if result.returncode != 0:
            print(f"Build failed with return code {result.returncode}.")
            return False
        
    except Exception as e:
        print(f"Error during build: {e}")
        return False

    print("Build completed successfully.")
    return True

def upload_project():
    try:
        print("Uploading the project to ESP32...")
        result = subprocess.Popen(
            ['platformio', 'run', '--target', 'upload'],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True
        )
        
        for line in result.stdout:
            print(line, end='')
        for line in result.stderr:
            print(line, end='', file=sys.stderr)
        
        result.wait()
        
        if result.returncode != 0:
            print(f"Upload failed with return code {result.returncode}.")
            return False
        
    except Exception as e:
        print(f"Error during upload: {e}")
        return False

    print("Upload completed successfully.")
    return True

def main():
    yaml_file_path = "menu.yaml"
    cpp_file_path = "src/main.cpp.template"
    config_file_path = "src/Config.h.template"
    
    try:
        data = validate_and_load_yaml_from_file(yaml_file_path)
    except Exception as e:
        print(f"Error loading YAML file: {e}")
        return

    cpp_code = generate_cpp_code(data)
    generate_cpp_file(cpp_file_path, data)
    generate_config_file(config_file_path, data)

    if not build_project():
        print("Build failed. Exiting.")
        return

    if not upload_project():
        print("Upload failed. Exiting.")
        return

    print("Build, binary move, and upload completed successfully.")

if __name__ == "__main__":
    main()
