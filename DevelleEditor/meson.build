sources = []

subdir('src')

executable('develle_editor', 
			sources, 
			dependencies: dependencies + develle_dep, 
			include_directories: 'src',
			cpp_pch: '../Develle/src/dvpch.h')
