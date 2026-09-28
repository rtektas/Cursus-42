# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alephoen <alephoen@42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/04/02 18:18:33 by alephoen          #+#    #+#              #
#    Updated: 2026/06/29 12:30:59 by isadbaib         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Colors
GREEN		:= \033[0;32m
RED			:= \033[0;31m
YELLOW		:= \033[0;33m
BLUE		:= \033[0;34m
CYAN		:= \033[0;36m
MAGENTA		:= \033[0;35m
BOLD		:= \033[1m
RESET		:= \033[0m

# Icons
ROCKET_ICON		:= 🚀
COMPILE_ICON	:= 🛠️
LINK_ICON		:= 📦
CLEAN_ICON		:= 🧹
SUCCESS_ICON	:= ✅
ERROR_ICON		:= ❌

NAME	:= webserv

CC		:= c++
CFLAGS	:= -g3 -O0 -Werror -Wall -Wextra -Wno-unused-function -std=c++98 

CFILES	:= \
main.cpp \
a_config_parser/ConfParser.cpp \
a_config_parser/LocationConf.cpp \
a_config_parser/RouteConf.cpp \
a_config_parser/ServerConf.cpp \
a_config_parser/ServersConf.cpp \
a_config_parser/Validation.cpp \
b_request/HTTParser.cpp \
b_request/check_req_header_state.cpp \
b_request/parse_header_val.cpp \
b_request/process_body.cpp \
b_request/process_headers.cpp \
b_request/process_req_line.cpp \
c_response/ResponseBuilder.cpp \
c_response/build_autoindex.cpp \
c_response/build_delete.cpp \
c_response/build_error.cpp \
c_response/PageError.cpp \
c_response/build_static.cpp \
c_response/build_upload.cpp \
c_response/parse_CT_response.cpp \
c_response/build_cgi.cpp \
d_client/Client.cpp \
e_server/ConnectionManager.cpp \
e_server/Server.cpp \
g_utils/Convertion.cpp \
g_utils/Logs.cpp \
g_utils/ReadFile.cpp \
g_utils/Signals.cpp \
g_utils/utils.cpp \
f_cgi/cgi_env.cpp \
f_cgi/CGIHandler.cpp \
f_cgi/cgi_utils.cpp \
f_cgi/on_stdin_ready.cpp \
f_cgi/on_stdut_ready.cpp \
f_cgi/parse_interpreter.cpp


SRCS_PATH := srcs
OBJS_PATH := objs

TEST_PATH := testPython

SRCS := $(addprefix $(SRCS_PATH)/,$(CFILES))
OBJS := $(patsubst $(SRCS_PATH)/%.cpp,$(OBJS_PATH)/%.o,$(SRCS))

INCS_PATH	:= incs
INCS		:= -I$(INCS_PATH)

# Auto-discovered test scripts: any .py/.sh file dropped into testPython/
# is picked up automatically by `make test` without touching this Makefile.
# - cgi_*.py and slowloris.py are handled by their own dedicated targets
#   (test-cgi, test-slowloris) and excluded here.
PY_TESTS := $(filter-out $(TEST_PATH)/cgi_%.py $(TEST_PATH)/slowloris.py,$(wildcard $(TEST_PATH)/*.py))
SH_TESTS := $(wildcard $(TEST_PATH)/*.sh)

.PHONY: all clean fclean re objs_dir start_msg gen_ctags test test-banner test-scripts test-cgi test-slowloris test-footer


#.PHONY: all clean fclean re objs_dir start_msg gen_ctags

all: objs_dir start_msg gen_ctags $(NAME)

print-debug:
	@echo -e $(SRCS)
	@echo -e $(OBJS)

objs_dir:
	@mkdir -p $(OBJS_PATH)
	@mkdir -p $(OBJS_PATH)/a_config_parser
	@mkdir -p $(OBJS_PATH)/b_request
	@mkdir -p $(OBJS_PATH)/c_response
	@mkdir -p $(OBJS_PATH)/d_client
	@mkdir -p $(OBJS_PATH)/e_server
	@mkdir -p $(OBJS_PATH)/f_cgi
	@mkdir -p $(OBJS_PATH)/g_utils

start_msg:
	@echo -e "$(ROCKET_ICON) $(BOLD)Starting build $(RESET) $(MAGENTA)$(NAME)$(RESET)..."

$(NAME): $(OBJS)
	@echo -e "$(LINK_ICON) $(BOLD)Linking$(RESET) $(GREEN)$(NAME)$(RESET)"
	@$(CC) $(CFLAGS) $(INCS) $(OBJS) -o $(NAME)
	@echo -e "$(SUCCESS_ICON) $(GREEN)Build successful:$(RESET) $(BOLD)$(NAME)$(RESET)"

$(OBJS_PATH)/%.o: $(SRCS_PATH)/%.cpp
	@echo -e "$(COMPILE_ICON) $(CYAN)Compiling$(RESET) $(YELLOW)$<$(RESET) → $(GREEN)$@$(RESET)"
	@$(CC) $(CFLAGS) $(INCS) -c $< -o $@ || (echo -e "$(ERROR_ICON) $(RED)Compile failed: $<$(RESET)"; exit 1)

clean:
	@echo -e "$(CLEAN_ICON) $(BLUE)Cleaning object files Minishell...$(RESET)"
	@rm -f $(OBJS)

fclean: clean
	@echo -e "$(CLEAN_ICON) $(BLUE)Removing executable:$(RESET) $(RED)$(NAME)$(RESET)"
	@rm -rf objs
	@rm -f tags .tags
	@rm -f $(NAME)

re: fclean all

gen_ctags:
#	@echo -e "$(ROCKET_ICON) $(BOLD)Generating $(RESET) $(MAGENTA)ctags$(RESET)..."
#	@ctags -R \
#	--languages=C,C++ \
#	--kinds-C=+fpd \
#	--kinds-C++=+fp \
#	--fields=+nS \
#	--extras=+q \
#	--tag-relative=yes \
#	-f tags .
#	@cp ./tags ./.tags

# Runs every test script from testPython/ against a server already
# running on 127.0.0.1:8080 (e.g. ./webserv conf/testfinal.conf &).
# test-slowloris runs last: it is expected to crash the server (see
# rapportAlex.txt, CRASH #2), so it must restart it before continuing.
test: test-banner test-scripts test-cgi test-slowloris test-footer

test-banner:
	@echo -e "$(CYAN)$(BOLD)"
	@echo -e "  ╔════════════════════════════════════════════════════╗"
	@echo -e "  ║   🛰️   W E B S E R V   ::  T E S T   S U I T E      ║"
	@echo -e "  ║   ⚡ load · stress · cgi edge-cases · slowloris    ║"
	@echo -e "  ╚════════════════════════════════════════════════════╝"
	@echo -e "$(RESET)"
	@echo -e "  $(YELLOW)target: http://localhost:8080  (start with: ./webserv conf/testfinal.conf &)$(RESET)\n"

test-scripts:
	@echo -e "$(MAGENTA)$(BOLD)  ▶ [1/3] SCRIPTS$(RESET) $(ROCKET_ICON) auto-discovered .py/.sh tests in $(TEST_PATH)/"
	@for f in $(PY_TESTS); do \
		echo -e "$(CYAN)    -- $$f --$(RESET)"; \
		python3 $$f; \
		echo ""; \
	done
	@for f in $(SH_TESTS); do \
		echo -e "$(CYAN)    -- $$f --$(RESET)"; \
		bash $$f; \
		echo ""; \
	done

test-cgi:
	@echo -e "$(MAGENTA)$(BOLD)  ▶ [2/3] CGI EDGE-CASES$(RESET) $(ROCKET_ICON) infinite loop + 5MB output"
	@cp $(TEST_PATH)/cgi_test_loop.py www/main/cgi-bin/_test_loop.py
	@cp $(TEST_PATH)/cgi_test_bigout.py www/main/cgi-bin/_test_bigout.py
	@chmod +x www/main/cgi-bin/_test_loop.py www/main/cgi-bin/_test_bigout.py
	@echo "    --- infinite loop CGI (8s timeout) ---"
	@curl -s -o /dev/null -w "    code=%{http_code} time=%{time_total}\n" --max-time 8 http://localhost:8080/cgi-bin/_test_loop.py || true
	@echo "    --- big output CGI (5MB) ---"
	@curl -s -o /dev/null -w "    code=%{http_code} size=%{size_download}\n" http://localhost:8080/cgi-bin/_test_bigout.py || true
	@rm -f www/main/cgi-bin/_test_loop.py www/main/cgi-bin/_test_bigout.py
	@echo ""

test-slowloris:
	@echo -e "$(MAGENTA)$(BOLD)  ▶ [3/3] SLOWLORIS$(RESET)  $(RED)(WARNING: known to crash the server, see rapportAlex.txt CRASH #2)$(RESET)"
	@python3 $(TEST_PATH)/slowloris.py 100 10
	@echo ""

test-footer:
	@echo -e "$(GREEN)$(BOLD)"
	@echo -e "  ╔═════════════════════════════════════════════════════════════╗"
	@echo -e "  ║   $(SUCCESS_ICON) TEST SUITE COMPLETE — check output above  ║"
	@echo -e "  ╚═════════════════════════════════════════════════════════════╝"
	@echo -e "$(RESET)"

