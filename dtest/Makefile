TEST_DIR := dtest

# All test units
TESTS := $(patsubst $(TEST_DIR)/%, %, $(wildcard $(TEST_DIR)/units/*.c))

test:
	@echo "******************************************"
	@for t in $(TESTS); do \
		echo "Compiling" $$t; \
		EXTRA_UNITS=$$(cut $(TEST_DIR)/$$t -c 3- | head -1); \
		EXPECTED=$$(cut $(TEST_DIR)/$$t -c 3- | head -2 | tail -1); \
		echo "Extra files:" $$EXTRA_UNITS; \
		$(CC) $(CFLAGS) $$EXTRA_UNITS $(TEST_DIR)/dtest.c -DTEST_FILE=$$t \
		-o $(TEST_DIR)/unit && ./$(TEST_DIR)/unit; \
		RESULT=$$?; \
		echo -n "Expected:$$EXPECTED; Result: $$RESULT | "; \
		if [ $$EXPECTED -eq $$RESULT ]; then \
			echo "OK"; \
		else \
			if [ $$RESULT -eq 139 ]; then \
				echo "SEGMENTATION FAULT"; \
			else \
				echo "FAIL"; \
			fi; \
		fi; \
		echo "******************************************"; \
	done
#	-rm $(TEST_DIR)/unit
