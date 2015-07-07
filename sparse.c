/**
    sparse.c
    Purpose: A simple implementation of sparse array as linked list is an array 
	in which most of the elements have the default value (usually 0 or null). 
	The occurrence of zero-value elements in a large array is inefficient for 
	both computation and storage. An array in which there is a large number of 
	zero elements is referred to as being sparse.

    @author George Papageorgakis
    @version 1.0 10/2011
*/
#include <stdio.h>
#include <stdlib.h>
#define MAX_LENGTH 31

struct node{
	int i, j;
	struct node *previous, *next, *up, *down;
}*temp, V[MAX_LENGTH], H[MAX_LENGTH];

/**
    Inserts node in the horizontal indexing.	
*/
void insert_horizontal(struct node *aux, struct node *temp){
    if (temp->j > aux->j){
		/* if next of aux is not null but there are other elements still to compare with temp
		the function takes the next structure that is being pointed by aux and the node that
		is already in temp.
		else if aux is the last element temp->previous points to aux, the NULL initialized
		*/
		if (aux->next != NULL)
			insert_horizontal(aux->next, temp);
		else{
			temp->previous	= aux;
			aux->next		= temp;
		}
	}
	/* else the temp is placed left of aux, the next of the previous struct 
	of aux must point to temp and aux->previous must show the new struct
	*/
    else if (temp->j < aux->j){
		temp->previous		= aux->previous;
		temp->next			= aux;
		aux->previous->next = temp;			
		aux->previous		= temp;
	}
}


/**
    Inserts node in the vertical indexing.	
*/
void insert_vertical(int *p_check, struct node *aux, struct node *temp){
   *p_check = 0;
    if (temp->i > aux->i){
		/* if down of aux is not null but there are other elements still to compare with temp
		the function takes the structure beneath that is being pointed by aux and the node that
		is already in temp.
		else if aux is the last element temp->up points to aux and temp->down persists as NULL 
		since its the last element
		*/
		if (aux->down != NULL)
			insert_vertical(p_check, aux->down, temp);
		else{
			temp->up  	= aux;
			aux->down 	= temp;
		}
	}
	/* else if temp is placed above the aux, the temp->up must point to where aux
	was pointing to, temp->down must point to aux, the down of the upper structure 
	of aux must point to temp and the aux->up must point to the new struct.
	*/
    else if (temp->i < aux->i){
		temp->up		= aux->up;
		temp->down		= aux;
		aux->up->down	= temp;
		aux->up			= temp;
	}
	//avoid insertion of same element, also works as diagonal index checking
    else
        *p_check = 1;
}

/**
    deletes a node if its safe
*/
void delete_node(struct node *aux){
	if (aux->next != NULL){
        aux->previous->next	= aux->next;
        aux->next->previous	= aux->previous;
    }
	else{
		aux->previous->next =NULL;
	}
    if (aux->down != NULL){
        aux->up->down = aux->down;
        aux->down->up = aux->up;
    }
    else{
		aux->up->down = NULL;
	}
	free(aux);
}

/**
    search for the node and delete it if it exists
*/
void search_delete(struct node *aux, int y){
    if (aux->j != y){
	  	//if not in the last element
		if ((aux->j < y) && (aux->next!=NULL))
			search_delete(aux->next, y);
		else{
			printf("Input Node does not exist.\n");fflush(stdout);
		}
	}
	//else the element exists and is to be deleted
    else{ 
		delete_node(aux);
		printf("Edge deleted.\n"); fflush(stdout);
	}
}

/**
    Print the sparse array
*/
void display(struct node *aux){
	if (aux != NULL){
		printf("(%d,%d)", aux->i, aux->j);
		display(aux->next);
	}
}

/**
    initialize the allocated indices of the matrix
*/
void initialize(void){
	int k;
    for (k=1; k < MAX_LENGTH; ++k){
        V[k].i			= k;
        V[k].j			= 0;
        V[k].next		= NULL;
        V[k].previous	= NULL;
        V[k].up			= NULL;
        V[k].down		= NULL;
        H[k].i			= 0;
        H[k].j			= k;
        H[k].next		= NULL;
        H[k].previous	= NULL;
        H[k].up			= NULL;
        H[k].down		= NULL;
    }
}


int main (void){
	int x = 0, y = 0, k = 0, check = 0, *p_check = &check;
	char ch;
	initialize();
	while (ch!='q'){
		printf("(I)nsert, (D)elete, (P)rint, (Q)uit :");fflush(stdout);
		ch = getchar();fflush(stdin);
		if (ch=='i'||ch=='I'){
			if ((temp = (struct node *) malloc(sizeof(struct node))) != NULL){
				temp->previous	= NULL;
				temp->next		= NULL;
				temp->up		= NULL;
				temp->down		= NULL;
				temp->i			= 0;
				temp->j			= 0;
				printf("Input node i (1-%d):\n", MAX_LENGTH-1 );
				scanf(" %d", &temp->i); fflush(stdin);
				printf("node j:\n");
				scanf(" %d", &temp->j); fflush(stdin);
                if (temp->i > 0 && temp->j > 0 && temp->i < 31 && temp-> j< 31){
					insert_horizontal(&V[temp->i], temp);   //ksekinaei apo tin grammi i
					insert_vertical(p_check, &H[temp->j], temp);
					x = temp->i;
					y = temp->j;
				}
                else{
					printf("Edge cordinates out of range\n");
				}
            }
            else{ 
				printf("Memory is Full");
			}
			
            if ((temp = (struct node *) malloc(sizeof(struct node))) != NULL){
				temp->previous	= NULL;
				temp->next		= NULL;
				temp->up		= NULL;
				temp->down		= NULL;
				temp->i			= 0;
				temp->j			= 0;
				//forbids insertion of diagonal nodes
				if (check == 0){
                    temp->i=y;
                    temp->j=x;
                    insert_horizontal(&V[temp->i],temp);
                    insert_vertical(p_check, &H[temp->j],temp);
                }
                else
					printf("Edge already regeistered\n");
            }
            else 
				printf("Memory is Full");
        }
		else if (ch=='p'|| ch=='P'){
			printf("______________________________________________________________________________\n");
			for (k=1; k < MAX_LENGTH; ++k){
				display(V[k].next);
				printf(" -\n");
			}
			printf("_______________________________________________________________________________\n");
		}
		else if (ch=='d'||ch=='D'){
			printf("Which edge do you want to delete?\ni=");
			scanf(" %d",&x);fflush(stdin);
			printf("j=");
			scanf(" %d",&y);fflush(stdin);
			search_delete(&V[x],y);
			search_delete(&V[y],x);
		}
    }
    fflush(stdout);
    return 0;
}
