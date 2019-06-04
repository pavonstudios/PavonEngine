
DEFINES_WINLIB:= 
INLCUDE_WINLIB :=
export DEFINES_WINLIB
export INLCUDE_WINLIB
#es2: DEFINES += -DGLX

.PHONY: es2
es2: DEFINES_WINLIB += -DX11 -DEGL
es2:
	$(MAKE) -C ./src es2

.PHONY: es2glx
es2glx: DEFINES_WINLIB += -DX11 -DGLX
es2glx:
	$(MAKE) -C ./src es2

.PHONY: es2wayland
es2wayland: INLCUDE_WINLIB += -lwayland-client ./WindowManager/xdg-shell-protocol.o -ldl -lEGL
es2wayland: DEFINES_WINLIB += -DWAYLAND -DEGL
es2wayland:
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