import re, os
Import("env") # access to global build environment

def after_build(source, target, env):
    route_file = "./include/CommsServer.hpp"
    env.Execute(f"mv {route_file}.tmp {route_file}")

def findFiles(line):
    # FIXME: replace template parameters with HTML one by one, not all at once
    template_params = re.findall('\{".*"\}', line) # template synax: {"directory/filename"}
    base_path = "./src/views/"
    paths = []
    for name in template_params:
        paths.append(f"{base_path}{name[2:-2]}.html")

    return paths, template_params

def add_js_css(html_text):
    public_dir = "./src/views/public/"
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

        with open(public_dir + path, "r") as resource:
            # TODO: minify JS, CSS and HTML
            html_text = html_text.replace(item, 
                                          "<script>" + "".join(map(str, resource.readlines())) + "</script>" if script else
                                          "<style>" + "".join(map(str, resource.readlines())) + "</style>")

    return html_text

def preprocess(source, target, env):
    # Perhaps you can compile the temporary file and not do the renaming dance? 
    # That way less things could go wrong while compiling
    # maybe change the template syntax to be more versatile
    route_file = "./include/CommsServer.hpp"
    paths = []
    template_params = []
    template_contents = ""

    with open(route_file, "r") as file:
        template_contents = "".join(map(str,file.readlines()))
        paths, template_params = findFiles(template_contents)

    if not paths or not template_params:
        return

    env.Execute(f"mv {route_file} {route_file}.tmp")

    try:
        with open(route_file, "w") as file:

            for i in range(len(paths)):
                with open(paths[i], "r") as html_file:
                    processed_html = add_js_css("".join(map(str, html_file.readlines())))
                    template_contents = template_contents.replace(str(template_params[i]), 'PSTR(R"raw(' + processed_html + ')raw")', 1)
            file.write(template_contents)

    except Exception as e:
        env.Execute(f"mv {route_file}.tmp {route_file}")
        raise e

env.AddPreAction("$BUILD_DIR/src/main.cpp.o", preprocess)
env.AddPostAction("$BUILD_DIR/src/main.cpp.o", after_build) # change files back


