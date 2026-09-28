NAME := inception
COMPOSE := docker compose -f srcs/docker-compose.yml
DATA_DIRS := /home/rtektas/data/mariadb /home/rtektas/data/wordpress
SECRET_DIR := secrets
SECRET_FILES := $(SECRET_DIR)/db_password.txt $(SECRET_DIR)/db_root_password.txt \
                $(SECRET_DIR)/wp_admin_password.txt $(SECRET_DIR)/wp_user_password.txt

all: up

prepare:
	@mkdir -p $(DATA_DIRS) $(SECRET_DIR)
	@if [ ! -f srcs/.env ]; then cp srcs/.env.example srcs/.env; fi
	@for file in $(SECRET_FILES); do \
		if [ ! -s $$file ]; then \
			head -c 48 /dev/urandom | base64 | tr -d '\n' | head -c 32 > $$file; \
		fi; \
	done
	@chmod 600 $(SECRET_FILES)

up: prepare
	$(COMPOSE) up -d --build

down:
	$(COMPOSE) down

start:
	$(COMPOSE) start

stop:
	$(COMPOSE) stop

ps:
	$(COMPOSE) ps

logs:
	$(COMPOSE) logs -f

check:
	bash srcs/check.sh

clean:
	$(COMPOSE) down --remove-orphans

fclean: clean
	$(COMPOSE) down --volumes --rmi all --remove-orphans
	@sudo rm -rf $(DATA_DIRS)

re: fclean all

.PHONY: all prepare up down start stop ps logs check clean fclean re
