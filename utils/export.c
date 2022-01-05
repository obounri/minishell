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
    while (tmp->next)
    {
        if (!ft_strcmp(tmp->key,key))
            return (0);
        tmp = tmp->next;
    }
    return (1);
}

void    add_var(char *key, char *value, t_env *env)
{
    t_env *tmp;

    tmp = env;
    while (tmp)
        tmp = tmp->next;
    tmp->key = key;
    tmp->value = value;
    tmp->next = NULL;
}

void    modify_var(char *key, char *value, t_env **env)
{
    t_env *tmp;

    tmp = *env;
    while (tmp)
    {
        if (!ft_strcmp(tmp->key,key))
        {
            printf("old value : %s\n",tmp->value);
            free(tmp->value);
            tmp->value = value;
            printf("new value : %s\n",tmp->value);
            break;
        }
        tmp = tmp->next;
    }
}

void	export(char **args, t_env **env)
{
    int i;
    int j;
    char *key;
    char *value;

    i = -1;
    if (args == NULL)
        exit(EXIT_FAILURE);
    while (args[++i])
    {
        j = 0;
        while (args[i][j])
        {
            if (args[i][j] == '=')
            {
                if (j == 0 || args[i][j + 1] == '\0')//err
                    printf("ERROR");
                key = ft_substr(args[i],0,j); // free
                value = ft_substr(args[i],j + 1, ft_strlen(args[i])); // free
                printf("KEY %s VALUE %s\n",key,value);
                if (!already_exist(key,env))
                    modify_var(key,value,env);
                // else
                //     add_var(key,value,env);
            }
            j++;
        }
    }
	printf("export\n");
	exit(EXIT_SUCCESS);
}
