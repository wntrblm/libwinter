#!/usr/bin/env python3

import argparse
import pathlib

from wintertools import buildgen
from wintertools.third_party import ninja_syntax

# Check the python version before doing anything else.
buildgen.check_python_version()

# Make sure we're in the right directory.
buildgen.ensure_directory()

# Gemini/test-specific sources, includes, and defines.

PROGRAM = "test"

SRCS = [
    "tests/**/*.c",
    "src/wntr_assert.c",
    "src/wntr_bezier.c",
    "src/wntr_error_correction.c",
    "src/wntr_midi_core.c",
    "src/wntr_sysex_dispatcher.c",
    "third_party/libfixmath/fix16_str.c",
    "third_party/libfixmath/fix16.c",
    "third_party/munit/munit.c",
]

INCLUDES = [
    "src/samd",
    "tests/stubs",
]

DEFINES = buildgen.Desktop.defines()

DEFINES.update(
    dict(
        DEBUG=1,
        __SAMD21G18A__=1,
    )
)


# Toolchain configuration. Wintertools does most of the work here.

# Switch to clang since buildgen defaults to ARM gcc.
buildgen.GCC = "clang"

COMMON_FLAGS = buildgen.Desktop.common_flags()

COMPILE_FLAGS = buildgen.Desktop.cc_flags()

COMPILE_FLAGS += [
    "-ggdb3 -Og",
]

LINK_FLAGS = buildgen.Desktop.ld_flags()


# Buildfile generation


def generate_build():
    srcs = buildgen.expand_srcs(SRCS)
    INCLUDES.extend(buildgen.includes_from_srcs(srcs))

    compiler_flags = COMMON_FLAGS + COMPILE_FLAGS
    linker_flags = COMMON_FLAGS + LINK_FLAGS

    buildfile_path = pathlib.Path("./build.ninja")
    buildfile = buildfile_path.open("w")
    writer = ninja_syntax.Writer(buildfile)

    # Global variables

    writer.comment("This is generated by configure.py- don't edit it directly!")
    writer.newline()

    buildgen.toolchain_variables(
        writer,
        cc_flags=compiler_flags,
        linker_flags=linker_flags,
        includes=INCLUDES,
        defines=DEFINES,
    )

    # Use wintertools' common rules for compiling and such.
    buildgen.common_rules(writer)

    # Builds for compiling, linking, and outputting the program
    objects = buildgen.compile_build(writer, srcs)
    buildgen.link_build(writer, PROGRAM, objects, ext="")

    # Builds for generated files

    # Formatting and linting
    format_files = list(pathlib.Path(".").glob("tests/**/*.[c,h]"))
    buildgen.clang_format_build(writer, format_files)

    # Special reconfigure build
    buildgen.reconfigure_build(writer)

    # All done. :)
    writer.close()


def main():
    parser = argparse.ArgumentParser(
        formatter_class=argparse.ArgumentDefaultsHelpFormatter
    )

    args = parser.parse_args()

    generate_build()

    print("Created build.ninja")


if __name__ == "__main__":
    main()
