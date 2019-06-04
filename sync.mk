RSYNC = rsync
RSYNC_OPTIONS = -va --human-readable --progress
RSYNC_SRC = ./
RSYNC_DEST = pi@raspberry:serverstream
RSYNC_INCLUDE = headers/ src/ tests/ makefile tests.mk sync.mk
RSYNC_EXCLUDE = sync-exclude.rsync

.PHONY: sync
sync:
	$(RSYNC) $(RSYNC_OPTIONS) $(RSYNC_SRC) $(RSYNC_DEST) --exclude-from=$(RSYNC_EXCLUDE)
