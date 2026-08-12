/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sboudcha <sboudcha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 03:09:30 by sboudcha          #+#    #+#             */
/*   Updated: 2026/08/07 02:02:01 by sboudcha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	swap_requests(t_request *a, t_request *b)
{
	t_request	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void	heap_push(t_heap *heap, t_request new_req)
{
	int	i;
	int	parent;

	// This is error for handling ?!
	if (heap->used >= heap->size)
		return ;
	heap->array[heap->used] = new_req;
	heap->used++;
	i = heap->used - 1;
	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (heap->array[parent].priority > heap->array[i].priority)
		{
			swap_requests(&heap->array[i], &heap->array[parent]);
			i = parent;
		}
		else
			break ;
	}
}
int	get_smallest(t_heap *heap, int parent)
{
	int	left;
	int	right;
	int	smallest;

	smallest = parent;
	left = 2 * parent + 1;
	if (heap->array[left].priority < heap->array[smallest].priority)
		smallest = left;
	right = 2 * parent + 2;
	if (right < heap->used
		&& heap->array[right].priority < heap->array[smallest].priority)
		smallest = right;
	return (smallest);
}

t_request	heap_pop(t_heap *heap)
{
	t_request	min;
	int			smallest;
	int			parent;

	// This is error for handling ?!
	if (heap->used == 0)
	{
		min.coder_id = -1;
		return (min);
	}
	min = heap->array[0];
	heap->array[0] = heap->array[heap->used - 1];
	heap->used--;
	parent = 0;
	while ((2 * parent + 1) < heap->used)
	{
		smallest = get_smallest(heap, parent);
		if (smallest == parent)
			break ;
		swap_requests(&heap->array[smallest], &heap->array[parent]);
		parent = smallest;
	}
	return (min);
}
