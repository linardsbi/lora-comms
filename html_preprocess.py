import re
Import("env")

# access to global build environment
#print(env)

# access to project build environment (is used source files in "src" folder)
#print(projenv)

def after_build(source, target, env):
    route_file = "./include/CommsServer.hpp"
    print("hi", source, target, env)
    #TODO: env.Execute(f"mv {route_file}.tmp {route_file}")

def findFiles(line):
    # FIXME: replace template parameters with HTML one by one, not all at once
    template_params = re.findall('\{".*"\}', line)
    base_path = "./src/views/"
    paths = []
    for name in template_params:
        paths.append(f"{base_path}{name[2:-2]}.html")

    #print(paths,template_params)
    return paths, template_params

def preprocess(source, target, env):
    # Perhaps you can compile the temporary file and not do the renaming dance?
    # maybe change the template syntax to be more versatile
    route_file = "./include/CommsServer.hpp"
    paths = []
    template_params = []
    template_contents = ""

    with open(route_file, "r") as file:
        template_contents = "".join(map(str,file.readlines()))
        paths, template_params = findFiles(template_contents)

    env.Execute(f"mv {route_file} {route_file}.tmp")

    with open(route_file, "w") as file:
        for i in range(len(paths)):
            with open(paths[i], "r") as html_file:
                #TODO: js and css preprocess with re.findall("(<script.*\/script>|<link.*stylesheet.*>)", line)
                template_contents = template_contents.replace(str(template_params[i]), 'PSTR(R"raw(' + "".join(map(str, html_file.readlines())) + ')raw")', 1)
        #print(template_contents, template_params)
        file.write(template_contents)


env.AddPostAction("buildprog", after_build) # change files back

env.AddPreAction("$BUILD_DIR/src/main.cpp.o", preprocess)

