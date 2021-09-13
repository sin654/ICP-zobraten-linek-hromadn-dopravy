Projekt ICP 2020


AUTOR: 
	- Jan Doležel, xdolez81@stud.fit.vutbr.cz
 	- "xname02" ve jménu souboru není jméno žádného studenta, ale bylo tam doplněno, aby název splňoval požadavky uvedené v zadání projektu
 	- tento projekt jsem implementoval samostatně z důvodu toho, že jsem si do začátku koronavirové pandemie nesehnal partnera


OVLÁDÁNÍ APLIKACE:
	- aplikace si hned po spuštění načte ze složky ./examples/ mapový podklad a seznam spojů a linek
	- v levém horním rohu se nachází aktuální čas simulace (výchozí čas simulace je 0:0:0)
	- ulice jsou na mapovém podkladu znázorněny červenou čarou a zastávky na nich modrým kroužkem
		* Při kliknutí na určitou ulici se její barva změní na zelenou, což značí, že ulice je označená.
	- zelené pohybující se kolečka znázorňují spoje na lince
		* Při najetí kurzorem na spoj je linka tohoto spoje zvýrazněna modrou tlustou čarou a v pravé straně aplikace se zobrazí itinerář spoje. Zastávky v itineráři spoje jsou popsány [jméno_zastávky - hodiny:minuty:sekundy]. Součástí itineráře je i jméno spoje [vehicle: jméno_spoje].

	OVLÁDACÍ TLAČÍTKA:
		* [Default Time] - nastavý výchozí čas simulace 0:0:0
		* [Default Speed] - nastaví výchozí rychlost simulace (normální rychlost hodin)
		* [Speed UP] - zrychlí simulaci 2x	(může být aplikováno několikrát)
		* [Slow DOWN] - zpomalí simulaci na 1/2 rychlost	(může být aplikováno několikrát)
		* [PAUSE/CONTINUE] - přeruší/ obnoví chod simulace
		* [Zoom In] - přiblíží mapu
		* [Zoom Out] - oddálí mapu
	SLIDER:
		* Onačené ulici mení rychlost (vlevo -> min, vpravo -> max(výchozí) rychlost)



SOUČÁST ODEVZDÁNÍ:
	- vytvořil jsem mapový podklad obsahující 21 ulic
	- vytvořil jsem 4 linky, kde každá z nich má 60 spojů	(poslední spoj vyrazí v 7:52:1 a poté se až to "nového dne" v 0:0:0 nic neděje)
		* linka 1 jezdí co minutu
		* linka 2 jezdí co 2 minuty
		* linka 3 jezdí co 4 minuty
		* linka 4 jezdí co 8 minut
	- zvolený formát bych JSON a tyto soubory jsou umístěné v ./examples/


DOKUMENACE:
	- "make doxygen" vytvoří ve složce ./doc složku ./doc/html/ kde se tato dokumentace nachází
	- Doxyfile jsem umístil do ./src


PŘEKLAD:
	- "make" vytvoří spustitelný program ve složce ./src/ ze které je následně pomocí "make run" spuštěn



NENAIMPLEMENTOVANÉ VLASTNOSTI:
	- možnost uzavření ulice a definování objízdné trasy
	- SLIDER na ovládání rychlosti ulice není s aktuální rychlostí na ulici propojen a tudíž při kliknutí na novou ulici není pozice slideru přenastavena (toto samozřejmě namá vliv na schopnost si rychlost na ulici nastavit) 

