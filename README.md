# minishell
This project is about creating a simple shell.

The shell should:

• Display a prompt when waiting for a new command.

• Have a working history.

• Search and launch the right executable.

• Handle ’ (single quotes) and " (double quotes).

• Implement redirections (>, >>, <, <<).

• Implement pipes (| character).

• Handle environment variables ($VAR).

• Handle $? which should expand to the exit status of the most recently executed
foreground pipeline.

• Handle ctrl-C, ctrl-D and ctrl-\ which should behave like in bash.

• The shell implements the following builtins:\
◦ echo with option -n\
◦ cd with only a relative or absolute path\
◦ pwd with no options\
◦ export with no options\
◦ unset with no options\
◦ env with no options or arguments\
◦ exit with no options

<a href="https://www.google.com/" target="_blank">bash</a>  is the reference.
