env = Environment()

# common compiler flags and include directories
env["CPPFLAGS"] = ["-O2", "-Wall"]
env["CPPPATH"] = ["#include"]

# the main object containing the entry point of the application
env["OBJ_MAIN"] = None
# common object files not associated to any codec
env["OBJ_COMMON"] = []
# objects associated to the different codecs
env["CODECS"] = {}

SConscript(dirs = ["src", "test"], exports = ["env"])

aplay = env.Program(
    "aplay",
    [env["OBJ_MAIN"]] + env["OBJ_COMMON"] + [env["CODECS"][c]["objects"] for c in env["CODECS"]]
)

# by default build the application
Default(aplay)
