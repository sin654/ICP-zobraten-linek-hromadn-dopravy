# Makefile
# Řešení ICP projekt, Zobrazeni linek hormadne dopravy a sledovani jejich pohybu
# Autor: Jan Doležel, FIT, <xdolez81@stud.fit.vutbr.cz>
# 2020

.PHONY: run clean pack doxygen

# vytvoří Makefile v /src pomocí qmake
#qmake -o ./src/Makefile ./src/icp.proj
hello:
	cd ./src && qmake -o Makefile icp.pro && make

run:
	cd ./src && ./icp

clean:
	cd ./src && make clean
	cd ./src && rm Makefile icp
	cd ./doc && rm -rf *
	
pack:
	zip -r xdolez81-xname02-100-0 *
doxygen:
	cd ./src && doxygen
