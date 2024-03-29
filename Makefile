STEAMRT_URL ?= https://repo.steampowered.com/steamrt-images-scout/snapshots
STEAMRT_FILE ?= steam-runtime.tar.xz
STEAMRT_VERSION ?= 0.20211102.0

.PHONY: download-steam-runtime
download-steam-runtime:
	-wget -nc -q -O "./$(STEAMRT_FILE)" "$(STEAMRT_URL)/$(STEAMRT_VERSION)/$(STEAMRT_FILE)"

.PHONY: run-steam
run-steam: download-steam-runtime
	@tar --skip-old-files -xf $(STEAMRT_FILE)
	@./steam-runtime/setup.sh
	@./steam-runtime/run.sh ./cmake-build-debug/zwei