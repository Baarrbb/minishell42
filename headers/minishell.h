/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 00:10:02 by bsuc              #+#    #+#             */
/*   Updated: 2024/02/25 17:34:33 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <linux/limits.h>
# include <string.h>
# include <errno.h>
# include <sys/stat.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <assert.h>
# include "../libft/libft.h"
# include "struct.h"
# include "parsing.h"
# include "builtins.h"
# include "exec.h"

# define CYAN "\x1b[36m"
# define RESET "\x1b[0m"

#endif