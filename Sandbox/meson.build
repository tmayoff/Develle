sources = []

subdir('src')

executable('sandbox', 
			sources, 
			dependencies: dependencies, 
			include_directories: ['src', develle_inc],
			link_with: develle,
			cpp_pch: '../Develle/src/dvpch.h')
