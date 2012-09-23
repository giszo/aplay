env = Environment()
env.Program(
    "aplay",
    source = [Glob("src/*.cc"), Glob("src/codec/mp3/*.cc")],
    CPPFLAGS = ["-O2", "-Wall"],
    CPPPATH = ["include"]
)
