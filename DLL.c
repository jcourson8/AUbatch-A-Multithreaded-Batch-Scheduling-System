#include "DLL.h" /* DLL class interface */

/* constructor implementation */
void DLL_ctor(DLL * const me) {
    me->head = NULL;
    me->tail = NULL;  
    me->issorted = false;
}

/* "getter" operations implementation */
node * DLL_gethead(DLL const * const me) {
    return me->head;
}
node * DLL_gettail(DLL const * const me) {
    return me->tail;
}
bool DLL_getissorted(DLL const * const me) {
    return me->issorted;
}

/* "setter" operations implementation */
void DLL_sethead(DLL * const me, node * newhead) {
    me->head = newhead;
}
void DLL_settail(DLL * const me, node * newtail) {
    me->tail = newtail;
}
void DLL_setissorted(DLL * const me, bool issortedval) {
    me->issorted = issortedval;
}

/* DLL functionality*/
void DLL_addnode(DLL * const me, job *curjob) {  

    //Create a new node  
    struct node *newnode = (struct node*)malloc(sizeof(struct node));  
    newnode->thisjob = curjob;  
      
    //If list is empty  
    if(DLL_isempty(me)) {  
        //Both head and tail will point to newnode
        DLL_sethead(me, newnode);
        DLL_settail(me, newnode);
        //head's previous will point to NULL  
        DLL_gethead(me)->prev = NULL;  
        //tail's next will point to NULL, as it is the last node of the list  
        DLL_gettail(me)->next = NULL;  
    }  
    else {  
        //newnode will be added after tail such that tail's next will point to newnode  
        DLL_gettail(me)->next = newnode;  
        //newnode's previous will point to tail  
        newnode->prev = DLL_gettail(me);  
        //newnode will become new tail  
        DLL_settail(me, newnode);  
        //As it is last node, tail's next will point to NULL  
        DLL_gettail(me)->next = NULL;  
    }  
}  

void DLL_display(DLL * const me) {  
    printf("Name    CPU_Time    Priority    Arrival_time    Progress\n");
    node *current_node = DLL_gethead(me);
    if (current_node == NULL) {
        return;
    }
    job *currentJob = current_node->thisjob; // start at the head of the list
    
    while (current_node != NULL) {
        char time_str[9];
        struct timeval time = currentJob->ARRIVAL_TIME;
        struct tm time_tm;
        gmtime_r(&time.tv_sec, &time_tm);
        strftime(time_str, 9, "%T", &time_tm);

        // printf("%s", currentJob->NAME);
        printf("%s      %d          %d        %s        %s\n", currentJob->NAME, currentJob->CPU_TIME, currentJob->PRI, time_str, currentJob->PROGRESS);
        if (current_node->next == NULL) {
            break;
        }
        current_node = current_node->next;
        currentJob = current_node->thisjob; // move to the next job in the list
    }
}

void DLL_displaydone(DLL * const me) {  
    printf("Name    CPU_Time    Priority    Arrival_time    Progress    Start_time    Finish_time    CPU_Time_used\n");
    node *current_node = DLL_gethead(me);
    if (current_node == NULL) {
        return;
    }
    job *currentJob = current_node->thisjob; // start at the head of the list
    
    while (current_node != NULL) {
        char arv_time_str[9];
        struct timeval arv_time = currentJob->ARRIVAL_TIME;
        struct tm arv_time_tm;
        gmtime_r(&arv_time.tv_sec, &arv_time_tm);
        strftime(arv_time_str, 9, "%T", &arv_time_tm);

        char start_time_str[9];
        struct timeval start_time = currentJob->START_TIME;
        struct tm start_time_tm;
        gmtime_r(&start_time.tv_sec, &start_time_tm);
        strftime(start_time_str, 9, "%T", &start_time_tm);

        char finish_str[9];
        struct timeval finish_time = currentJob->FINISH_TIME;
        struct tm finish_time_tm;
        gmtime_r(&finish_time.tv_sec, &finish_time_tm);
        strftime(finish_str, 9, "%T", &finish_time_tm);
 
        printf("%s      %d          %d        %s        %s        %s        %s\n", currentJob->NAME, 
        currentJob->CPU_TIME, currentJob->PRI, arv_time_str, currentJob->PROGRESS, start_time_str,finish_str);

        if (current_node->next == NULL) {
            break;
        }
        current_node = current_node->next;
        currentJob = current_node->thisjob; // move to the next job in the list
    }
}


//sortList() will sort the given list in ascending order  
void DLL_sortlist(DLL * const me, int policy) {  
    struct node *current = NULL, *index = NULL;  
    struct job *temp_job;  
    //Check whether list is empty  
    if(DLL_gethead(me) == NULL) {  
        return;  
    } else {  
        //Current will point to head  
        struct timeval currenttime;
        struct timeval newnodetime;
        // bool istail = false;

        for(current = DLL_gethead(me); current->next != NULL && strcmp(current->next->thisjob->PROGRESS, "Run") != 0 
                                                             && strcmp(current->next->thisjob->PROGRESS, "Fail") != 0 
                                                             && strcmp(current->next->thisjob->PROGRESS, "Done") != 0; current = current->next) {  
            //Index will point to node next to current  
            for(index = current->next; index != NULL && strcmp(index->thisjob->PROGRESS, "Run") != 0 
                                                     && strcmp(index->thisjob->PROGRESS, "Fail") != 0 
                                                     && strcmp(index->thisjob->PROGRESS, "Done") != 0; index = index->next) { 
            
            // if (strcmp(index->thisjob->PROGRESS, "Run") == 0 || 
            //     strcmp(index->thisjob->PROGRESS, "Fail") == 0 || 
            //     strcmp(index->thisjob->PROGRESS, "Done") == 0) break;

                switch (policy) {
                    case FCFS: {
                        //If current's data is greater than index's data, swap the data of current and index  
                        currenttime = current->thisjob->ARRIVAL_TIME;
                        newnodetime = index->thisjob->ARRIVAL_TIME;
                        // t1 = mktime(&currenttime);
                        // t2 = mktime(&newnodetime);
                        
                        // if (cmptime(currenttime,newnodetime))
                        if (cmptime(&currenttime, &newnodetime) < 0) {  
                            temp_job = current->thisjob;  
                            current->thisjob = index->thisjob;  
                            index->thisjob = temp_job;  
                            // if (istail) {

                            // }
                        } 
                        break;
                    }
                    case SJF: {
                        //If current's data is greater than index's data, swap the data of current and index  
                        if(current->thisjob->CPU_TIME < index->thisjob->CPU_TIME) {  
                            temp_job = current->thisjob;  
                            current->thisjob = index->thisjob;  
                            index->thisjob = temp_job;  
                        } 
                        break;
                    }
                    case PRIORITY: {
                        //If current's data is greater than index's data, swap the data of current and index  
                        if(current->thisjob->PRI > index->thisjob->PRI) {  
                            temp_job = current->thisjob;  
                            current->thisjob = index->thisjob;  
                            index->thisjob = temp_job;  
                        } 
                    }
                }
            }  
        }  
    }
    DLL_setissorted(me, true);  
}


//is list empty
bool DLL_isempty(DLL * const me) {
   return DLL_gethead(me) == NULL;
}

int DLL_length(DLL * const me) {
   int length = 0;
   struct node *current;

   for(current = DLL_gethead(me); current != NULL; current = current->next){
      length++;
   }
	
   return length;
}

int DLL_waitingjobs(DLL * const me) {
    int length = 0;
    struct node *current;

    if (DLL_length(me) == 0) {
        return length;
    }
    
	if (DLL_length(me) == 1) {
        current = DLL_gethead(me);
        if (strcmp(current->thisjob->PROGRESS, "Run") != 0 
         && strcmp(current->thisjob->PROGRESS, "Fail") != 0 
         && strcmp(current->thisjob->PROGRESS, "Done") != 0)
            length++;
        return length;
    }
    length = 1; // idk why i have to put this here
    for(current = DLL_gethead(me); current->next != NULL && strcmp(current->next->thisjob->PROGRESS, "Run") != 0 
                                                         && strcmp(current->next->thisjob->PROGRESS, "Fail") != 0 
                                                         && strcmp(current->next->thisjob->PROGRESS, "Done") != 0; current = current->next){
        length++;
    }
	
   return length;
}

void DLL_insertfirst(DLL * const me, job *curjob) {
    DLL_setissorted(me, false); 
    
    //create a link
    struct node *link = (struct node*) malloc(sizeof(struct node));
    link->thisjob = curjob;	

    if(DLL_isempty(me)) {
    //    //make it the last link
    //    DLL_sethead(me, link);
        DLL_addnode(me, curjob);
        return;
    } else {
       //update first prev link
       DLL_gethead(me)->prev = link;
    }

    //point it to old first link
    link->next = DLL_gethead(me);
	
    //point first to new first link
    DLL_sethead(me, link);
}

// // struct Node** head_ref, struct Node* newnode
// void DLL_sortedinsert(DLL * const me, job *curjob, int policy) {

//     struct node *newnode = (struct node*) malloc(sizeof(struct node));
//     newnode->thisjob = curjob;
  
//     // if list is empty
//     if (DLL_isempty(me)) {

//         DLL_addnode(me,curjob);

//     } else {
//         struct node *current = DLL_gethead(me);
//         switch (policy) {
//             case FCFS: {

//                 struct tm currenttime = current->thisjob->ARRIVAL_TIME;
//                 struct tm newnodetime = newnode->thisjob->ARRIVAL_TIME;
       
//                 if (cmptime(&currenttime, &newnodetime) <= 0) {
//                     DLL_insertfirst(me, curjob);
//                     break;
//                 }
//                 while (current->next != NULL) {
//                     currenttime = current->next->thisjob->ARRIVAL_TIME;
//                     if (cmptime(&currenttime, &newnodetime) <= 0) {
//                         current = current->next;
//                     } else {
//                         break; //check on this
//                     }
//                 }
//                 // check first if the last one is bigger
//                 currenttime = current->thisjob->ARRIVAL_TIME;
//                 if (cmptime(&currenttime, &newnodetime) <= 0) {
//                     // insert before last 
//                     newnode->next = current;
//                     newnode->prev = current->prev;
//                     current->prev = newnode;
//                 }
//                 break;
//             }
//             case SJF: {
//                 if (current->thisjob->CPU_TIME <= newnode->thisjob->CPU_TIME) {
//                     DLL_insertfirst(me, curjob);
//                     break;
//                 }
//                 while (current->next != NULL && current->next->thisjob->CPU_TIME < newnode->thisjob->CPU_TIME) {
//                     current = current->next;
//                 }
//                 // check first if the last one is bigger
//                 if(current->thisjob->CPU_TIME >= newnode->thisjob->CPU_TIME) {
//                     // insert before last 
//                     newnode->next = current;
//                     newnode->prev = current->prev;
//                     current->prev = newnode;
//                 }
//                 break;
//             }
//             case PRIORITY: {
//                 if (current->thisjob->PRI >= newnode->thisjob->PRI) {
//                     // newnode->next = current;
//                     // newnode->next->prev = newnode;
//                     // DLL_sethead(me,newnode);
//                     DLL_insertfirst(me, curjob);
//                     break;
//                 }
//                 while (current->next != NULL && current->next->thisjob->PRI < newnode->thisjob->PRI) {
//                     current = current->next;
//                 }
//                 // check first if the last one is bigger
//                 if(current->thisjob->PRI >= newnode->thisjob->PRI) {
//                     // insert before last 
//                     newnode->next = current;
//                     newnode->prev = current->prev;
//                     current->prev = newnode;
//                 }
//                 break;
//             }
//             // insertlast

//             current->next = newnode;
//             newnode->prev = current;
//             newnode->next = NULL;
//         }

//     }
// }

/* Returns which ever one is greater or if they are equal it returns t1*/
int cmptime(struct timeval *t1, struct timeval *t2) {
    // int hourdiff = t1->tm_hour - t2->tm_hour;
    // int mindiff = t1->tm_min - t2->tm_min;
    // int secdiff = t1->tm_sec - t2->tm_sec;
    // int difftotal = (hourdiff * 60 * 24) + (mindiff * 60) + secdiff;

    // do the above but for timeval instead of tm
    int secdiff = t1->tv_sec - t2->tv_sec;
    int usecdiff = t1->tv_usec - t2->tv_usec;
    // int difftotal = (hourdiff * 60 * 24) + (mindiff * 60) + secdiff;
    int difftotal = (secdiff * 1000000) + usecdiff;

    return difftotal;

}

void DLL_removedone(DLL * const me) {
    node * tail = DLL_gettail(me);
    if (tail == NULL) {
        perror("the tail shouldnt be NULL");
    }
    // edge case where length is one
    if (DLL_length(me) == 1) {
        DLL_settail(me, NULL);
        DLL_sethead(me, NULL);
        // return tail;
        return;
    }
    tail->prev->next = NULL;
    DLL_settail(me, tail->prev);
    // return tail;
}

// // must only be called after removetail has successfully returned a node (not NULL)
// void DLL_comsumetail(DLL * const me) {
//     node * tail = DLL_gettail(me);
//     // edge case where length is one
//     if (DLL_length(me) == 1) {
//         DLL_settail(me, NULL);
//         DLL_sethead(me, NULL);
//         return tail;
//     }
//     tail->prev->next = NULL;
//     DLL_settail(me, tail->prev);
//     return tail;
// }