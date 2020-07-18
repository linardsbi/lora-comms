import re, os
Import("env") # access to global build environment

class PATHS:
    route_file = "./include/CommsServer.hpp"
    base_path = "./src/"
    view_path = "./src/views/"
    public_dir = "./src/views/public/"

def after_build(source, target, env):
    with open(PATHS.route_file, "r") as file:
        for controller_path in find_controllers("".join(map(str,file.readlines()))):
            if os.path.exists(f"{PATHS.base_path + controller_path}.tmp"):
                env.Execute(f"mv {PATHS.base_path + controller_path}.tmp {PATHS.base_path + controller_path}")

def find_template_files(text):
    # FIXME: replace template parameters with HTML one by one, not all at once
    template_params = re.findall('\{".*"\}', text) # template synax: {"directory/filename"}
    paths = [f"{PATHS.view_path}{name[2:-2]}.html" for name in template_params]
    return paths, template_params

def add_js_css(html_text):
    isURL = lambda str : re.search('http[s]?://(?:[a-zA-Z]|[0-9]|[$-_@.&+]|[!*\(\), ]|(?:%[0-9a-fA-F][0-9a-fA-F]))+', str)
    for item in re.findall("(<script.*\/script>|<link.*stylesheet.*>)", html_text):
        script = item[1:7] == "script"

        if script:
            found = re.search("""src=(["'].*\.js["'])""", item)
            path = found.group()[5:-1]
        else:
            path = re.search("""href=(["'].*\.css["'])""", item).group()[6:-1]

        if isURL(path): continue # can't do anything with URLs since the whole thing is offline

        if path[0:2] == "./":
            path = path[2:]
        elif path[0] == "/":
            path = path[1:]

        with open(PATHS.public_dir + path, "r") as resource:
            # TODO: minify JS, CSS and HTML
            html_text = html_text.replace(item, 
                                          "<script>" + "".join(map(str, resource.readlines())) + "</script>" if script else
                                          "<style>" + "".join(map(str, resource.readlines())) + "</style>")

    return html_text

def find_controllers(route_text):
    return re.findall('controllers\/[^"]*|[A-z]+Controller\.h[p]{0,2}', route_text)

def preprocess(source, target, env):
    # Perhaps you can compile the temporary file and not do the renaming dance? 
    # That way less things could go wrong while compiling
    # maybe change the template syntax to be more versatile

    with open(PATHS.route_file, "r") as file:
        for controller_path in find_controllers("".join(map(str,file.readlines()))):
            paths = []
            template_params = []
            controller_text = ""

            with open(PATHS.base_path + controller_path, "r+") as controller_file:
                controller_text = "".join(map(str,controller_file.readlines()))
                paths, template_params = find_template_files(controller_text)
            
            if not paths or not template_params:
                continue

            env.Execute(f"mv {PATHS.base_path + controller_path} {PATHS.base_path + controller_path}.tmp")

            # FIXME:
            try:
                with open(PATHS.base_path + controller_path, "w") as file:
                    for i in range(len(paths)):
                        with open(paths[i], "r") as html_file:
                            processed_html = add_js_css("".join(map(str, html_file.readlines())))
                            controller_text = controller_text.replace(str(template_params[i]), 'PSTR(R"raw(' + processed_html + ')raw")', 1)
                    file.write(controller_text)

            except Exception as e:
                env.Execute(f"mv {PATHS.base_path + controller_path}.tmp {PATHS.base_path + controller_path}")
                raise e

env.AddPreAction("$BUILD_DIR/src/main.cpp.o", preprocess)
env.AddPostAction("$BUILD_DIR/src/main.cpp.o", after_build) # change files back


