#include "../includes/minishell.h"

//export key=value
//export key
//export key=value key2=value2
//         equal_address = ft_strchr(args[1],'=');
        // if (!equal_address || equal_address = args[i])
            // return;//print corresponding error (bash: export: `=': not a valid identifier) $? = 1

int     already_exist(char *key, t_env **env)
{
    t_env *tmp;

    tmp = *env;
    while (tmp)
    {
        if (!ft_strcmp(tmp->key,key))
            return (0);
        tmp = tmp->next;
    }
    return (1);
}

void    add_var(char *key, char *value, t_env **env)
{
    t_env *tmp;
    t_env *new_var;

    new_var = malloc(sizeof(t_env));
    new_var->key = key;
    new_var->value = value;
    new_var->next = NULL;
    if (!*env)
        *env = new_var;
    else
    {
        tmp = *env;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = new_var;
    }
}

void    modify_var(char *key, char *value, t_env **env)
{
    t_env *tmp;

    tmp = *env;
    while (tmp)
    {
        if (!ft_strcmp(tmp->key,key))
        {
            free(tmp->value);
            tmp->value = value;
            break;
        }
        tmp = tmp->next;
    }
}
//export with no argument:\
declare -x Key=value

//export var (with no '=') -> Variable is exported. not added to env list.\
but when export with no args is used "declare -x var" is printed
void	export(char **args, t_env **env, int child)
{
    int i;
    int j;
    char *key;
    char *value;

    i = -1;
    if (args == NULL || !*env && child)
        exit(EXIT_FAILURE);
    while (args[++i])
    {
        j = 0;
        while (args[i][j])
        {
            if (args[i][j] == '=')
            {
                if (j == 0)//err
                {
                    ft_putstr_fd("minishell: ", 2);
		        	ft_putstr_fd(args[i], 2);
        			ft_putstr_fd(": not a valid identifier\n", 2);//set exit status to 1
                }
                key = ft_substr(args[i],0,j); // free
                value = ft_substr(args[i],j + 1, ft_strlen(args[i])); // free
                if (!already_exist(key,env))
                    modify_var(key,value,env);
                else
                    add_var(key,value,env);
            }
            j++;
        }

    }
    if (child)
	    exit(EXIT_SUCCESS);
	// printf("export\n");
}
