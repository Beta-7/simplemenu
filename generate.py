import yaml

def load_yaml_from_file(file_path):
    with open(file_path, 'r') as file:
        return yaml.safe_load(file)

def generate_config_file(file_path, menu_data):
    with open(file_path, 'r') as file:
        config = file.read()
        config += f"#define HOME_ASSISTANT_URL \"{menu_data["homeAssistantUrl"]}\" \n"
        config += f"#define ACCESS_TOKEN \"{menu_data["accessToken"]}\" \n"
        config += f"#define SSID \"{menu_data["ssid"]}\" \n"
        config += f"#define PASSWORD \"{menu_data["password"]}\" \n"

    with open(file_path.replace(".template", ""), 'w') as file:
        file.write(config)

def generate_cpp_file(file_path, menu_data):
    with open(file_path, 'r') as file:
        cpp_code = file.read()
        cpp_code = cpp_code.replace("MenuItem mainMenu;", generate_cpp_code(menu_data))
        with open(file_path.replace(".template", ""), 'w') as file:
            file.write(cpp_code)


    

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
            # In case of entity-based menu items
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

def main():
    yaml_file_path = "menu.yaml"
    cpp_file_path = "src/main.cpp.template"
    config_file_path = "src/Config.h.template"
    
    try:
        data = load_yaml_from_file(yaml_file_path)
    except Exception as e:
        print(f"Error loading YAML file: {e}")
        return

    cpp_code = generate_cpp_code(data)
    generate_cpp_file(cpp_file_path, data)
    generate_config_file(config_file_path, data)

if __name__ == "__main__":
    main()
