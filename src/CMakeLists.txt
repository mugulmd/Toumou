if(BUILD_TOUMOU_ENGINE)
	add_subdirectory(toumou)
endif()


if(BUILD_EXAMPLE_APP)
	add_subdirectory(apps)
endif()


if(BUILD_PYTHON_BINDINGS)
	add_subdirectory(bindings)
endif()
