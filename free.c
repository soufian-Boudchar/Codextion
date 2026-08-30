#include "codexion.h"

void free_dongles(t_data *data){
    free(data->dongles);
    free(data->coders);
    free(data->heap.array);
}
