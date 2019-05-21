
.PHONY: es2
es2:
	$(MAKE) -C ./src es2

.PHONY: win
win:
	$(MAKE) -C ./src win

.PHONY: clean
clean: clean_game
	rm -f ./src/*.o

.PHONY: clean_game
clean_game:
	rm -f ./src/Game/*.o