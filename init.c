/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sboudcha <sboudcha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/04 17:12:48 by sboudcha          #+#    #+#             */
/*   Updated: 2026/08/06 00:38:23 by sboudcha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int init_heap(t_data *data){
    data->heap.size = data->args.number_of_coders;
    data->heap.used = 0;
    data->heap.array = malloc(sizeof(t_request) * data->args.number_of_coders);
    
    // This is error for handling ?!
    if (!data->heap.array)
        return 1;

    return 0;
}

int init_dongles(t_data *data){
    int i = 0;

    data->dongles = malloc(sizeof(t_dongle) * data->args.number_of_coders);
    
    // This is error for handling ?!
    if (!data->dongles)
        return 1;
    
    while (i < data->args.number_of_coders)
    {
        pthread_mutex_init(&data->dongles[i].mutex, NULL);
        pthread_cond_init(&data->dongles[i].cond, NULL);
        i++;
    }
    return 0;
}

int init_coders(t_data *data){
    int i = 0;

    data->coders = malloc(sizeof(t_coder) * data->args.number_of_coders);
    
    // This is error for handling ?!----------------------------
    if (!data->coders)
        return 1;

    while (i < data->args.number_of_coders)
    {
        data->coders[i].id = i + 1;
        data->coders[i].compiles_count = 0;
        data->coders[i].last_compile_time = 0;
        data->coders[i].left_dongle = &data->dongles[i];
        data->coders[i].right_dongle = &data->dongles[(i + 1) % data->args.number_of_coders];
        pthread_mutex_init(&data->coders[i].coder_mutex, NULL);
        i++;
    }
    return 0;
}