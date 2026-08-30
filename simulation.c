#include "codexion.h"



void	get_elapsed_ms(struct timeval start, long *time_ms)
{
	struct timeval	end;

	gettimeofday(&end, NULL);
	*time_ms = (((end.tv_sec - start.tv_sec) * 1000) + ((end.tv_usec
					- start.tv_usec) / 1000));
}

void	update_simulation_current_time(t_data *data, long *current_time_ms)
{
	pthread_mutex_lock(&data->simulation.timer_mutex);
	get_elapsed_ms(data->simulation.start_time_tv, current_time_ms);
	pthread_mutex_unlock(&data->simulation.timer_mutex);
}

int check_game_over(t_data *data){
	int status;

	pthread_mutex_lock(&data->monitor.game_over_mutex);
	status = data->monitor.game_over;
	pthread_mutex_unlock(&data->monitor.game_over_mutex);

	return status;
}

void print_action(t_coder *coder, char *action)
{
    long time;
    pthread_mutex_lock(&coder->data->monitor.game_over_mutex);
    if (coder->data->monitor.game_over == 0)
    {
        get_elapsed_ms(coder->data->simulation.start_time_tv, &time);
        printf("%ld %d %s\n", time, coder->id, action);
    }
    pthread_mutex_unlock(&coder->data->monitor.game_over_mutex);
}



void	*coder_routine(void *coder_ptr)
{
	t_coder		*coder;
	t_request	req;
	long current_time_ms;
	coder = (t_coder *)coder_ptr;
	req.coder_id = coder->id;
	long now;

	while (check_game_over(coder->data) == 0)
	{	
		if (coder->data->args.sch == 0)
		{ // if scheduler == FIFO
			get_elapsed_ms(coder->data->simulation.start_time_tv,
				&current_time_ms);
			req.priority = current_time_ms;
		}
		else if (coder->data->args.sch == 1)
        { // if scheduler == EDF
            long last_compile_from_start;
            
            pthread_mutex_lock(&coder->time_mutex);
            // N-7sbo ch7al dyal l-wa9t daz mn start_time_tv 7ta akher compile
            last_compile_from_start = ((coder->lst_compile_tv.tv_sec - coder->data->simulation.start_time_tv.tv_sec) * 1000) + 
                                      ((coder->lst_compile_tv.tv_usec - coder->data->simulation.start_time_tv.tv_usec) / 1000);
            pthread_mutex_unlock(&coder->time_mutex);
            
            // Priority hiya Deadline dyal L-Mawt b d-dabt (Absolute Time)
            req.priority = last_compile_from_start + coder->data->args.time_burnout;
        }

		pthread_mutex_lock(&coder->coder_mutex);

		pthread_mutex_lock(&coder->data->simulation.heap_mutex);
		heap_push(&coder->data->heap, req);
		pthread_mutex_unlock(&coder->data->simulation.heap_mutex);
		
		pthread_cond_wait(&coder->coder_cond, &coder->coder_mutex);
		pthread_mutex_unlock(&coder->coder_mutex);

	
		if (check_game_over(coder->data) != 0)
			return coder_ptr;

			
		pthread_mutex_lock(&coder->L_dongle->mutex);
		print_action(coder, "has taken a dongle");

		pthread_mutex_lock(&coder->R_dongle->mutex);
		print_action(coder, "has taken a dongle");


		
		pthread_mutex_lock(&coder->time_mutex);
		gettimeofday(&coder->lst_compile_tv, NULL);
		coder->compiles_count++;
		pthread_mutex_unlock(&coder->time_mutex);

		print_action(coder, "is compiling");
		usleep(coder->data->args.time_compile * 1000);
		pthread_mutex_lock(&coder->data->simulation.heap_mutex);
		get_elapsed_ms(coder->data->simulation.start_time_tv, &now);
		coder->L_dongle->available_at = now + coder->data->args.dongle_cooldown;
		coder->R_dongle->available_at = now + coder->data->args.dongle_cooldown; 

		coder->L_dongle->is_free = 1;
		coder->R_dongle->is_free = 1;
		pthread_mutex_unlock(&coder->data->simulation.heap_mutex);



		pthread_mutex_unlock(&coder->L_dongle->mutex);
		pthread_mutex_unlock(&coder->R_dongle->mutex);
		
		print_action(coder, "is debugging");

		usleep(coder->data->args.time_debug * 1000);
		print_action(coder, "is refactoring");

		usleep(coder->data->args.time_refactor * 1000);
	}
	return (coder_ptr);
}

int	monitor(t_data *data)
{
	int top_id;
	int c;
	int i;
	int state;
    long burnout;
	struct timeval current;
	t_request poped;
	long now;

	i = 0;
	c = 0;
	pthread_mutex_init(&data->simulation.timer_mutex, NULL);
	
	gettimeofday(&data->simulation.start_time_tv, NULL);
	while (i < data->args.n_coders)
	{	
		pthread_mutex_lock(&data->coders[i].time_mutex);
		data->coders[i].lst_compile_tv = data->simulation.start_time_tv;
		pthread_mutex_unlock(&data->coders[i].time_mutex);

		state = pthread_create(&data->coders[i].tid, NULL, coder_routine,
				&data->coders[i]);
		// this is an error i have to handle !!!;
		// and i have to destroy threads
		if (state != 0)
			return (1);
		i++;
	}

	while (check_game_over(data) == 0)
    {
        i = 0;
        int finished_coders = 0;
        
        while (i < data->args.n_coders)
        {   
            gettimeofday(&current, NULL);
            
            pthread_mutex_lock(&data->coders[i].time_mutex);
            burnout = ((current.tv_sec - data->coders[i].lst_compile_tv.tv_sec) * 1000) + ((current.tv_usec - data->coders[i].lst_compile_tv.tv_usec) / 1000);
            

            if (data->coders[i].compiles_count >= data->args.n_compiles_required)
                finished_coders++;
                
            pthread_mutex_unlock(&data->coders[i].time_mutex);


            if (burnout >= data->args.time_burnout){
                get_elapsed_ms(data->simulation.start_time_tv, &burnout);
                
                pthread_mutex_lock(&data->monitor.game_over_mutex);
                data->monitor.game_over = 1;
                pthread_mutex_unlock(&data->monitor.game_over_mutex); 
                break;
            }
            i++;
        }
        

        if(check_game_over(data) == 1)
		{
			c = 0;
			while (c < data->args.n_coders)
			{
				pthread_cond_signal(&data->coders[c].coder_cond);
				c++;
			}
			printf("%ld %d burned out\n", burnout, data->coders[i].id);
			break;
		}   

        if (finished_coders == data->args.n_coders)
        {
            pthread_mutex_lock(&data->monitor.game_over_mutex);
            data->monitor.game_over = 1;
            pthread_mutex_unlock(&data->monitor.game_over_mutex);
            

            c = 0;
            while (c < data->args.n_coders) {
                pthread_cond_signal(&data->coders[c].coder_cond);
                c++;
            }
            break;
        }
		int b = 0;
        // ------------------
        // L-MODIR (L-HEAP POP)
        // ------------------
        pthread_mutex_lock(&data->simulation.heap_mutex);
        if(data->heap.used > 0)
        {
            t_request temp_array[250];   // Sndo9 fin n-khebbiw li m-blokyin
            int dongle_needed[250];      // L-Bla9at dyal Reservation 🛑
            int k = 0;
            
            // F l-bdaya, 7tta dongle ma fih bla9a
            while (k < data->args.n_coders) {
                dongle_needed[k] = 0;
                k++;
            }
            
            int temp_count = 0;
            long now;

            // L-Modir kay-9lb f l'Heap kaml b t-tartib
            while (data->heap.used > 0)
            {
                top_id = data->heap.array[0].coder_id - 1;
                get_elapsed_ms(data->simulation.start_time_tv, &now);

                int l_id = top_id;
                int r_id = (top_id + 1) % data->args.n_coders;

                // Wach wajd? W (AND) 7ta wa7d 9bl mnno ma dar reservation l d-dongles dyalo?
                if (data->coders[top_id].L_dongle->is_free &&
                    data->coders[top_id].R_dongle->is_free &&
                    now >= data->coders[top_id].L_dongle->available_at &&
                    now >= data->coders[top_id].R_dongle->available_at &&
                    dongle_needed[l_id] == 0 &&
                    dongle_needed[r_id] == 0)
                {
                    // Wajd w M-ta7! 3tiwh d-dongles w fiy9oh
                    data->coders[top_id].L_dongle->is_free = 0;
                    data->coders[top_id].R_dongle->is_free = 0;

                    poped = heap_pop(&data->heap);
                    pthread_mutex_lock(&data->coders[poped.coder_id - 1].coder_mutex);
                    pthread_cond_signal(&data->coders[poped.coder_id - 1].coder_cond);
                    pthread_mutex_unlock(&data->coders[poped.coder_id - 1].coder_mutex);                    
                    break; // Salina l-9lib!
                }
                else
                {
                    // L-Coder m-bloki. Ghadi n-khebbiwh f Sndo9 (Stash).
                    // WALAKIN, 7it Priority dyalo tal3a, ghadi n-reservéw lih d-dongles dyalo!
                    dongle_needed[l_id] = 1;
                    dongle_needed[r_id] = 1;
                    
                    temp_array[temp_count] = heap_pop(&data->heap);
                    temp_count++;
                }
            }
            
            // Rdd ga3 L-Coders li khebbinahom mn S-sndo9 (temp) l L'Heap bach y-tsnnaw d-dor dyalhom
            k = 0;
            while (k < temp_count)
            {
                heap_push(&data->heap, temp_array[k]);
                k++;
            }
        }
        pthread_mutex_unlock(&data->simulation.heap_mutex);
        // usleep(500);
        
    }

	i = 0;
	while (i < data->args.n_coders)
	{
		state = pthread_join(data->coders[i].tid, NULL);
		i++;
	}

	return (0);
}