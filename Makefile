.PHONY: docs format-examples cloc build-examples build-boards update-ver spaces clean check

docs:
	doxygen extras/doxygen.config

format-examples:
	find examples extras/tests -name '*.ino' -exec astyle --options=extras/formatter.conf {} \;

cloc:
	cloc ./

build-examples:
	python3 extras/build-examples.py

build-boards:
	python3 extras/build-boards.py

spaces:
	find ./src ./linux -name '*.cpp' -o -name '*.h' -exec sed -i 's/\t/    /g' {} \;
	find ./examples ./extras/tests -name '*.ino' -o -name '*.h' -exec sed -i 's/\t/  /g' {} \;

update-ver:
	sed -i 's/"version": "[0-9\.]*"/"version": "$(VER)"/g' library.json
	sed -i 's/version=[0-9\.]*/version=$(VER)/g' library.properties
	sed -i 's/BLYNK_VERSION        "[0-9\.]*"/BLYNK_VERSION        "$(VER)"/g' src/Blynk/BlynkConfig.h

clean:
	-find . -type f -name "*.orig" -exec rm -f {} \;

check: spaces format-examples
	@echo '******************************************'
	@echo ' Files with no "YourAuthToken":'
	@echo '******************************************'
	@find ./examples -name "*.ino" -exec grep -L "YourAuthToken" {} \;
