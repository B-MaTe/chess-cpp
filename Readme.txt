Balázs Máté - EY3292 - Sakk Program

A sakk programot C++-ban írtam meg az SFML library segítségével. https://www.sfml-dev.org/
Ez a library csak a megjelenítésben vesz részt, a logika el lett különítve a csomagtól.

A projekt megtalálható github-on is: https://github.com/B-MaTe/chess-cpp


Előkészületek:
	Amennyiben lehetséges, linuxon vagy WSL-en ajánlanám a program futtatását, azt tudtam tesztelni, mivel ubuntu rendszerem van.
	Emellett windowson nehézkes a vcpkg-t használni, de több helyen is láttam ezt a megoldást működni, amit leírtam.

	Telepíteni kell az SFML libraryt.

	Linux:
		sudo apt-get install libsfml-dev
	Windows:
		https://en.sfml-dev.org/forums/index.php?topic=27825.0 utolsó bejegyzés
		Ez alapján:
			git clone https://github.com/microsoft/vcpkg
			cd .\vcpkg\
			.\bootstrap-vcpkg.bat
			.\vcpkg.exe install sfml --triplet x64-windows-static

Megoldás fordítása:
	Ez a projekt CMakeLists.txt segítségével lett létrehozva, így a fordításhoz ezt kell használni.
	Ez létrefog hozni egy build foldert, nekem cmake-build-debug néven hozta létre.
	A futtatáshoz erre lesz szükség.

Megoldás futtatása:
	A futtatáshoz az előbb említett cmake-build-debug kibuildelt mappában kell futtatni a chess futtatható file-t. A program két command line arugment-et fogad el, egyrészt egy FEN-t, avagy
	Forsyth–Edwards Notation-t.  (Én ezt az oldalt használtam ilyenek generálásához: http://www.netreal.de/Forsyth-Edwards-Notation/index.php)
	A program ennek megfelelően fogja felrakni a táblát. A FEN végén második bemenetként meglehet még adni a következő lépést elvégző felet. w/W -> Világos, b/B -> Sötét. (Apalból fehér kezd.)
	 példa futtatás linux:
	 	cd cmake-build-debug
	 	./chess 4kb1r/1p2pppp/1np2qn1/p7/2PP3P/1PN2PN1/PB3PB1/3RR1K1 B

Megoldás használata:
	A program elindítása után megjelenik a kezdőképernyő, ez egy FULLSCREEN játék.
	A programból bármikor ki lehet lépni az ESC billentyűvel.
	A játék az En passant kivételével teljes mértékben a sakk szabályai alapján működik.
	Sakk esetén az ellenfél király mezeje beszíneződik.
	Matt/Patt esetén a program kilép és kiírja a konzolba a játék végeredményét.