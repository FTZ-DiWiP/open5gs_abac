libabac_sources = files('''
    context.c
    event.c

    pip-build.c
    pip-handler.c

    prp-build.c
    prp-handler.c

    nudm-build.c
    nudm-handler.c
    ue-sm.c

    sbi-path.c
    abac-sm.c

    init.c
'''.split())

libabac = static_library('abac',
                         sources : libabac_sources,
                         dependencies : [libcrypt_dep,
                                         libsbi_dep],
                         install : false)

libabac_dep = declare_dependency(
    link_with : libabac,
    dependencies : [libcrypt_dep,
                    libsbi_dep])

abac_sources = files('''
    app.c
    ../main.c
'''.split())

executable('open5gs-abacd',
           sources : abac_sources,
           c_args : '-DDEFAULT_CONFIG_FILENAME="@0@/abac.yaml"'.format(open5gs_sysconfdir),
           include_directories : srcinc,
           dependencies : libabac_dep,
           install_rpath : libdir,
           install : true)
