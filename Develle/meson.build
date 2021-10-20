sources = []

subdir('src')

develle_inc = include_directories('src')
develle = library('develle', sources, include_directories: develle_inc, dependencies: dependencies, cpp_pch: 'src/dvpch.h')
