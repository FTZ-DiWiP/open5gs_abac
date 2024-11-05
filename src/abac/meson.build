libabac_sources = files('''
    context.c
    event.c

    nabac-handler.c

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

openssl_dep = dependency('openssl')

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

executable('create_certificate',
           sources: ['create_certificate.c'],
           dependencies: [openssl_dep],
           install: true)
