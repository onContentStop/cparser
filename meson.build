project(
    'cparser',
    'cpp',
    version : '0.1.0',
    default_options : ['cpp_std=c++20', 'warning_level=3']
)

magic_enum_proj = subproject('magic_enum')
magic_enum_dep = magic_enum_proj.get_variable('magic_enum_dep')

fmt_proj = subproject('fmt')
fmt_dep = fmt_proj.get_variable('fmt_dep')

rang_proj = subproject('rang')
rang_dep = rang_proj.get_variable('rang_dep')

conf_data = configuration_data()
if get_option('maiden_mode')
    conf_data.set('maiden_mode', 'true')
else
    conf_data.set('maiden_mode', 'false')
endif
configure_file(
    input : 'config.hpp.cin',
    output : 'config.hpp',
    configuration : conf_data,
)
configuration_inc = include_directories('.')

subdir('cparser')
subdir('driver')
