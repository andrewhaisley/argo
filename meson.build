project('argo', 'cpp',
    default_options : [ 'cpp_std=c++11' ],
    version : '1.0.1',
    license : 'MIT'
)

sources = [
    'json.cpp',
    'utf8.cpp',
    'lexer.cpp',
    'reader.cpp',
    'stream_reader.cpp',
    'fd_reader.cpp',
    'file_reader.cpp',
    'writer.cpp',
    'stream_writer.cpp',
    'fd_writer.cpp',
    'file_writer.cpp',
    'token.cpp',
    'json_exception.cpp',
    'parser.cpp',
    'unparser.cpp',
    'json_io_exception.cpp',
    'json_parser_exception.cpp',
    'json_utf8_exception.cpp',
    'json_array_index_range_exception.cpp',
    'json_pointer_exception.cpp',
    'json_invalid_key_exception.cpp',
    'pointer.cpp'
]

argo_lib = static_library('argo', sources)

pkg_mod = import('pkgconfig')

pkg_mod.generate(argo_lib,
    version : '1.0.1',
    name : 'argo',
    filebase : 'argo',
    description : 'C++ JSON parser'
)

tests = executable('tests', 'json_test.cpp', link_with : argo_lib)

test('JSON tests', tests)

#doxygen configuration
doxygen = find_program('doxygen', required : false)

if doxygen.found()
    doc_conf = configuration_data()
    doc_conf.set('DOXYGEN_INPUT', meson.source_root())
    doc_conf.set('DOXYGEN_OUTPUT', meson.current_build_dir())

    doxyfile = configure_file(
        input : 'doxygen.conf',
        output : 'Doxyfile',
        configuration : doc_conf
    )

    custom_target('doc',
        build_by_default : false,
        command : [doxygen, doxyfile],
        output : ['html']
    )
endif
