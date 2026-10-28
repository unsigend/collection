# collection - A high-performance C data structure library
# Copyright (C) 2026-2027 Yixiang Qiu
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

CUR_DIR := .
MODULES := $(patsubst %/Makefile,%,$(wildcard */Makefile))

.DEFAULT_GOAL := help
.PHONY: help bench-% clean

help:
	@echo "Usage: make bench-NAME"

bench-%:
	@if [ ! -f $*/Makefile ]; then \
		echo "unknown bench module: $*"; \
		exit 1; \
	fi
	@$(MAKE) -C $*

clean:
	@for m in $(MODULES); do $(MAKE) -C $$m clean; done
