/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 14:39:33 by marvin            #+#    #+#             */
/*   Updated: 2025/11/11 15:23:46 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* ========================== INCLUDES =========================== */
# include "libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>

/* ========================== ENUMS ============================= */
typedef enum e_redir_type
{
	R_INPUT,
	R_OUTPUT,
	R_APPEND,
	R_HEREDOC
}								t_redir_type;

/* ========================== STRUCTS =========================== */
typedef struct s_redir
{
	t_redir_type				type;
	char						*file;
	int							fd;
	struct s_redir				*next;
}								t_redir;
typedef struct s_token
{
	char						*value;
	int							type;
	struct s_token				*next;
}								t_token;
typedef struct s_command
{
	char						*next_deli;
	char						**argv;
	int							argc;
	int							size;
	char						*infile;
	char						*outfile;
	int							append;
	int							heredoc;
	char						*heredoc_delim;
	int							pipe_fd[2];
	int							type;
	struct s_command			*next;
}								t_command;
typedef struct s_expand_ctx
{
	int							interrupted;
	const char					*input;
	char						*out;
	int							*oi;
	char						**env;
	int							last_status;
	int							*i;
	int							j;
	int							type;
}								t_expand_ctx;
typedef struct s_cmd
{
	char						*value;
	struct s_cmd				*next;
}								t_cmd;
typedef struct s_minishell
{
	int							cmd;
	char						**envp;
	t_list						*token;
	t_list						*h_history;
	int							last_status;
}								t_minishell;
typedef struct s_exec_args
{
	t_command					*cmd;
	char						**paths;
	t_minishell					**ms;
	int							fd[2];
	int							prev_in;
}								t_exec_args;

/* ========================== BOOT ============================= */
void							bump_shlvl(t_minishell *minishell);

/* ========================== UTILS ============================= */
int								ft_strlen_tab(char **tab);
void							ft_strdup_tab(t_minishell **minishell,
									char **tab);
void							ft_free_tab(char **tab);
int								ft_strcmp(const char *s1, const char *s2);
int								count_cmds(const t_command *cmd);
void							ft_add_slash(char **paths);
char							**ft_find_path(char **envp);
void							setup_child_fds(int pipefd[2]);
int								ft_is_built(char **arg);

/* ========================== EXECUTION ========================= */
void							handle_exec(t_command *cmd, t_minishell **ms);
int								exec_pipeline(t_command *cmd, t_minishell **ms,
									char **paths, int n);
int								update_pipe(int fd[2], int i, int n);
int								handle_pipe(int fd[2], int i, int n,
									t_minishell **ms);
void							exec_one_child(t_exec_args args);
int								handle_cmd_not_found(t_command *cmd,
									t_minishell **ms);
void							fork_child(t_command *cmd, char **paths,
									t_minishell **ms, int pipefd[2]);
void							fork_child_with_bin(t_command *cmd,
									t_minishell **ms, char *bin);
char							*find_executable(t_command *cmd, char **paths);
void							exec_command(t_command *cmd, char *bin,
									t_minishell *ms);
void							ms_perror(const char *cmd, const char *msg);
int								run_builtin_with_redir(t_command *cmd,
									t_minishell **ms);
int								exec_only_redirections(t_command *cmd,
									t_minishell **ms);
int								exec_single(t_command *cmd, t_minishell **ms,
									char **paths);

/* ========================== REDIRECTIONS ====================== */
int								apply_redirections(t_command *cmd);
int								prepare_heredocs(t_command *cmd,
									t_minishell **ms);

/* ========================== LEXER / PARSER ==================== */
t_token							*lexer(char *prompt);
void							parse_token(t_token *toklist,
									t_minishell **minishell);
void							free_command(t_command *cmd);
int								get_type(const char *tok);
int								is_blank(char c);
int								char_len(const char *str);
int								skip_blanks(const char *s, int i);
void							skip_blanks_with_ptr(const char *s, int *i);
int								skip_quote(const char *s, int i);
int								skip_word(const char *s, int i);
void							parse_operator(const char *s, int *i);
void							parse_quoted_string(const char *s, int *i);
void							parse_word(const char *s, int *i);
char							*clean_token(const char *token);

/* ========================== TOKEN SPLITTING =================== */
char							**ft_split_tokens(char *s);
char							**ft_free_split(char **tab);
t_token							*build_list(char **tab);

/* ========================== COMMAND INIT ====================== */
t_command						*init_command(const char *word, int type);
int								guess_delim(t_command **cmd, t_token **cur,
									int type);
int								set_heredoc(t_command **cmd, t_token **cur);
int								add_arg(t_command *cmd, const char *val);

/* ========================== EXPANDER ========================== */
char							*expand(const char *input, char **env,
									int last_status, int type);
void							expand_args(char **argv, char **env,
									int last_status, int type);
char							*get_env_value(const char *name, char **env);
int								handle_dollar(const char *input,
									t_expand_ctx *ctx);
int								handle_dollar_var(const char *input,
									t_expand_ctx *ctx);
void							handle_quotes(const char *input,
									t_expand_ctx *ctx);

/* ========================== BUILTINS ========================== */
int								exec_builtin(char **args, char ***env);
int								ft_echo(char **args);
int								ft_cd(char **args, char ***env);
int								ft_pwd(char **args);
int								ft_exit(char **args);
int								ft_export(char **args, char ***env);
int								ft_unset(char **args, char ***env);
int								ft_env(char **args, char **env);
int								add_or_update_env(char *key, char *value,
									char ***env);
int								remove_env(char *key, char ***env);
int								cmp_env(const void *a, const void *b);
int								export_process_arg(char *arg, char ***env);
int								is_valid_identifier(char *key);
void							print_key_until_eq(char *str);
void							extract_key(char *arg, char *key, char *eq);
int								env_key_index(char **env, const char *key);
int								is_directory_path(const char *path);
char							*dup_direct_path(char *arg);
char							*search_in_paths(t_command *cmd, char **paths);

/* ========================== MEMORY ============================ */
void							free_token_list(t_token *head);
void							print_sorted_env_line(char *line);
int								build_new_value(char *old, char *val,
									char **out);
void							export_print_sorted_env(char **env);

/* ========================== SIGNALS / PROMPT ================= */
void							sig_handler(int sig);
char							*read_full_command(void);
extern volatile sig_atomic_t	g_signal;

#endif /* MINISHELL_H */