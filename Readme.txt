Balázs Máté - EY3292 - Sakk Program

A sakk programot C++-ban írtam meg az SFML library segítségével. https://www.sfml-dev.org/
Ez a library csak a megjelenítésben vesz részt, a logika el lett különítve a csomagtól.


Előkészületek:
	Amennyiben lehetséges, linuxon ajánlanám a program futtatását, az egyszerűség miatt.
	Telepíteni kell az SFML libraryt.
	Linux:
		sudo apt-get install libsfml-dev
	Windows:

Megoldás fordítása:

	Ez a projekt CMakeLists.txt segítségével lett létrehozva, így a fordításhoz ezt kell használni.
	Ez létrefog hozni egy build foldert, nekem `cmake-build-debug` néven hozta létre.
	A futtatáshoz erre lesz szükség.

Megoldás futtatása:
	A futtatáshoz az előbb említett `cmake-build-debug` kibuildelt mappában kell futtatni a `chess` futtatható file-t. A program két command line arugment-et fogad el, egyrészt egy FEN-t, avagy
	Forsyth–Edwards Notation-t.  (Én ezt az oldalt használtam ilyenek generálásához: http://www.netreal.de/Forsyth-Edwards-Notation/index.php)
	A program ennek megfelelően fogja felrakni a táblát. A FEN végén második bemenetként meglehet még adni a következő lépést elvégző felet. w/W -> Világos, b/B -> Sötét. (Apalból fehér kezd.)
	 példa futtatás linux: 
	 	cd cmake-build-debug
	 	./chess 4kb1r/1p2pppp/1np2qn1/p7/2PP3P/1PN2PN1/PB3PB1/3RR1K1 B

Megoldás használata:
	A program elindítása után megjelenik a kezdőképernyő, ez egy FULLSCREEN játék.
	A programból bármikor ki lehet lépni az `ESC` billentyűvel. 
	A játék az En passant kivételével teljes mértékben a sakk szabályai alapján működik.
	Sakk esetén az ellenfél király mezeje beszíneződik.
	Matt/Patt esetén a program kilép és kiírja a konzolba a játék végeredményét.

