
.PHONY: es2
es2:
	$(MAKE) -C ./src es2

.PHONY: vk
vk:
	$(MAKE) -C ./src vk

.PHONY: win
win:
	$(MAKE) -C ./src win

.PHONY: clean
clean: clean_game clean_renderer clean_bin connectivity
	rm -f ./src/*.o

.PHONY: clean_game
clean_game:
	rm -f ./src/Game/*.o

.PHONY: clean_renderer
clean_renderer:
	rm -f ./src/renderers/*.o

.PHONY: connectivity
connectivity:
	rm -f ./src/Multiplayer/*.o

.PHONY: clean_bin
clean_bin:
	rm -f ./renderer