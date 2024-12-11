pack:
	zip -r xfeket01.zip \
	src/* \
	docs/* \
	Makefile \
	platform.ini \
	README.md \
	CMakeList.txt

idfpy:
	@if [ -d src ]; then mv src main; fi
	@if [ -f "main/CMakeLists.txt" ]; then rm -f main/CMakeLists.txt; fi
	echo "idf_component_register(SRCS \"main.c\" \"display_task.c\" \"driver.c\" \"udp_server.c\" \"wifi_task.c\" \
                    INCLUDE_DIRS \".\")" > main/CMakeLists.txt

pio:
	@if [ -d main ]; then mv main src; fi
	@if [ -f "src/CMakeLists.txt" ]; then rm -f src/CMakeLists.txt; fi