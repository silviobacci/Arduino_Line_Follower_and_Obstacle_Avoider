CC = gcc

CFLAGS = -Wall --coverage
LFLAGS = -lcunit -lm --coverage

DIR_SRC = src
DIR_OBJ = obj
DIR_COV = res-coverage
DIR_UNIT = res-unit

DIR_COV_HTML = $(DIR_COV)/html

run: clean prepare compile link exe cov ret

clean:
	rm -f -r $(DIR_OBJ) $(DIR_UNIT) $(DIR_COV)
	mkdir $(DIR_OBJ) $(DIR_UNIT) $(DIR_COV)

prepare:
	cp /usr/local/Cellar/cunit/2.1-3/share/CUnit/*.xsl $(DIR_UNIT)

compile:
	$(CC) -o $(DIR_OBJ)/Cunit_Test.o -c $(DIR_SRC)/Cunit_Test.c $(CFLAGS)

link:
	$(CC) -o $(DIR_OBJ)/CTest $(DIR_OBJ)/Cunit_Test.o $(LFLAGS)

cov:
	lcov --capture --directory $(DIR_OBJ) --output-file $(DIR_COV)/coverage.info

ret:
	genhtml $(DIR_COV)/coverage.info --output-directory $(DIR_COV_HTML)

exe:
	./$(DIR_OBJ)/CTest
