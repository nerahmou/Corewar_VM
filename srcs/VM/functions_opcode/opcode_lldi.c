/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   opcode_lldi.c                                    .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: fpupier <fpupier@student.le-101.fr>        +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/12/07 09:55:47 by fpupier      #+#   ##    ##    #+#       */
/*   Updated: 2018/12/07 09:55:47 by fpupier     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../includes/corewar.h"

static int		check_lldi(t_var *data, unsigned int pc, int dir_oct)
{
	unsigned char	p[3];
	unsigned char	test;
	size_t 			i;

	i = 0;
	test = '\0';
	p[0] = data->vm[((pc + 1) % MEM_SIZE)] >> 6;
	p[1] = (unsigned char)(0x3 & (data->vm[((pc + 1) % MEM_SIZE)] >> 4));
	p[2] = (unsigned char)(0x3 & (data->vm[((pc + 1) % MEM_SIZE)] >> 2));
	data->op_size += 1;
	while (i < 3)
	{
		if (p[i] == REG_CODE)
			data->op_size += 1;
		else if (p[i] == DIR_CODE)
			dir_oct == 2 ? (data->op_size += 2) : (data->op_size += 4);
		else if (p[i] == IND_CODE)
			data->op_size += 2;
		i++;
	}
	if (p[1] == REG_CODE)
		test = p[1];
	else if (p[1] == DIR_CODE)
		test = p[1];
	if (!(p[0] & 0x3) || !(test) || (p[2] != REG_CODE))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int				opcode_lldi(t_var *data, t_process *p_process)
{
	unsigned int	tmp_adr;

	if (!check_lldi(data, p_process->pc, 2) && !ft_params_opcode(data, p_process, 2, 0))
	{
		tmp_adr = (data->t_params[0][0] + data->t_params[0][1]);
		p_process->registre[data->t_params[1][2]].val =
				((data->vm[(p_process->pc + tmp_adr) % MEM_SIZE] << 24)
				 +  (data->vm[(p_process->pc + tmp_adr + 1) % MEM_SIZE] << 16)
				 + (data->vm[(p_process->pc + tmp_adr + 2) % MEM_SIZE] << 8)
				 + (data->vm[(p_process->pc + tmp_adr + 3) % MEM_SIZE]));
		if (data->v == 4 || data->v == 6)
		{
			ft_printf("P %4i | lldi %i %i r%i\n", p_process->id,
					  data->t_params[0][0], data->t_params[0][1],
					  data->t_params[1][2]);
			ft_printf("       | -> load from %i + %i = %i (with pc %i)\n",
					  data->t_params[0][0], data->t_params[0][1],
					  (data->t_params[0][0] + data->t_params[0][1]),
					  (p_process->pc + tmp_adr));
		}
		if (p_process->registre[data->t_params[1][2]].val == 0)
			p_process->carry = 1;
		else
			p_process->carry = 0;
		p_process->pc = ((p_process->pc + data->op_size) % MEM_SIZE);
		return (EXIT_SUCCESS);
	}
	p_process->pc = ((p_process->pc + data->op_size) % MEM_SIZE);
	return (EXIT_FAILURE);
}