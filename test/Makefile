DIR	:= test

.PHONY: test
test:
	@docker-compose -f $(DIR)/docker-compose.yml up -d
	@sh $(DIR)/test.sh

.PHONY: down
down:
	@docker-compose -f $(DIR)/docker-compose.yml down
