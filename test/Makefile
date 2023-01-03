DIR	:= test

.PHONY: test
test:
	@docker-compose -f $(DIR)/docker-compose.yml up -d
	@sh $(DIR)/test.sh

.PHONY: down
down:
	@docker-compose -f $(DIR)/docker-compose.yml down

DOCKER_EXEC_CMD := /bin/sh
CONTAINER	:= $(word 2, $(MAKECMDGOALS))
exec:
	@docker-compose -f $(DIR)/docker-compose.yml exec $(CONTAINER) $(DOCKER_EXEC_CMD)

retest:
	docker-compose -f $(DIR)/docker-compose.yml restart $(CONTAINER)

logs:
	docker-compose -f $(DIR)/docker-compose.yml logs -f $(CONTAINER)