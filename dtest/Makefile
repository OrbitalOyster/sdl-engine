TEST_DIR := dtest
TEST_OUTPUT := output
DELIMITER := "********************************************************************************"

# All test units
TESTS := $(patsubst $(TEST_DIR)/%, %, $(wildcard $(TEST_DIR)/units/*.c))

test:
	@echo $(DELIMITER)
	@for t in $(TESTS); do \
		echo "Compiling" $$t; \
		EXTRA_UNITS=$$(cut $(TEST_DIR)/$$t -c 3- | head -1); \
		EXPECTED=$$(cut $(TEST_DIR)/$$t -c 3- | head -2 | tail -1); \
		echo "Extra files:" $$EXTRA_UNITS; \
		$(CC) $(CFLAGS) $$EXTRA_UNITS $(TEST_DIR)/dtest.c -DTEST_FILE=$$t \
		-o $(TEST_DIR)/$(TEST_OUTPUT) && ./$(TEST_DIR)/$(TEST_OUTPUT); \
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
		echo $(DELIMITER); \
	done

test-strict:
	@echo $(DELIMITER)
	@for t in $(TESTS); do \
		echo "Compiling" $$t; \
		EXTRA_UNITS=$$(cut $(TEST_DIR)/$$t -c 3- | head -1); \
		EXPECTED=$$(cut $(TEST_DIR)/$$t -c 3- | head -2 | tail -1); \
		echo "Extra files:" $$EXTRA_UNITS; \
		$(CC) $(CFLAGS) $$EXTRA_UNITS $(TEST_DIR)/dtest.c -DTEST_FILE=$$t -DTEST_STRICT\
		-o $(TEST_DIR)/$(TEST_OUTPUT); \
		COMPILE_RES=$$?; \
		if [ $$COMPILE_RES -ne 0 ]; then \
			echo "FAILED TO COMPILE: " $$COMPILE_RES; \
			exit $$COMPILE_RES; \
		fi; \
		./$(TEST_DIR)/$(TEST_OUTPUT); \
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
			exit $$RESULT; \
		fi; \
		echo $(DELIMITER); \
	done

#	-rm $(TEST_DIR)/$(OUTPUT)
